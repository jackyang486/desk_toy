#pragma once

#include <Arduino.h>
#include <string>

// USB 摄像头（UVC）拍照模块。
// 依赖 ESP32_USB_Stream 库（espressif 官方 Arduino 库，封装 esp-iot-solution 的
// usb_stream 组件，与 esp-iot-solution usb_camera_mic_spk 示例同一驱动）。
//
// 用法：MCP "拍照看看有什么" 时调 capture_to_file 拍一帧 JPEG 存进 SD 卡，
// 之后走 VisionDescribeImage 上传识图。USB host 任务与摄像头常驻（非拍照时
// 流保持 suspend 不收帧），首次调用 init 需要几秒枚举摄像头。
class Usb_camera {
public:
    static Usb_camera& GetInstance();

    Usb_camera(const Usb_camera&) = delete;
    Usb_camera& operator=(const Usb_camera&) = delete;

    // 拍一帧 JPEG 写到 path（SD_MMC 路径，如 "/capture.jpg"，每次覆盖）。
    // 失败时 err_msg 给出可读原因（摄像头未接 / 超时 / SD 写失败等）。
    // 注意：SD_MMC 与音乐解码任务互斥，播歌期间不能调用（调用方守卫）。
    bool capture_to_file(const char* path, std::string& err_msg);

    // USB host 是否已启动（与 U 盘 MSC 设备模式互斥：S3 只有一个 OTG 外设）
    static bool host_started();

    // 释放摄像头全部资源（USB host 任务、缓冲、控件）：离开功能页时调用。
    // 拍照进行中则不释放（返回 false）；释放后 host_started 变 false，
    // OTG 归还（U 盘模式可用），下次拍照重新枚举（约需 5 秒）。
    bool release();

private:
    Usb_camera() = default;
    bool init(std::string& err_msg);  // 懒初始化（只执行一次）
};
