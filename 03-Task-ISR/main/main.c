#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define ESP_FLAG_DEFAULT 0 
#define BUTTON_PIN 22
#define LED_PIN 23

TaskHandle_t ISR = NULL;
void IRAM_ATTR button_isr_handler(void *arg)
{
    xTaskResumeFromISR(ISR);
}

void button_task(void *arg)
{
    bool led_status = false;

    while(1)
    {
        vTaskSuspend(NULL);
        led_status = !led_status;
        gpio_set_level(LED_PIN, led_status);
        printf("Button pressed!\t LED Status is %s", led_status ? "ON\n" : "OFF\n");
    }
}
void app_main(void)
{
    // init gpio buttons
    gpio_pad_select_gpio(BUTTON_PIN);
    gpio_pad_select_gpio(LED_PIN);

    // set PIN direction
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    // Enable interrupt on failing edge 
    gpio_set_intr_type(BUTTON_PIN, GPIO_INTR_NEGEDGE);

    // Install the driver's GPIO ISR handler 
    // Insatll ISR service with default configuration
    gpio_install_isr_service(ESP_FLAG_DEFAULT);

    // Attach the interrupt service routine
    gpio_isr_handler_add(BUTTON_PIN, button_isr_handler, NULL);

    // Create and start a task
    xTaskCreate(button_task, "button_task", 4096, NULL, 10, &ISR);


}
