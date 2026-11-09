#include "./main/Day_20_Interrupt_Management_in_FreeRTOS/day20.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <stdio.h>

#define TAG "DAY20"
#define BUT_1 GPIO_NUM_4
#define QUEUE_LENGTH 5
#define DEBOUNCE_DUR 100

static QueueHandle_t queue;

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
    const uint32_t button = (uint32_t) arg;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xQueueSendFromISR(queue, &button, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
}

static void debounce(const QueueHandle_t q, const uint32_t ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));  // ignore bounces
    xQueueReset(q);             // drop events queued meanwhile
}

static void task_receive(void *arg) {
    (void) arg;
    int message;
    while (1) {
        xQueueReceive(queue, &message, portMAX_DELAY);
        printf("Received a button press from this button: %d\n", message);
        debounce(queue, DEBOUNCE_DUR);
    }
}

void day20_run() {
    configure_button(BUT_1);
    queue = xQueueCreate(QUEUE_LENGTH, sizeof(int));
    xTaskCreate(task_receive, "Task", 2048, NULL, 5, NULL);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUT_1, button_isr_handler, (void *)BUT_1);
}