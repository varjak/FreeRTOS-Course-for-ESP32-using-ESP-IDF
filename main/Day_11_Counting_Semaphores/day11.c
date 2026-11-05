#include "./main/Day_11_Counting_Semaphores/day11.h"
#include "freertos/FreeRTOS.h"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#define SEMAPHORE_MAX 5
#define NUM_CONSUMERS 2
#define GIVE_DELAY 500
#define TAKE_DELAY 400
// #define TAKE_DELAY 400 intentionally shows that if consumers are fast they can deplete semaphore.
// To prevent this, use portMAX_DELAY instead of pdMS_TO_TICKS(TAKE_DELAY)

static SemaphoreHandle_t semaphore_counting;

static void producer(void *pvParameter) {
    TickType_t now_tick = xTaskGetTickCount();
    while (1) {
        if (xSemaphoreGive(semaphore_counting)) {
            printf("Producer gave to semaphore.\n");
        } else {
            printf("Semaphore is full!\n");
        }
        vTaskDelayUntil(&now_tick, pdMS_TO_TICKS(GIVE_DELAY));
    }
}

static void consumer(void *pvParameter) {
    const char *consumer_name = pcTaskGetName(NULL);
    while (1) {
        if (xSemaphoreTake(semaphore_counting, pdMS_TO_TICKS(TAKE_DELAY))) {
            printf("%s took from semaphore.\n", consumer_name);
        } else {
            printf("Semaphore is empty!\n");
        }
    }
}

void day11_run() {
    semaphore_counting = xSemaphoreCreateCounting(SEMAPHORE_MAX, SEMAPHORE_MAX);
    xTaskCreate(producer, "Producer", 2048, NULL, 10, NULL);
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        char consumer_name[11];
        sprintf(consumer_name, "Consumer %c", i);
        xTaskCreate(consumer, consumer_name, 2048, NULL, 10, NULL);
    }
}
