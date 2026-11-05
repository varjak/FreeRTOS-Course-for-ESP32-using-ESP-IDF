#include "./main/Day_6_Using_vTaskDelay_and_vTaskDelayUntil/day6.h"
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#define LED_GPIO GPIO_NUM_22
#define BLINK_DELAY 1000
#define SAMPLING_DELAY 200


static void sensor_sampling(void *pvParameters) {
    while (1) {
        printf("Sampled a value!\n");
        vTaskDelay(pdMS_TO_TICKS(SAMPLING_DELAY));
    }
}
static void blink_LED(void *pvParameters) {
    gpio_reset_pin(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    while (1) {
        printf("Blinking!\n");
        gpio_set_level(LED_GPIO, 0);
        vTaskDelay(pdMS_TO_TICKS(BLINK_DELAY));
        gpio_set_level(LED_GPIO, 1);
        vTaskDelay(pdMS_TO_TICKS(BLINK_DELAY));
    }
}

void day6_run() {
    xTaskCreate(sensor_sampling, "Sensor Sampling", 2048, NULL, 2, NULL);
    xTaskCreate(blink_LED, "LED Blinking", 2048, NULL, 3, NULL);
}