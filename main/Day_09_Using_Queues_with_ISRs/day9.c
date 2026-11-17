#include "./main/Day_9_Using_Queues_with_ISRs/day9.h"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#define BUT_1 GPIO_NUM_4
#define BUT_2 GPIO_NUM_5
#define TAG "DAY9"

static QueueHandle_t button_queue;

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
    uint32_t gpio_num = (uint32_t) arg;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xQueueSendFromISR(button_queue, &gpio_num, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
}

static void button_task(void *pvParameter) {
    uint32_t gpio_num;
    while (1) {
        if (xQueueReceive(button_queue, &gpio_num, portMAX_DELAY)) {
            ESP_LOGI(TAG, "The button on pin %" PRIu32 " was pressed!", gpio_num);
        }
    }
}

void day9_run() {
    configure_button(BUT_1);
    configure_button(BUT_2);
    button_queue = xQueueCreate(10, sizeof(uint32_t));
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUT_1, button_isr_handler, (void *) BUT_1);
    gpio_isr_handler_add(BUT_2, button_isr_handler, (void *) BUT_2);
    xTaskCreate(button_task, "Button task", 2048, NULL, 10, NULL);
}
