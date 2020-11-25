#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"

#define BUTTON_PIN 22
#define LED_PIN 23
#define ESP_INTR_FLAG 0


SemaphoreHandle_t xSemaphore = NULL;

TaskHandle_t taskHandle1 = NULL;
TaskHandle_t taskHandle2 = NULL;
volatile bool led_status = false;

void IRAM_ATTR button_isr_handler(void* arg)
{
    led_status = !led_status;
    xSemaphoreGiveFromISR(xSemaphore, NULL);
    gpio_set_level(LED_PIN, led_status);

}

void task1(void *arg)
{
    
    
    while(1)
    {   
        printf("Semaphore is sent!\t[%d]\n", xTaskGetTickCount());
        xSemaphoreGive(xSemaphore);
        vTaskDelay(pdMS_TO_TICKS(5000)); // 5 Sec.
    }
}

void task2(void *arg)
{
    while(1)
    {
        if(xSemaphoreTake(xSemaphore, portMAX_DELAY)) // This will wait until it gets the Semaphore (FLAG)
        {
            printf("Semaphore is received!\t[%d]\n", xTaskGetTickCount()); // returns the elapsed time since the begining
            vTaskDelay(pdMS_TO_TICKS(1000));
        } // 0.5 Sec.
    }
}

void app_main(void)
{
    gpio_pad_select_gpio(BUTTON_PIN);
    gpio_pad_select_gpio(LED_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_intr_type(BUTTON_PIN, GPIO_INTR_NEGEDGE);
    gpio_install_isr_service(ESP_INTR_FLAG);
    gpio_isr_handler_add(BUTTON_PIN, button_isr_handler, NULL);

    xSemaphore = xSemaphoreCreateBinary();
    xTaskCreate(task1, "task1", 4096, NULL, 10, &taskHandle1);
    xTaskCreatePinnedToCore(task2, "task2", 4096, NULL, 10, &taskHandle2, 1);

}
