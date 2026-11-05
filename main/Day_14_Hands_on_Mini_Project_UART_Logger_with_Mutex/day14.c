#include "./main/Day_14_Hands_on_Mini_Project_UART_Logger_with_Mutex/day14.h"
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"

#define TAG "DAY14"
#define UART_PORT UART_NUM_0
#define BUF_SIZE 1024

static SemaphoreHandle_t uart_mutex = NULL;


static void uart_logger(const char *msg) {
    if (!msg || !uart_mutex) {
        return;
    }
    if (xSemaphoreTake(uart_mutex, portMAX_DELAY) == pdTRUE) {
        uart_write_bytes(UART_PORT, msg, (size_t)strlen(msg));
        uart_write_bytes(UART_PORT, "\n", 1);
        xSemaphoreGive(uart_mutex);
    }
}

static void taskA(void *pvParameter) {
    (void)pvParameter;
    while (1) {
        uart_logger("Task A: Writing log message...");
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

static void taskB(void *pvParameter) {
    (void)pvParameter;
    while (1) {
        uart_logger("Task B: Writing log message...");
        vTaskDelay(pdMS_TO_TICKS(800));
    }
}

static void taskC(void *pvParameter) {
    (void)pvParameter;
    while (1) {
        uart_logger("Task C: Writing log message...");
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void day14_run() {
    // Create UART mutex
    uart_mutex = xSemaphoreCreateMutex();
    if (uart_mutex == NULL) {
        printf("Failed to create mutex\n");
        return;
    }

    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    uart_driver_install(UART_PORT, BUF_SIZE, BUF_SIZE, 0, NULL, 0);
    uart_param_config(UART_PORT, &uart_config);

    // Create logging tasks (same priority -> time-sliced on equal priority cores).
    xTaskCreate(taskA, "TaskA", 2048, NULL, 5, NULL);
    xTaskCreate(taskB, "TaskB", 2048, NULL, 5, NULL);
    xTaskCreate(taskC, "TaskC", 2048, NULL, 5, NULL);
}
