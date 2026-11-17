#include "./main/Day_23_Debugging_FreeRTOS_Applications/day23.h"
#include "freertos/FreeRTOS.h"
#include <stdio.h>
#include "freertos/task.h"
#define TAG "DAY23"

// Had to enable in menuconfig (Component > FreeRTOS > Kernel):
// configUSE_TRACE_FACILITY
// configUSE_STATS_FORMATTING_FUNCTIONS
// configGENERATE_RUN_TIME_STATS

void debug_task(void *pvParameters) {
    const char * name = (char *) pvParameters;
    while (1) {
        // Print stack high-water mark
        UBaseType_t watermark = uxTaskGetStackHighWaterMark(NULL);
        printf("%s high-water mark: %u words\n", name, watermark);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void day23_run() {
    // Create the debug task
    xTaskCreate(debug_task, "DebugTask0", 2048, (void *) "DebugTask0", 5, NULL);
    xTaskCreate(debug_task, "DebugTask1", 1400, (void *) "DebugTask1", 5, NULL);
    // If StackDepth is set to 600, it triggers a stack overflow as requested in the exercise:
    // ***ERROR*** A stack overflow in task DebugTask2 has been detected.
    xTaskCreate(debug_task, "DebugTask2", 1400, (void *) "DebugTask2", 5, NULL);

    // Example: Print runtime stats periodically
    char buffer[512];
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(5000));
        vTaskList(buffer);
        printf("Task List:\n%s\n", buffer);

        vTaskGetRunTimeStats(buffer);
        printf("Runtime Stats:\n%s\n", buffer);
    }
}