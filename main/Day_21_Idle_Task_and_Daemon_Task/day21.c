#include "./main/Day_21_Idle_Task_and_Daemon_Task/day21.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_log.h"
#include "driver/gpio.h"

#define TAG "DAY21"
#define TIMER_1_PER 1000
#define TIMER_2_PER 3000
#define BLINK_DUR 1000
#define LED GPIO_NUM_22

TimerHandle_t timer1;
TimerHandle_t timer2;

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

// Toggle LED
static inline void toggle_led(const gpio_num_t pin) {
    gpio_set_level(pin, !gpio_get_level(pin));
}

// It seems the idle hook didn't work. Probably has to be configured in menuconfig. Tried this but didn't work:
// idf.py menuconfig: / configUSE_IDLE_HOOK > enable and  configUSE_Tick_HOOK > enable
// Printscreen is available for future debugging
void vApplicationIdleHook(void) {
    static int counter = 0;
    counter++;
    if (counter > 10) {
        printf(".");
        fflush(stdout);     // Ensure the dot is printed immediately
        counter = 0;
        toggle_led(LED);
    }
}

void periodic_callback(TimerHandle_t xTimer) {
    unsigned int period_ms = (unsigned int)pvTimerGetTimerID(xTimer);
    ESP_LOGI(TAG, "Daemon task: %lu ms timer fired!", period_ms);
}

void day21_run() {
    configure_led(LED);
    timer1 = xTimerCreate("Timer 1", pdMS_TO_TICKS(TIMER_1_PER), pdTRUE, (void *) TIMER_1_PER, periodic_callback);
    timer2 = xTimerCreate("Timer 2", pdMS_TO_TICKS(TIMER_2_PER), pdTRUE, (void *) TIMER_2_PER, periodic_callback);
    if (timer1 != NULL && timer2 != NULL) {
        xTimerStart(timer1, 0);
        xTimerStart(timer2, 0);
    }
}

// Problems:

// On Best practices: FreeRTOS ticks are not hardware? I thought it was a timer controler in ESP32 that issued tick interupts,
// that were handled by tick hooks in FreeRTOS. If not, how different are hardware timers and esp_timer then?

// Cannot make idle hooks work

