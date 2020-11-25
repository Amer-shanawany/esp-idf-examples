#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

SemaphoreHandle_t xSemaphore = NULL;


TaskHandle_t taskHandle1 = NULL;
TaskHandle_t taskHandle2 = NULL;


void task1(void *arg)
{
    
    
    while(1)
    {   
        printf("Semaphore is sent!\t[%d]\n", xTaskGetTickCount());
        xSemaphoreGive(xSemaphore);
        vTaskDelay(pdMS_TO_TICKS(1000)); // 2 Sec.
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
   
    xSemaphore = xSemaphoreCreateBinary();
    xTaskCreate(task1, "task1", 4096, NULL, 10, &taskHandle1);
    xTaskCreatePinnedToCore(task2, "task2", 4096, NULL, 10, &taskHandle2, 1);

}
