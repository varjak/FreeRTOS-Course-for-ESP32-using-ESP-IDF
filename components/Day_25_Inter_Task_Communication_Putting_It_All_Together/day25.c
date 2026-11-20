#include "./include/day25.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"

#define TAG "DAY25"
#define BIT_SENSOR_READY    (1 << 0)
#define BIT_PROCESSOR_READY (1 << 1)
#define BUT GPIO_NUM_4
#define TICK_HOOK_PERIOD 2000
// Handles
QueueHandle_t sensor_queue;
SemaphoreHandle_t sample_sem;
SemaphoreHandle_t uart_mutex;
EventGroupHandle_t system_event_group;
static TaskHandle_t button_task_handle = NULL;
static volatile uint32_t s_ms_since_boot = 0;
static TaskHandle_t tick_watchdog_h = NULL;

// Configure GPIO as button
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

static void debounce(const TaskHandle_t h, const uint32_t ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));  // ignore bounces
    ulTaskNotifyValueClear(h, 0xFFFFFFFF);            // drop events queued meanwhile
    gpio_set_intr_type(BUT, GPIO_INTR_NEGEDGE);
}

void sensor_task(void *pvParameters) {
    int temp = 25;
    while (1) {
        temp = 20 + rand() % 10; // fake data
        xQueueSend(sensor_queue, &temp, portMAX_DELAY);
        xSemaphoreGive(sample_sem); // signal new sample
        xEventGroupSetBits(system_event_group, BIT_SENSOR_READY);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void processor_task(void *pvParameters) {
    int value;
    int sum = 0, count = 0;

    while (1) {
        if (xSemaphoreTake(sample_sem, portMAX_DELAY)) {
            if (xQueueReceive(sensor_queue, &value, 0)) {
                sum += value;
                count++;
                if (count == 5) {
                    int avg = sum / count;
                    sum = 0;
                    count = 0;
                    ESP_LOGI(TAG, "Computed avg temp: %d", avg);
                    xEventGroupSetBits(system_event_group, BIT_PROCESSOR_READY);
                }
            }
        }
    }
}

void logger_task(void *pvParameters) {
    while (1) {
        EventBits_t bits = xEventGroupWaitBits(
            system_event_group,
            BIT_SENSOR_READY | BIT_PROCESSOR_READY,
            pdFALSE, // don’t clear bits
            pdTRUE,  // wait for ALL
            portMAX_DELAY
        );

        if ((bits & (BIT_SENSOR_READY | BIT_PROCESSOR_READY)) ==
            (BIT_SENSOR_READY | BIT_PROCESSOR_READY)) {
            if (xSemaphoreTake(uart_mutex, portMAX_DELAY)) {
                printf("Logger: Both Sensor & Processor are active\n");
                xSemaphoreGive(uart_mutex);
            }
            vTaskDelay(pdMS_TO_TICKS(2000));
            }
    }
}

void button_log(void *pvParameters) {
    while (1) {
        if (xTaskNotifyWait(0x00,0xFFFFFFFF,NULL,portMAX_DELAY) == pdTRUE) {
            if (xSemaphoreTake(uart_mutex, portMAX_DELAY) == pdTRUE) {
                printf("Button Logger: The button was pressed!\n");
                xSemaphoreGive(uart_mutex);
                debounce(button_task_handle, 200);
            }
        }
    }
}

static void IRAM_ATTR button_isr_handler(void *arg) {
    gpio_set_intr_type(BUT, GPIO_INTR_DISABLE);
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(button_task_handle, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
}

// Tick hook
void IRAM_ATTR vApplicationTickHook(void) {
    // Accumulate milliseconds using the known tick period
    s_ms_since_boot += portTICK_PERIOD_MS;

    // Notify once every TICK_HOOK_SEND_PER_1 ms
    static uint32_t acc_ms = 0;
    acc_ms += portTICK_PERIOD_MS;

    if (acc_ms >= TICK_HOOK_PERIOD) {
        acc_ms -= TICK_HOOK_PERIOD;
        BaseType_t higher_woken = pdFALSE;
        if (tick_watchdog_h) {
            // Wake the task (one count per half-second)
            vTaskNotifyGiveFromISR(tick_watchdog_h, &higher_woken);
        }
        if (higher_woken) {
            portYIELD_FROM_ISR();
        }
    }
}

void tick_watchdog(void *pvParameters) {
    while (1) {
        if (xTaskNotifyWait(0x00,0xFFFFFFFF,NULL,portMAX_DELAY) == pdTRUE) {
            if (xSemaphoreTake(uart_mutex, portMAX_DELAY) == pdTRUE) {
                printf("Tick watchdog: Notification received!\n");
                xSemaphoreGive(uart_mutex);
            }
        }
    }
}


void day25_run() {

    configure_button(BUT);

    // Create a queue for sensor data
    sensor_queue = xQueueCreate(10, sizeof(int));

    // Counting semaphore for samples
    sample_sem = xSemaphoreCreateCounting(10, 0);

    // Mutex for UART access
    uart_mutex = xSemaphoreCreateMutex();

    // Event group for system state
    system_event_group = xEventGroupCreate();

    // Create tasks
    xTaskCreate(sensor_task, "SensorTask", 2048, NULL, 5, NULL);
    xTaskCreate(processor_task, "ProcessorTask", 2048, NULL, 6, NULL);
    xTaskCreate(logger_task, "LoggerTask", 2048, NULL, 4, NULL);
    xTaskCreate(button_log, "ButtonLoggerTask", 2048, NULL, 4, &button_task_handle);
    xTaskCreate(tick_watchdog, "TickWatchdog", 2048, NULL, 6, &tick_watchdog_h);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUT, button_isr_handler, NULL);

    ESP_LOGI(TAG, "Day 25 system initialized");
}