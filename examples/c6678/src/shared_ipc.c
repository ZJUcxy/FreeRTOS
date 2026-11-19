#include "shared_ipc.h"
#include "FreeRTOS.h"
#include "task.h"

/* 放到 MSMC 共享段 */
#pragma DATA_SECTION(g_ipc_mailbox, ".shared_msmc")
volatile IpcMailbox_t g_ipc_mailbox;

static TaskHandle_t s_ipc_task;

void Ipc_BindTask(void *task_handle)
{
    s_ipc_task = (TaskHandle_t)task_handle;
}

void Ipc_InitMailbox(void)
{
    g_ipc_mailbox.magic = IPC_MAGIC;
    g_ipc_mailbox.seq = 0U;
    g_ipc_mailbox.cmd = IPC_CMD_NOP;
    g_ipc_mailbox.status = IPC_STATUS_IDLE;
    g_ipc_mailbox.in_addr = 0U;
    g_ipc_mailbox.out_addr = 0U;
    g_ipc_mailbox.length = 0U;
    g_ipc_mailbox.result_code = 0U;
}

void Ipc_SubmitProcess(uint32_t in_addr, uint32_t out_addr, uint32_t length)
{
    g_ipc_mailbox.in_addr = in_addr;
    g_ipc_mailbox.out_addr = out_addr;
    g_ipc_mailbox.length = length;
    g_ipc_mailbox.cmd = IPC_CMD_PROCESS_BLOCK;
    g_ipc_mailbox.status = IPC_STATUS_BUSY;
    g_ipc_mailbox.seq++;

    /* 实际工程：此处写回 cache，再触发 Core1 门铃 */
    Ipc_NotifyRemoteCore(1U);
}

void Ipc_NotifyRemoteCore(uint32_t core_id)
{
    (void)core_id;
    /*
     * 板级实现示例：
     * 1) 写 IPCGR / 软件中断寄存器
     * 2) 或调用 TI IPC Notify_sendEvent
     */
}

void Ipc_OnDoorbellIsr(void)
{
    BaseType_t hpw = pdFALSE;

    if (s_ipc_task != NULL) {
        vTaskNotifyGiveFromISR(s_ipc_task, &hpw);
        portYIELD_FROM_ISR(hpw);
    }
}
