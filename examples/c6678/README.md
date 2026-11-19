# TMS320C6678 FreeRTOS 例程骨架

## 目标

在 C6678 **Core0** 上运行 FreeRTOS，演示：

1. 周期 Worker 任务（模拟算力负载）
2. 命令队列（与 STM32 例程同构的软件接口）
3. 共享内存 Mailbox + 门铃通知它核（IPC）

> 本目录提供可编译结构与移植挂钩点。`port/` 为教学级适配层，需按 CCS/CGT ABI 与板级 Timer/INTC 完善后才能上板。

## 目录

```text
include/     公共头文件、共享内存布局
src/         应用任务、IPC、启动
cfg/         FreeRTOSConfig、链接段建议
port/        C66x Tick / 上下文切换挂钩（示意）
```

## 推荐运行模型

```text
Core0: FreeRTOS
  - CmdTask      解析主机命令
  - WorkerTask   1ms/10ms 周期处理
  - IpcTask      处理它核完成通知

Core1: 裸循环
  - 等待 mailbox 任务描述符
  - 计算完成后写结果并门铃打断 Core0
```

## 集成步骤（CCS）

1. 新建 C6678 工程，加入 FreeRTOS Kernel 源码 + 本目录 `src/include/cfg`
2. 实现/替换 `port/` 中 Tick 与 `pxPortInitialiseStack`
3. 链接脚本增加 `.shared_msmc` 段（见 `cfg/linker_shared.cmd.fragment`）
4. 先单核验证任务切换，再使能 Core1 IPC

## 与 STM32H7 例程的对应关系

| STM32H7 | C6678 |
|---------|-------|
| ControlTask | WorkerTask |
| CommTask | CmdTask |
| UART 协议 | 主机接口 / 仿真命令注入 |
| 电机 PWM | 可替换为算法加速 offload |
