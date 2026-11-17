#include "./main/Day_8_Monitoring_Queue_Usage/day8.h"
#include "freertos/FreeRTOS.h"
#include "esp_random.h"

#define QUEUE_LENGTH 5
#define RANDOM_GEN_DELAY 200

QueueHandle_t queue;
static TaskHandle_t producer_handle = NULL;

static inline const char * check_if_odd_or_even(const uint32_t a) {
    if (a % 2 == 0) {
        return "even";
    } else {
        return "odd";
    }
}

static void producer(void *pvParameters) {
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(RANDOM_GEN_DELAY));
        uint32_t r = esp_random();
        if (xQueueSend(queue, &r, pdMS_TO_TICKS(100)) == pdPASS) {
            printf("Sent this value to Queue: %" PRIu32 "!\n", r);
        } else {
            printf("Could not send value to Queue: it is full! Producer is suspending...\n");
            vTaskSuspend(NULL);
        }
    }
}

static void consumer(void *pvParameters) {
    while (1) {
        uint32_t v;
        if (xQueueReceive(queue, &v, pdMS_TO_TICKS(500)) == pdPASS) {
            printf("Received value from Queue! Value is %" PRIu32 ". It is %s!\n", v, check_if_odd_or_even(v));
            if (eTaskGetState(producer_handle) == eSuspended) {
                printf("Queue is no longer full. Producer is resuming...\n");
                vTaskResume(producer_handle);
            }
        } else {
            printf("Could not receive value to Queue: it is empty!\n");
        }
    }
}

void day8_run() {
    queue = xQueueCreate(QUEUE_LENGTH, 4);
    xTaskCreate(producer, "Producer", 2048, NULL, 5, &producer_handle);
    xTaskCreate(consumer, "Consumer", 2048, NULL, 5, NULL);
}