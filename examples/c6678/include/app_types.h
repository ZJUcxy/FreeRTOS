#ifndef APP_TYPES_H
#define APP_TYPES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    APP_CMD_SET_RATE = 1,
    APP_CMD_START_JOB = 2,
    APP_CMD_STOP = 3
} AppCmdType_t;

typedef struct {
    AppCmdType_t type;
    uint32_t param0;
    uint32_t param1;
} AppCmd_t;

typedef struct {
    uint32_t jobs_done;
    uint32_t ipc_errors;
    uint32_t worker_ticks;
    uint32_t last_seq;
} AppStats_t;

#ifdef __cplusplus
}
#endif

#endif /* APP_TYPES_H */
