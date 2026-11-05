#include "./main/Day_7_Blinking_Two_LEDs_with_Two_Tasks/day7.h"

#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#define LED_1 GPIO_NUM_22
#define LED_2 GPIO_NUM_4
#define BLINK_DELAY_1 500
#define BLINK_DELAY_2 1000


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

static inline void toggle_led(const gpio_num_t pin) {
    gpio_set_level(pin, !gpio_get_level(pin));
}

static uint32_t measure_drift( const TickType_t start_ticks, const int counter, const int delay) {
    const uint32_t ideal = counter * delay;
    const uint32_t real = (xTaskGetTickCount() - start_ticks) * portTICK_PERIOD_MS;
    return real - ideal;
}

static void blink_delay(void *pvParameters) {
    int counter = 0;
    const TickType_t start_ticks = xTaskGetTickCount();
    while (1) {
        toggle_led(LED_1);
        vTaskDelay(pdMS_TO_TICKS(BLINK_DELAY_1));
        counter++;
        printf("Blink delay drift: %" PRIu32 "\n", measure_drift(start_ticks, counter, BLINK_DELAY_1));
    }
}

static void blink_delay_until(void *pvParameters) {
    int counter = 0;
    const TickType_t start_ticks = xTaskGetTickCount();
    while (1) {
        TickType_t now_tick = xTaskGetTickCount();
        toggle_led(LED_2);
        vTaskDelayUntil(&now_tick, pdMS_TO_TICKS(BLINK_DELAY_2));
        counter++;
        printf("Blink delay until drift: %" PRIu32 "\n", measure_drift(start_ticks, counter, BLINK_DELAY_2));
    }
}

void day7_run() {
    configure_led(LED_1);
    configure_led(LED_2);
    xTaskCreate(blink_delay, "Blink delay", 2048, NULL, 5, NULL);
    xTaskCreate(blink_delay_until, "Blink delay until", 2048, NULL, 5, NULL);
}
