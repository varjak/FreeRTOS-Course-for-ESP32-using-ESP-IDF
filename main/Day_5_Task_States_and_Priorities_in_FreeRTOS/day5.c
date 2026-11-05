#include "./main/Day_5_Task_States_and_Priorities_in_FreeRTOS/day5.h"
#include "freertos/FreeRTOS.h"



static void low_prio_print(void *pvParameters) {
    while (1) {
        printf("Low priority printing\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void med_prio_print(void *pvParameters) {
    while (1) {
        printf("Medium priority printing\n");
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

static void high_prio_print(void *pvParameters) {
    int counter = 0;
    while (1) {
        printf("High priority printing\n");
        vTaskDelay(pdMS_TO_TICKS(400));
        counter++;
        if (counter >= 5) {
            vTaskPrioritySet(NULL, 1);
        }
    }
}


void day5_run() {
    xTaskCreate(low_prio_print, "Low Print", 2048, NULL, 2, NULL);
    xTaskCreate(med_prio_print, "Med Print", 2048, NULL, 3, NULL);
    xTaskCreate(high_prio_print, "High Low", 2048, NULL, 4, NULL);
}