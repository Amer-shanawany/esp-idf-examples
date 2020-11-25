#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#define ESP_INTR_FLAG_DEFAULT 0
#define BUTTON_PIN 22
#define LED_PIN 23
TaskHandle_t taskHandle1 = NULL;
TaskHandle_t taskHandle2 = NULL;

QueueHandle_t queue1;

void IRAM_ATTR button_isr_handler()
{

}

TaskHandle_t ISR = NULL;
void IRAM_ATTR button_isr_handler(void *arg)
{
    xTaskResumeFromISR(ISR);
}

void task1(void *arg)
{
    char txBuffer[50];

    //queue1 = xQueueCreate(5, sizeof(txBuffer));
    queue1 = xQueueCreate(1, sizeof(txBuffer)); // make size small so it will fail
    if(queue1 == 0 )
    {
        printf("Failed to create queue1 = %p\n", queue1);
    }
  
    // sprint concatinate the string
    sprintf(txBuffer, "Sending text from Queue1 1\n");
    if(xQueueSend(queue1, (void*)txBuffer, (TickType_t) 0) != 1) // Returns 1 onSuccessful & 0 onFailure
    {
        printf("Couldn't send this message:\t%s\n",txBuffer);
    }

    xQueueSendToFront(queue1, (void*)txBuffer, (TickType_t) 0); //second is pointer to data, third is blocking time 

    xQueueSend(queue1, (void*)txBuffer, (TickType_t) 0); //second is pointer to data, third is blocking time 
    sprintf(txBuffer, "Sending text from Queue1 3\n");

    
    sprintf(txBuffer, "Overwriting text from Queue1 4\n");
    if(xQueueOverwrite(queue1, (void*)txBuffer) != 1)
    {
       printf("Couldn't Overwrite text from Queue1 4\t%s\n",txBuffer);
    
    }
    
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
    
    xTaskCreate(task1, "task1", 4096, NULL, 10, &taskHandle1);
    xTaskCreatePinnedToCore(task2, "task2", 4096, NULL, 10, &taskHandle2, 1);


}
