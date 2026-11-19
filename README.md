# FreeRTOS 开发指南

面向嵌入式实时系统的 FreeRTOS 实战手册，并附带 **STM32H7** 与 **TMS320C6678** 两套可参考例程骨架。

## 文档导航

| 章节 | 内容 |
|------|------|
| [01 概述与选型](docs/01_overview.md) | 为什么用 FreeRTOS、版本选择、工程组织 |
| [02 内核概念](docs/02_kernel_concepts.md) | 调度、优先级、Tick、临界区 |
| [03 任务设计](docs/03_tasks.md) | 任务模型、栈、状态机、周期任务 |
| [04 任务间通信](docs/04_ipc.md) | Queue / Semaphore / Mutex / EventGroup / StreamBuffer |
| [05 内存与堆](docs/05_memory.md) | heap_1~5、静态分配、MPU 注意点 |
| [06 中断与 ISR](docs/06_interrupt_isr.md) | FromISR API、优先级规则、延迟处理 |
| [07 移植要点](docs/07_porting.md) | Port 层、时钟、Tickless、多核 |
| [08 调试与调优](docs/08_debug_tuning.md) | 栈溢出、死锁、CPU 占用、Trace |
| [09 STM32H7 实战](docs/09_stm32h7_guide.md) | CubeMX + HAL + CMSIS-RTOS v2 |
| [10 C6678 实战](docs/10_c6678_guide.md) | C66x 移植、定时器 Tick、核间协作 |

## Git 分支约定

| 分支 | 内容 |
|------|------|
| `main` | 完整快照：文档 + 双平台例程 + 算法 |
| `stm32` | 仅 STM32H7 FreeRTOS 例程 |
| `c6678` | 仅 TMS320C6678 FreeRTOS 例程 |
| `algorithm` | 仅绝对式 PID / 增量式 PID / LADRC |

## 例程与算法目录

```text
examples/
  stm32h7/     # STM32H743/H750 风格：电机控制 + 通信任务
  c6678/       # TMS320C6678 风格：核0调度 + IPC 通知例程
algorithms/    # 控制算法（亦维护于 algorithm 分支）
```

- [STM32H7 例程说明](examples/stm32h7/README.md)
- [C6678 例程说明](examples/c6678/README.md)
- [算法库说明](algorithms/README.md)

## 快速开始建议

1. 先通读 `01~06`，建立统一任务/通信模型。
2. 按你的芯片打开对应实战章节（`09` 或 `10`）。
3. 把例程中的 `App/` 层直接迁入现有工程，替换引脚/定时器实例名即可联调。

## 推荐版本

- FreeRTOS Kernel：**V10.5.x / V11.x**
- STM32：CubeMX 生成 CMSIS-RTOS v2（底层仍是 FreeRTOS）
- C6678：CCS + TI CGT，配合本仓库 `port` 适配说明

## 许可说明

本指南与例程代码用于学习与工程参考；FreeRTOS 本体遵循 Amazon FreeRTOS / MIT 许可，请在产品中保留对应版权声明。
