#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#define ESP_INTR_FLAG 0
#define BUTTON_PIN 22
#define LED_PIN  23

TaskHandle_t taskHandle1 = NULL;
TaskHandle_t taskHandle2 = NULL;

QueueHandle_t queue1;


TaskHandle_t ISR = NULL;

void IRAM_ATTR button_isr_handler(void *arg)
{
    char txBuffer[50];
    sprintf(txBuffer, "Hello from ISR!");
    xQueueSendFromISR(queue1, &txBuffer, NULL);
}

void task1(void *arg)
{
    char txBuffer[50];

    queue1 = xQueueCreate(5, sizeof(txBuffer));
    if(queue1 == 0 )
    {
        printf("Failed to create queue1 = %p\n", queue1);
    }
  
    // sprint concatinate the string
    sprintf(txBuffer, "Sending text from Queue1 1\n");
    xQueueSend(queue1, (void*)txBuffer, (TickType_t) 0); //second is pointer to data, third is blocking time 

    sprintf(txBuffer, "Sending text from Queue1 2\n");
    xQueueSend(queue1, (void*)txBuffer, (TickType_t) 0); //second is pointer to data, third is blocking time 

    sprintf(txBuffer, "Sending text from Queue1 3\n");
    xQueueSendToFront(queue1, (void*)txBuffer, (TickType_t) 0); //second is pointer to data, third is blocking time 

    
    while(1)
    {   
        printf("data waiting to be read: %d available spaces: %d\n", uxQueueMessagesWaiting(queue1), uxQueueSpacesAvailable(queue1));
        vTaskDelay(pdMS_TO_TICKS(1000)); // 2 Sec.
    }
}

void task2(void *arg)
{
    char rxBuffer[50];

    
    while(1)
    {
        xQueuePeek(queue1, &(rxBuffer), (TickType_t) 5);// Will not delete msg from Queue
        if(xQueueReceive(queue1, (void*)rxBuffer, (TickType_t) 5)) // Will delete msg from Buffer
        {
            printf("Got data from queue buffer! ===\t%s\n",rxBuffer);

        }


        vTaskDelay(pdMS_TO_TICKS(1000)); // 0.5 Sec.
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
    
    xTaskCreate(task1, "task1", 4096, NULL, 10, &taskHandle1);
    xTaskCreatePinnedToCore(task2, "task2", 4096, NULL, 10, &taskHandle2, 1);


}
