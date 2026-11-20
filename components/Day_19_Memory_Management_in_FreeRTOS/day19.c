#include "./main/Day_19_Memory_Management_in_FreeRTOS/day19.h"
#include "freertos/FreeRTOS.h"

#define TAG "DAY19"
#define STACK_SIZE_BYTES (3 * 1024)

StaticTask_t task_buffer;
StackType_t task_stack[STACK_SIZE_BYTES];

static void taskDyn(void *pvParameter) {
    while (1) {
        printf("Print from dynamic task\n");
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

static void taskSta(void *pvParameter) {
    while (1) {
        printf("Print from static task\n");
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void day19_run() {
    printf("Free Heap size = %zu\n", xPortGetFreeHeapSize());
    xTaskCreate(taskDyn, "Task Dynamic", 2048, NULL, 5, NULL);
    printf("Free Heap size = %zu\n", xPortGetFreeHeapSize());
    xTaskCreateStatic(taskSta, "Task Static", 2048, NULL, 5, task_stack, &task_buffer);
    printf("Free Heap size = %zu\n", xPortGetFreeHeapSize());
}