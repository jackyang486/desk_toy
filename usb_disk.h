#ifndef __USB_DISK_H__
#define __USB_DISK_H__

/*
 * U盘模式（USB MSC）：把整张 SD 卡通过 ESP32-S3 内置 USB-OTG 暴露给电脑，
 * 电脑端出现一个可读写的 U 盘，直接拖拽拷贝歌曲/照片。
 *
 * 约束（FAT 表并发安全）：
 *  - MSC 工作期间 ESP32 侧不得读写 SD 卡（音乐播放/相册/扫描都会破坏 FAT）。
 *  - 所以做成"模式互斥"：平时 mediaPresent=false，电脑看不到盘符；
 *    进入 U 盘模式才点亮；电脑端"安全弹出"或手动退出后恢复正常访问。
 *  - UI 层（custom.c）负责入口与全屏遮罩，本模块只管 MSC 本身。
 */

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 开机调用一次：注册 MSC 接口描述符并启动 USB 栈（幂等，重复调用无害） */
void usb_disk_init(void);

/* 是否允许进入 U 盘模式：SD 卡已挂载、且音乐引擎未占用 SD */
bool usb_disk_can_enter(void);

/* 进入 U 盘模式（把 SD 卡容量暴露给主机）；成功返回 true */
bool usb_disk_enter(void);

/* 退出 U 盘模式（主机看到介质移除，之后 ESP32 侧才恢复 SD 访问） */
void usb_disk_exit(void);

/* 当前是否处于 U 盘模式 */
bool usb_disk_active(void);

/* 电脑端"安全弹出"后置位；由 UI 轮询取走（取走即清零） */
bool usb_disk_take_eject_request(void);

#ifdef __cplusplus
}
#endif

#endif /* __USB_DISK_H__ */
