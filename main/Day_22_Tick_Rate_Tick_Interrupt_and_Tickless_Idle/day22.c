#include "./main/Day_22_Tick_Rate_Tick_Interrupt_and_Tickless_Idle/day22.h"
#include "freertos/FreeRTOS.h"

#define TAG "DAY22"
#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define LED_GPIO GPIO_NUM_22
#define TICK_HOOK_SEND_PER_1 100
#define TICK_HOOK_SEND_PER_2 1000

static TaskHandle_t s_tick_task = NULL;
static volatile uint32_t s_ms_since_boot = 0;

// Configure GPIO as LED
static void configure_led(const gpio_num_t pin) {
    const gpio_config_t io = {
        .pin_bit_mask = (1ULL << pin),
        .mode = GPIO_MODE_INPUT_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io);
    gpio_set_level(pin, 1);
}

void IRAM_ATTR vApplicationTickHook(void)
{
    // Accumulate milliseconds using the known tick period
    s_ms_since_boot += portTICK_PERIOD_MS;

    // Notify once every TICK_HOOK_SEND_PER_1 ms
    static uint32_t acc_ms = 0;
    acc_ms += portTICK_PERIOD_MS;

    if (acc_ms >= TICK_HOOK_SEND_PER_1) {
        acc_ms -= TICK_HOOK_SEND_PER_1;
        BaseType_t higher_woken = pdFALSE;
        if (s_tick_task) {
            // Wake the task (one count per half-second)
            vTaskNotifyGiveFromISR(s_tick_task, &higher_woken);
        }
        if (higher_woken) {
            portYIELD_FROM_ISR();
        }
    }

    // Notify once every TICK_HOOK_SEND_PER_2 ms
    if (acc_ms >= TICK_HOOK_SEND_PER_2) {
        acc_ms -= TICK_HOOK_SEND_PER_2;
        BaseType_t higher_woken = pdFALSE;
        if (s_tick_task) {
            // Wake the task (one count per half-second)
            vTaskNotifyGiveFromISR(s_tick_task, &higher_woken);
        }
        if (higher_woken) {
            portYIELD_FROM_ISR();
        }
    }
}

static void tick_consumer_task(void *arg)
{
    bool led_on = false;

    for (;;) {
        // Block until the tick hook gives us a notification (every 500 ms)
        ulTaskNotifyTake(/*clearOnExit=*/pdTRUE, /*wait=*/portMAX_DELAY);

        // Safe to do work here (task context)
        ESP_LOGI(TAG, "Half-second tick: ~%" PRIu32 " ms since boot", s_ms_since_boot);

        led_on = !led_on;
        gpio_set_level(LED_GPIO, led_on);
    }
}


void day22_run() {

    configure_led(LED_GPIO);
    // Create the task to be notified by the tick hook
    xTaskCreate(tick_consumer_task, "TickConsumer", 2048, NULL, 6, &s_tick_task);

    ESP_LOGI(TAG, "Tick hook demo started. TICK_RATE_HZ=%d, tick=%d ms",
             configTICK_RATE_HZ, (int)portTICK_PERIOD_MS);
}

// vApplicationTickHook is not being triggered. See problems in day21.c