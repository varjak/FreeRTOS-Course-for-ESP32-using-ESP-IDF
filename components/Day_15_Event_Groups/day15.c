#include "./main/Day_15_Event_Groups/day15.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"

#define BIT_TASK_A (1U << 0)
#define BIT_TASK_B (1U << 1)
#define BIT_TASK_C (1U << 2)
#define DELAY_1 2000
#define DELAY_2 4000
#define DELAY_3 6000
#define TAG "DAY15"

EventGroupHandle_t event_group;

void taskA(void *pv) {
    vTaskDelay(pdMS_TO_TICKS(DELAY_1));
    ESP_LOGI(TAG, "Task A finished, setting BIT_TASK_A");
    xEventGroupSetBits(event_group, BIT_TASK_A);
    vTaskDelete(NULL);
}

void taskB(void *pv) {
    vTaskDelay(pdMS_TO_TICKS(DELAY_2));
    ESP_LOGI(TAG, "Task B finished, setting BIT_TASK_B");
    xEventGroupSetBits(event_group, BIT_TASK_B);
    vTaskDelete(NULL);
}

void taskC(void *pv) {
    vTaskDelay(pdMS_TO_TICKS(DELAY_3));
    ESP_LOGI(TAG, "Task C finished, setting BIT_TASK_C");
    xEventGroupSetBits(event_group, BIT_TASK_C);
    vTaskDelete(NULL);
}

static unsigned int count_bits(EventBits_t v) {
    unsigned int n = 0;
    while (v) {
        n += v & 1;
        v >>= 1;
    }
    return n;
}

void coordinator(void *pv) {
    ESP_LOGI(TAG, "Coordinator waiting for task A and B...");
    const EventBits_t required_bits = BIT_TASK_A | BIT_TASK_B;

    EventBits_t bits = xEventGroupWaitBits(
        event_group,
        required_bits,
        pdTRUE,
        pdTRUE,
        portMAX_DELAY
    );

    if ((bits & required_bits) == required_bits) {
        ESP_LOGI(TAG, "ALl tasks are finished, coordinator proceeds!");
    }

    vTaskDelete(NULL);
}

void coordinator2(void *pv) {
    ESP_LOGI(TAG, "Coordinator waiting for task A and B...");
    const EventBits_t required_bits = BIT_TASK_A | BIT_TASK_B | BIT_TASK_C;

    while (1) {
        EventBits_t bits = xEventGroupWaitBits(
            event_group,
            required_bits,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY
        );

        if (count_bits((bits & required_bits))>=2) {
            ESP_LOGI(TAG, "Two of the three tasks are finished, coordinator proceeds!");
            break;
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
    vTaskDelete(NULL);
}

void day15_run() {
    event_group = xEventGroupCreate();
    xTaskCreate(taskA, "taskA", 2048, NULL, 5, NULL);
    xTaskCreate(taskB, "taskB", 2048, NULL, 5, NULL);
    xTaskCreate(taskC, "taskC", 2048, NULL, 5, NULL);
    xTaskCreate(coordinator2, "coordinator", 2048, NULL, 5, NULL);
}