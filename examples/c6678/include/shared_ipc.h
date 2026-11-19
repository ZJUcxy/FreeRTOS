#ifndef SHARED_IPC_H
#define SHARED_IPC_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define IPC_MAGIC              0xC66781CCu
#define IPC_CMD_NOP            0U
#define IPC_CMD_PROCESS_BLOCK  1U
#define IPC_STATUS_IDLE        0U
#define IPC_STATUS_BUSY        1U
#define IPC_STATUS_DONE        2U
#define IPC_STATUS_ERROR       3U

typedef struct {
    uint32_t magic;
    uint32_t seq;
    uint32_t cmd;
    uint32_t status;
    uint32_t in_addr;   /* 物理/共享地址，按工程约定 */
    uint32_t out_addr;
    uint32_t length;
    uint32_t result_code;
} IpcMailbox_t;

/* 由链接脚本放置到 MSMC 共享区 */
extern volatile IpcMailbox_t g_ipc_mailbox;

void Ipc_InitMailbox(void);
void Ipc_BindTask(void *task_handle); /* 传入 TaskHandle_t */
void Ipc_SubmitProcess(uint32_t in_addr, uint32_t out_addr, uint32_t length);
void Ipc_NotifyRemoteCore(uint32_t core_id);
void Ipc_OnDoorbellIsr(void); /* Core0 门铃 ISR 调用 */

#ifdef __cplusplus
}
#endif

#endif /* SHARED_IPC_H */
