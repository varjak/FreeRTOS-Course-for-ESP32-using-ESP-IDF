#include "./main/Day_10_Binary_Semaphores/day10.h"
#include "freertos/FreeRTOS.h"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#define BUT_1 GPIO_NUM_4
#define TAG "DAY10"

static SemaphoreHandle_t button_semaphore;

static void configure_button(const gpio_num_t pin) {
    const gpio_config_t io = {
        .pin_bit_mask = (1ULL << pin),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE
    };
    gpio_config(&io);
}

static void IRAM_ATTR button_isr_handler(void *arg) {
    (void) arg;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(button_semaphore, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
}

static void button_task(void *pvParameter) {
    const char *name = pcTaskGetName(NULL);
    while (1) {
        if (xSemaphoreTake(button_semaphore, portMAX_DELAY)) {
            ESP_LOGI(TAG, "Task %s noticed button on pin %" PRIu32 " was pressed!",name, BUT_1);
        }
    }
}

void day10_run() {
    configure_button(BUT_1);
    button_semaphore = xSemaphoreCreateBinary();
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUT_1, button_isr_handler, (void *) BUT_1);
    xTaskCreate(button_task, "Button task 1", 2048, NULL, 10, NULL);
    xTaskCreate(button_task, "Button task 2", 2048, NULL, 10, NULL);
}
