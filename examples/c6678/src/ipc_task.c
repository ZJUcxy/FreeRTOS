#include "FreeRTOS.h"
#include "task.h"
#include "app_types.h"
#include "shared_ipc.h"

extern AppStats_t g_stats;

void IpcTask(void *arg)
{
    (void)arg;

    Ipc_BindTask(xTaskGetCurrentTaskHandle());

    for (;;) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        if (g_ipc_mailbox.magic != IPC_MAGIC) {
            g_stats.ipc_errors++;
            continue;
        }

        if (g_ipc_mailbox.status == IPC_STATUS_DONE) {
            g_stats.jobs_done++;
            g_stats.last_seq = g_ipc_mailbox.seq;
            g_ipc_mailbox.status = IPC_STATUS_IDLE;
        } else if (g_ipc_mailbox.status == IPC_STATUS_ERROR) {
            g_stats.ipc_errors++;
            g_ipc_mailbox.status = IPC_STATUS_IDLE;
        }
    }
}
