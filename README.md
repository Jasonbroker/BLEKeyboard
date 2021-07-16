# BLEKeyboard
nrf52832 nrf52810 ble keyboard firmware

## WHY 
Learm more about hardware design and engineering.

## WHAT
Implement a full featured powerful and extendable BLE keyboard firmware.

## HOW
Write it at from beginning and add features for every version continuously.


## version history
v0.1
This version contains is the source code for a sample application using the HID, Battery and Device
 * Information Services for implementing a simple keyboard functionality.
 * Pressing Button 0 will send text 'hello' to the connected peer. On receiving output report,
 * it toggles the state of LED 2 on the mother board based on whether or not Caps Lock is on.
 * This application uses the @ref app_scheduler.
 v0.5
add tmk_core lib and finish basic feature.

# BLEKeyboard
使用NRF52832、52810 蓝牙5.0 键盘主控固件

## 缘起
学习硬件设计和硬件工程

## 工作
实现一个高度自定义、丰富功能的蓝牙键盘固件

## 做法
从0开始开发一块蓝牙键盘固件
会在b站同步更新开发视频，功能演进
每个版本增加一些功能~

## FAQ 
为了方便大家、尤其是小白使用，将SDK依赖文件全部上传到项目中，导致项目比较大，如果觉得下载慢，可以从gitee中克隆项目，或直接下载zip

## version history
v0.1 文件结构化、用nordic实例项目用作初始化
v0.5 移植tmk键盘控制逻辑
