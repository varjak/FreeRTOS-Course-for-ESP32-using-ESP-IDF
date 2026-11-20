#include "./main/Day_13_Avoiding_Priority_Inversion/day13.h"
#include "freertos/FreeRTOS.h"

SemaphoreHandle_t lock;

static void busy_ms(uint32_t ms) {
    TickType_t start = xTaskGetTickCount();
    while ((xTaskGetTickCount() - start) < pdMS_TO_TICKS(ms)) { }   // burns CPU
}

void low_task(void *pv) {
    while (1) {
        xSemaphoreTake(lock, portMAX_DELAY);
        printf("Low: took lock\n");
        busy_ms(1000);                    // Real work while holding it
        printf("Low: giving lock\n");
        xSemaphoreGive(lock);
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

void high_task(void *pv) {
    vTaskDelay(pdMS_TO_TICKS(200));       // Let low take the lock first
    printf("High: waiting\n");
    xSemaphoreTake(lock, portMAX_DELAY);
    printf("High: got lock\n");
    xSemaphoreGive(lock);
    printf("High: done\n");
    vTaskDelete(NULL);
}

void medium_task(void *pv) {
    vTaskDelay(pdMS_TO_TICKS(300));       // Wakes while low holds the lock
    printf("Medium: hogging CPU\n");
    busy_ms(3000);                        // (Keep it under the watchdog limit to prevent warning)
    printf("Medium: done\n");
    vTaskDelete(NULL);
}

void day13_run() {
    lock = xSemaphoreCreateBinary();  // To notice priority inversion
    // lock = xSemaphoreCreateMutex();
    xSemaphoreGive(lock);                     // start available
    xTaskCreatePinnedToCore(low_task,    "Low",  4096, NULL, 2,  NULL, 0);
    xTaskCreatePinnedToCore(high_task,   "High", 4096, NULL, 10, NULL, 0);
    xTaskCreatePinnedToCore(medium_task, "Med",  4096, NULL, 5,  NULL, 0);
}
