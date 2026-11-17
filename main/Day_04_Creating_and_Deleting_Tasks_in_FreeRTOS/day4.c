#include "./main/Day_4_Creating_and_Deleting_Tasks_in_FreeRTOS/day4.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_GPIO GPIO_NUM_22
#define BLINK_DELAY 500
#define BLINK_END 5000

static TaskHandle_t task_handle_blink = NULL;

static void blink_LED(void *pvParameters) {
    gpio_reset_pin(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    int counter = 0;
    while (1) {
        gpio_set_level(LED_GPIO, 0);
        vTaskDelay(pdMS_TO_TICKS(BLINK_DELAY));
        gpio_set_level(LED_GPIO, 1);
        vTaskDelay(pdMS_TO_TICKS(BLINK_DELAY));
        counter++;
        if (counter >= 10) {
            printf("Blink Task deleting itself\n");
            vTaskDelete(NULL);
        }
    }
}

static void manager_blink(void *pvParameters) {
    vTaskDelay(pdMS_TO_TICKS(BLINK_END));
    if (task_handle_blink != NULL) {
        printf("Manager Task deleting Blink Task\n");
        vTaskDelete(task_handle_blink);
    }
    printf("Manager Task deleting itself\n");
    vTaskDelete(NULL);
}

void day4_run() {
    xTaskCreate(blink_LED, "Blink LED", 2048, NULL, 5, &task_handle_blink);
    xTaskCreate(manager_blink, "Manager Blink", 2048, NULL, 4, NULL);
}
