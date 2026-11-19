# 09 STM32H7 实战指南

目标芯片：`STM32H743 / H750 / H723`（Cortex-M7 + FPU）。  
例程路径：`examples/stm32h7/`。

## 9.1 CubeMX 配置清单

1. **RCC**：按板子选 HSE/PLL，确认 `SystemCoreClock`
2. **FreeRTOS**：Interface 选 `CMSIS_V2`，Heap `heap_4`
3. **TIM1**：PWM 输出（电机占空比）
4. **TIM3**：Encoder Mode（正交解码）
5. **USART1**：115200，命令与遥测
6. **NVIC**：外设中断优先级数值 ≥ `configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY`
7. 生成代码后，业务逻辑只改 `App/` 与 `freertos.c` 任务入口

## 9.2 推荐任务表

| 任务 | 优先级 | 周期 | 栈建议 |
|------|--------|------|--------|
| ControlTask | High | 1ms | ≥ 512 words |
| CommTask | Normal | 20ms / 队列 | ≥ 512 words |
| SafetyTask | AboveNormal | 10ms | ≥ 256 words |

## 9.3 关键文件

- `App/Inc/app_config.h` — 控制周期、PID、编码器参数
- `App/Src/control_task.c` — 闭环控制
- `App/Src/comm_task.c` — `SPD:<rpm>` 协议
- `App/Src/safety_task.c` — 失联与限幅保护
- `Core/Src/freertos.c` — 创建队列与任务（示意）

## 9.4 H7 特有注意

1. **Cache**：DMA 缓冲放 `RAM_D2` 或加 `SCB_CleanDCache_by_Addr`
2. **DTCM/ITCM**：可将控制任务栈或关键变量放 DTCM 降延迟
3. **FPU**：确保任务上下文保存浮点（Cube 默认通常已开）
4. **VOS/Flash wait state**：主频升高后确认电源档位

## 9.5 联调步骤

1. 开环固定 PWM，确认方向与编码器增减符号
2. 只开 Kp，观察阶跃跟随
3. 加 Ki 消稳态误差；Kd 慎用
4. 串口发送 `SPD:1000` 验证队列覆盖
5. 拔掉串口线，确认 Safety 在超时后停机
