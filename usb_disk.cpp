/*
 * U盘模式实现（USB MSC -> SD_MMC 扇区桥接）
 *
 * 原理：
 *  - USBMSC 的全局对象在 C++ 静态构造阶段（app_main 之前）就调用
 *    tinyusb_enable_interface(USB_INTERFACE_MSC, ...)，因此无论
 *    "USB CDC On Boot" 开关如何，MSC 接口都会出现在描述符里，
 *    可以与 CDC 串口日志共存为复合设备。
 *  - USBMSC::begin() 只是填充 LUN 的容量参数；主机查询容量时实时读取，
 *    所以开机先 USB.begin()（平时 mediaPresent=false，电脑只看到"无介质"），
 *    进入 U 盘模式时再 begin()+mediaPresent(true)，无需重新枚举。
 *  - onRead/onWrite 回调给出的是 LBA 扇区级访问，直接映射到
 *    SD_MMC.readRAW()/writeRAW()。
 *
 * 并发安全：
 *  - 回调只在 s_active 为 true 时才真正访问 SD；
 *  - 进入前提由 usb_disk_can_enter() 保证（音乐引擎已停、SD 已挂载）；
 *  - UI 遮罩（custom.c）阻止用户在 U 盘模式期间打开音乐/相册等页面。
 */

#include <Arduino.h>
#include <USB.h>
#include <USBMSC.h>
#include <SD_MMC.h>
#include "usb_disk.h"
#include "music_audio.h"

#if SOC_USB_OTG_SUPPORTED && CONFIG_TINYUSB_MSC_ENABLED

/* 全局对象：构造函数里就向 TinyUSB 注册 MSC 描述符（仅 _lun==0 生效一次） */
static USBMSC s_msc;
static bool   s_inited   = false;
static bool   s_active   = false;
static volatile bool s_eject_req = false;

/* 主机读：lba 起的 bufsize 字节（通常为扇区整数倍，不是的话只处理完整扇区） */
static int32_t ud_read_cb(uint32_t lba, uint32_t offset, void *buffer, uint32_t bufsize) {
    (void)offset;
    const uint16_t sec = SD_MMC.sectorSize();
    if (!s_active || sec == 0) {
        return -1;
    }
    bufsize = (bufsize / sec) * sec;
    if (bufsize == 0) {
        return 0;
    }
    uint8_t *dst = (uint8_t *)buffer;
    const uint32_t n = bufsize / sec;
    for (uint32_t i = 0; i < n; i++) {
        if (!SD_MMC.readRAW(dst, lba + i)) {
            return (int32_t)(i * sec);   /* 部分成功，0 表示全失败 */
        }
        dst += sec;
    }
    return (int32_t)bufsize;
}

/* 主机写：同样按扇区切分 */
static int32_t ud_write_cb(uint32_t lba, uint32_t offset, uint8_t *buffer, uint32_t bufsize) {
    (void)offset;
    const uint16_t sec = SD_MMC.sectorSize();
    if (!s_active || sec == 0) {
        return -1;
    }
    bufsize = (bufsize / sec) * sec;
    if (bufsize == 0) {
        return 0;
    }
    const uint8_t *src = buffer;
    const uint32_t n = bufsize / sec;
    for (uint32_t i = 0; i < n; i++) {
        if (!SD_MMC.writeRAW((uint8_t *)src, lba + i)) {
            return (int32_t)(i * sec);
        }
        src += sec;
    }
    return (int32_t)bufsize;
}

/* Start/Stop Unit：电脑端"安全弹出"时 start=false, load_eject=true */
static bool ud_start_stop_cb(uint8_t power_condition, bool start, bool load_eject) {
    (void)power_condition;
    if (!start && load_eject) {
        s_eject_req = true;   /* UI 层轮询后自动退出 U 盘模式 */
    }
    return true;
}

void usb_disk_init(void) {
    if (s_inited) {
        return;
    }
    s_inited = true;

    s_msc.vendorID("Espressif");
    s_msc.productID("DeskToy-SD");
    s_msc.productRevision("1.0");
    s_msc.onStartStop(ud_start_stop_cb);
    s_msc.onRead(ud_read_cb);
    s_msc.onWrite(ud_write_cb);
    s_msc.isWritable(true);
    s_msc.mediaPresent(false);   /* 平时不暴露盘符 */
    Serial.println("[UDISK] USB MSC interface registered (not started)");
}

bool usb_disk_can_enter(void) {
    if (SD_MMC.cardType() == CARD_NONE || SD_MMC.sectorSize() == 0) {
        return false;   /* SD 未挂载 */
    }
    if (music_audio.is_active()) {
        return false;   /* 音乐引擎正占用 SD（含暂停态：文件句柄还开着） */
    }
    return true;
}

bool usb_disk_enter(void) {
    if (s_active) {
        return true;
    }
    if (!usb_disk_can_enter()) {
        return false;
    }
    const uint32_t count = SD_MMC.numSectors();
    const uint16_t size  = SD_MMC.sectorSize();
    if (count == 0 || size == 0) {
        return false;
    }
    if (!s_msc.begin(count, size)) {
        return false;
    }
    USB.productName("DeskToy");
    USB.begin();   /* 第一次进入模式时才启动 TinyUSB（从此串口日志的 CDC 让位给 OTG） */
    s_eject_req = false;
    s_msc.mediaPresent(true);   /* 主机下一次 Test Unit Ready 即可挂载 */
    s_active = true;
    Serial.printf("[UDISK] MSC started: %u sectors x %u bytes (%.1f MB)\n",
                  (unsigned)count, (unsigned)size, (float)count * size / 1048576.0f);
    return true;
}

void usb_disk_exit(void) {
    if (!s_active) {
        return;
    }
    s_msc.mediaPresent(false);   /* 主机看到介质移除 */
    s_active = false;
    Serial.println("[UDISK] MSC stopped, rebooting to restore USB-Serial-JTAG...");
    Serial.flush();
    delay(200);
    esp_restart();   /* OTG 拿走的 GPIO19/20 无法还回（无 tinyusb_deinit），
                        只能重启把端口还给 USB-Serial-JTAG，同时 SD 卡全新重挂载 */
}

bool usb_disk_active(void) {
    return s_active;
}

bool usb_disk_take_eject_request(void) {
    if (!s_eject_req) {
        return false;
    }
    s_eject_req = false;
    return true;
}

#else /* 非 S3 或 TinyUSB MSC 未启用：空实现，保证可编译 */

void usb_disk_init(void) {}
bool usb_disk_can_enter(void) { return false; }
bool usb_disk_enter(void) { return false; }
void usb_disk_exit(void) {}
bool usb_disk_active(void) { return false; }
bool usb_disk_take_eject_request(void) { return false; }

#endif /* SOC_USB_OTG_SUPPORTED && CONFIG_TINYUSB_MSC_ENABLED */
