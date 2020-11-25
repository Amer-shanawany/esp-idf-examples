#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

TaskHandle_t myTaskhandle1 = NULL;
TaskHandle_t myTaskhandle2 = NULL;

void task1(void *arg)
{
    for(int i = 0; i<10; i++)
    {
        printf("Esp will restart after task1 finish, in [%d] \n", i);
        vTaskDelay(1000 / portTICK_RATE_MS);
        // If a task is finished ESP will resart itself
    }
}


void task2(void *arg)
{
    while(1)
    {
        printf("hello from task2 \n");
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

void app_main(void)
{
  xTaskCreate(task1 ,"task1", 4096, NULL, 10, &myTaskhandle1);// TODO: Create a task, Get stack size and then change the value
  xTaskCreatePinnedToCore(task2 ,"task1", 4096, NULL, 10, &myTaskhandle2, 1);
}
