#include "./main/Day_17_Stream_Buffers_and_Message_Buffers/day17.h"

#include "driver/gpio.h"
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"

#define TAG "DAY17"
#define BUT_1 GPIO_NUM_4

static StreamBufferHandle_t stream_buf = NULL;
static MessageBufferHandle_t msg_buf = NULL;

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

static void producer_task(void *pvParameter) {
    const char *stream_data = "ABCDEF";
    const char *msg1 = "Hello";
    const char *msg2 = "World";
    (void)pvParameter;
    while (1) {
        xStreamBufferSend(stream_buf, stream_data, 6, portMAX_DELAY);
        xMessageBufferSend(msg_buf, msg1, strlen(msg1) + 1, portMAX_DELAY);
        xMessageBufferSend(msg_buf, msg2, strlen(msg2) + 1, portMAX_DELAY);
        printf("Producer: Sent two messages to message buffer\n");
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

static void IRAM_ATTR button_isr_handler(void *arg) {
    (void) arg;
    const char *stream_data = "ABCDEF";
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xStreamBufferSendFromISR(stream_buf, stream_data, 6, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
}

static void consumer_task(void *pvParameter) {
    (void)pvParameter;
    char rx_stream[20];
    char rx_msg[20];
    int counter = 0;
    while (1) {
        size_t received = xStreamBufferReceive(stream_buf, rx_stream, sizeof(rx_stream) - 1 /* leave room for '\0' */,pdMS_TO_TICKS(1000));
        if (received > 0) {
            rx_stream[received] = '\0';
            counter++;
            printf("Consumer: Stream buffer received message %d: %s\n", counter,  rx_stream);
        }
        // received = xMessageBufferReceive(msg_buf, rx_msg, sizeof(rx_msg), pdMS_TO_TICKS(1000));
        // if (received > 0) {
        //     printf("Consumer: Message buffer received: %s\n", rx_msg);
        // }
    }
}

void day17_run(void) {
    configure_button(BUT_1);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUT_1, button_isr_handler, NULL);

    stream_buf = xStreamBufferCreate(50, 1);  // Changing size to a size smaller than message caps message
    msg_buf = xMessageBufferCreate(100);
    if (msg_buf == NULL) {
        printf("Error: Failed to create message buffer\n");
        vTaskSuspend(NULL);
    }
    // xTaskCreate(producer_task, "Producer", 2048, NULL, 5, NULL);
    xTaskCreate(consumer_task, "Consumer", 2048, NULL, 5, NULL);
}
