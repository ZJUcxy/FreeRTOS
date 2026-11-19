#include "comm_task.h"
#include "app_config.h"
#include "app_telemetry.h"
#include "cmsis_os2.h"
#include "usart.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint8_t s_rx_line[48];
static uint32_t s_rx_len;

static void try_parse_line(void)
{
    float spd;

    if (s_rx_len < 5U) {
        return;
    }
    s_rx_line[s_rx_len] = '\0';

    if (strncmp((char *)s_rx_line, "SPD:", 4) == 0) {
        spd = (float)atof((char *)&s_rx_line[4]);
        (void)osMessageQueuePut(g_targetSpeedQueue, &spd, 0U, 0U);
        AppTelemetry_SetTarget(spd);
    }
}

static void poll_uart_rx(void)
{
    uint8_t ch;

    while (HAL_UART_Receive(&huart1, &ch, 1U, 0U) == HAL_OK) {
        if ((ch == '\r') || (ch == '\n')) {
            if (s_rx_len > 0U) {
                try_parse_line();
                s_rx_len = 0U;
            }
        } else if (s_rx_len < (sizeof(s_rx_line) - 1U)) {
            s_rx_line[s_rx_len++] = ch;
        } else {
            s_rx_len = 0U;
        }
    }
}

void CommTask_Entry(void *argument)
{
    char tx[128];
    int n;
    AppTelemetry_t snap;

    (void)argument;
    s_rx_len = 0U;

    for (;;) {
        poll_uart_rx();
        AppTelemetry_Snapshot(&snap);

        n = snprintf(tx, sizeof(tx),
                     "tar=%.1f rpm, meas=%.1f rpm, out=%.2f, fault=0x%02lX\r\n",
                     (double)snap.target_rpm,
                     (double)snap.meas_rpm,
                     (double)snap.ctrl_out,
                     (unsigned long)snap.fault_flags);
        if (n > 0) {
            (void)HAL_UART_Transmit(&huart1, (uint8_t *)tx, (uint16_t)n, 20U);
        }

        osDelay(COMM_PERIOD_MS);
    }
}
