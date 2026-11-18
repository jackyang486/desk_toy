#ifndef __BATTERY_H__
#define __BATTERY_H__

#ifdef __cplusplus
extern "C" {
#endif

/* 电量检测模块
 * 采集：ADC1_CH8(GPIO9) + 曲线拟合校准，×2 分压还原电池电压
 * 显示：顶栏 battery_label 图标按电量分档刷新（<=20% 变红）
 * 播报：电量 <=20% 时单次播放 battery_low_notice 提示音；
 *       若正处于音乐/电台页，先切回首页（卸载回调会停止播放、释放 codec）
 *       再由独立任务播提示音
 * 机器人页暂不处理（AI 对话占用 codec 时提示音会打断当轮播报）
 */

// 在 setup() 中调用（须在 lvgl_user_init() 之后，内部会创建 LVGL 定时器）
void battery_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __BATTERY_H__ */
