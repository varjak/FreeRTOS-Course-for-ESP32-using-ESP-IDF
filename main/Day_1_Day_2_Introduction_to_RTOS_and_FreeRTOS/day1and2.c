/**
 * @file main.c
 * @brief Simple ESP-IDF example demonstrating a FreeRTOS task that prints a message periodically.
 *
 * This example creates one FreeRTOS task that prints "Hello from FreeRTOS!"
 * every second using vTaskDelay for timing control. It illustrates the basic
 * structure for creating and running tasks in an ESP32 application.
 */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "day1and2.h"
#include "esp_log.h"
#include "driver/gpio.h"

#define LED_GPIO GPIO_NUM_22
static const char *TAG = "blink";

void blink_LED(void *pvParameters) {
    gpio_reset_pin(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);

    while (1) {

        ESP_LOGI(TAG, "LED off");
        gpio_set_level(LED_GPIO, 0);
        vTaskDelay(pdMS_TO_TICKS(2000)); // Wait 2 seconds

        // Turn LED ON
        ESP_LOGI(TAG, "LED on");
        gpio_set_level(LED_GPIO, 1);
        vTaskDelay(pdMS_TO_TICKS(2000)); // Wait 2 seconds
    }
}

/**
 * @brief Task function that repeatedly prints a message to the console.
 *
 * This task runs indefinitely in a loop, printing "Hello from FreeRTOS!"
 * every second. The delay is implemented using vTaskDelay().
 *
 * @param pvParameters Pointer to optional parameters passed during task creation (unused here).
 */
void hello_task(void *pvParameters) {
    while (1) {
        printf("Hello from FreeRTOS!\n");
        vTaskDelay(pdMS_TO_TICKS(1000)); // delay for 1 second
    }
}

/**
 * @brief Application entry point for the ESP-IDF program.
 *
 * This function creates the hello_task with a stack size of 2048 bytes and
 * a priority level of 5. It does not return, as the FreeRTOS scheduler takes over.
 */
void day1and2_run() {
    ESP_LOGI(TAG, "app_main started");
    xTaskCreate(
        blink_LED,       // Task function
        "BlinkLED",      // Name
        2048,             // Stack size
        NULL,             // Parameters
        5,                // Priority
        NULL              // Task handle
    );
}