/**
 * @file core1_stub.c
 * @brief Core1 裸机从核示例：轮询/门铃唤醒后处理 mailbox 作业
 *
 * 不跑 FreeRTOS。由二级 Boot/IPC 启动到本 main。
 */
#include "shared_ipc.h"

/* 板级：等待门铃 / 自旋 */
static void wait_for_job(void)
{
    while (g_ipc_mailbox.status != IPC_STATUS_BUSY) {
        /* idle / WFE 等价物 */
    }
}

static void process_block(void)
{
    /*
     * 在此执行 FFT / 滤波 / 多轴运算。
     * in_addr/out_addr/length 来自 mailbox。
     * 处理前后按需做 cache invalidate/writeback。
     */
    (void)g_ipc_mailbox.in_addr;
    (void)g_ipc_mailbox.out_addr;
    (void)g_ipc_mailbox.length;
}

static void notify_core0_done(void)
{
    g_ipc_mailbox.result_code = 0U;
    g_ipc_mailbox.status = IPC_STATUS_DONE;
    /*
     * 触发 Core0 门铃：
     * Ipc_NotifyRemoteCore(0);
     */
}

int main(void)
{
    for (;;) {
        wait_for_job();
        if (g_ipc_mailbox.cmd == IPC_CMD_PROCESS_BLOCK) {
            process_block();
            notify_core0_done();
        } else {
            g_ipc_mailbox.status = IPC_STATUS_ERROR;
            g_ipc_mailbox.result_code = 1U;
        }
    }
}
