#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "./main/Day_03_Scheduling_and_Core_Affinity/day3.h"


void print(void *pvParameters) {
    int c = 0;
    while (1) {
        printf("[+2s] Hello from task! (count %d)\n", c++);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void day3_run() {
    xTaskCreate(print, "BlinkLED", 2048, NULL, 5, NULL);
}