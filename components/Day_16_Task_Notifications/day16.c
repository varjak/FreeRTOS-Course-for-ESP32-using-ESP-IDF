#include "./main/Day_16_Task_Notifications/day16.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "freertos/task.h"

#define BUTTON_GPIO 4
#define TAG "DAY16"

static TaskHandle_t button_task_handle = NULL;

static void IRAM_ATTR button_isr_handler(void *arg) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(button_task_handle, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
}

static void notify_task(void *pvParameter) {
    (void)pvParameter;
    for (;;) {
        xTaskNotify(button_task_handle, 0, eIncrement);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

static void button_task(void *pvParameter) {
    (void)pvParameter;
    int counter = 0;
    for (;;) {
        (void)ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        counter++;
        ESP_LOGI(TAG, "Button pressed %d times! Task unblocked by ISR", counter);
    }
}

void day16_run(void) {
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_NEGEDGE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << BUTTON_GPIO),
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE
    };
    gpio_config(&io_conf);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_GPIO, button_isr_handler, NULL);

    xTaskCreate(
        button_task,         // Task function
        "ButtonTask",        // Name for identification
        2048,                // Stack size in bytes
        NULL,                // Parameter (unused)
        10,                  // Priority (higher than idle/default)
        &button_task_handle  // Out: task handle for ISR notifications
    );

    xTaskCreate(
    notify_task,         // Task function
    "NotifyTask",        // Name for identification
    2048,                // Stack size in bytes
    NULL,                // Parameter (unused)
    10,                  // Priority (higher than idle/default)
    NULL  // Out: task handle for ISR notifications
);
}
