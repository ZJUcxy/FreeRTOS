# STM32H7 FreeRTOS 电机控制例程

## 目标

在 STM32H7 + FreeRTOS（CMSIS-RTOS v2）上快速搭起：

- 1ms 速度闭环（编码器 + PID + PWM）
- 串口命令 `SPD:<rpm>`
- 通信失联保护停机

## 目录

```text
App/
  Inc/   应用头文件
  Src/   控制 / 通信 / 安全任务与驱动封装
Core/
  Inc/   FreeRTOSConfig 参考、入口声明
  Src/   freertos 任务创建示意、main 示意
```

## 集成步骤

1. CubeMX 生成 H7 工程（TIM PWM + Encoder + UART + FreeRTOS）
2. 把 `App/` 加入工程 Include/Source
3. 按板子修改 `app_config.h` 与 `motor.c` / `encoder.c` 中的句柄、引脚
4. 在 `MX_FREERTOS_Init` 中创建队列与三个任务（参考 `Core/Src/freertos.c`）
5. 编译下载：先开环，再闭环

## 串口协议

```text
下发: SPD:1500\r\n
上报: tar=1500.0 rpm, meas=1492.3 rpm, out=0.42, fault=0x00\r\n
```

## 默认假设

| 外设 | 实例 |
|------|------|
| PWM | `htim1` CH1 |
| Encoder | `htim3` |
| UART | `huart1` |
| 方向脚 | `MOTOR_DIR_GPIO_Port` / `MOTOR_DIR_Pin` |

请在 CubeMX 中创建同名句柄，或全局替换为你的命名。
