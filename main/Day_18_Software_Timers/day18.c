#include "./main/Day_18_Software_Timers/day18.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"

#define TAG "DAY18"
#define TIMER_1_PERIOD 5000
#define TIMER_2_PERIOD 500
#define TIMER_2_N 9
#define LED_1 GPIO_NUM_22

static int counter = 0;

static void configure_led(const gpio_num_t pin) {
    gpio_set_level(pin, 1);
    const gpio_config_t io = {
        .pin_bit_mask = (1ULL << pin),
        .mode = GPIO_MODE_INPUT_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io);

}

static inline void toggle_led(const gpio_num_t pin) {
    gpio_set_level(pin, !gpio_get_level(pin));
}

void pxCallbackFunction(TimerHandle_t timer) {
    toggle_led(LED_1);
    int * c = (int *) pvTimerGetTimerID(timer);
    if (c != NULL) {
        (*c)++;
        ESP_LOGI(TAG, "Periodic timer tick %d", *c);
        if (*c >= TIMER_2_N) {
            ESP_LOGI(TAG, "Stopping periodic timer after %d ticks", TIMER_2_N);
            xTimerStop(timer, 0);
        }
    } else {
        ESP_LOGI(TAG, "One-shot timer tick");
    }
}


void day18_run() {
    configure_led(LED_1);
    TimerHandle_t timer_1 = xTimerCreate("Timer", pdMS_TO_TICKS(TIMER_1_PERIOD), pdFALSE, NULL, pxCallbackFunction);
    TimerHandle_t timer_2 = xTimerCreate("Timer", pdMS_TO_TICKS(TIMER_2_PERIOD), pdTRUE, &counter, pxCallbackFunction);
    xTimerStart(timer_1, 0);
    xTimerStart(timer_2, 0);
}