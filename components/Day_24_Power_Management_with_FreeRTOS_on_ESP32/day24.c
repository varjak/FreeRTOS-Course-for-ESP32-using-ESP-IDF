#include "./main/Day_24_Power_Management_with_FreeRTOS_on_ESP32/day24.h"

#include "esp_log.h"
#include "esp_sleep.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"

#define TAG "DAY24"
#define SENSOR_TASK_STACK   (2048)
#define SENSOR_TASK_PRIO    (5)
#define SENSOR_PERIOD_MS    (2000)
#define WAKE_PERIOD_US      (5000000ULL)  // 5 seconds
#define BUT GPIO_NUM_4

// In menuconfig, I had to enable:
// Component config > Power Management > Support for power management (CONFIG_PM_ENABLE)
// Component config > FreeRTOS > Kernel > configUSE_TICKLESS_IDLE

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

static void sensor_task(void *pvParameter) {
    (void)pvParameter;
    while (1) {
        ESP_LOGI(TAG, "Reading sensor...");
        vTaskDelay(pdMS_TO_TICKS(SENSOR_PERIOD_MS));
    }
}

void day24_run() {
    configure_button(BUT);
    esp_sleep_enable_timer_wakeup(WAKE_PERIOD_US);
    esp_sleep_enable_ext0_wakeup(BUT, 0);
    xTaskCreate(sensor_task, "SensorTask", SENSOR_TASK_STACK, NULL, SENSOR_TASK_PRIO, NULL);
    esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
    switch (cause) {
        case ESP_SLEEP_WAKEUP_TIMER:
            ESP_LOGI(TAG, "Woke up: RTC timer");
            break;
        case ESP_SLEEP_WAKEUP_EXT0:
            ESP_LOGI(TAG, "Woke up: button");
            break;
        case ESP_SLEEP_WAKEUP_UNDEFINED:
            ESP_LOGW(TAG, "Woke up: cause undefined");
            break;
        default:
            ESP_LOGI(TAG, "Woke up: cause=%d", (int)cause);
            break;
    }
    ESP_LOGI(TAG, "Entering Deep Sleep for ~%llu us", (unsigned long long)WAKE_PERIOD_US);
    esp_deep_sleep(WAKE_PERIOD_US);
}
