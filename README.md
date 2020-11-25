## FreeRTOS

### Tasks [ESP32 - reference]([https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html?highlight=freertos])


Choosing core
```
xTaskCreatePinnedToCore(TaskFunction_t pvTaskCode, const char *constpcName, const uint32_t usStackDepth, void *constpvParameters, UBaseType_t uxPriority, TaskHandle_t *constpvCreatedTask, const BaseType_t xCoreID)
```


Without
```
xTaskCreate(TaskFunction_t pvTaskCode, const char *constpcName, const uint32_t usStackDepth, void *constpvParameters, UBaseType_t uxPriority, TaskHandle_t *constpvCreatedTask)
```

Delete a task
```
vTaskDelete(TaskHandle_txTaskToDelete)
```

Delay Task
```
void vTaskDelay(const TickType_t xTicksToDelay)
```

Task priority
```
uxTaskPriorityGet(constTaskHandle_txTask)
```

Set task priority
```
uxTaskPriorityGet(constTaskHandle_txTask)
```

Suspend a Task
```
uxTaskPriorityGet(constTaskHandle_txTask)
```

Resumes a suspended task.

```
vTaskResume(TaskHandle_txTaskToResume)
```


## Queue API

Send/Recieve data

## Semaphore API

Gives Notice to other Task

Creates a new binary semaphore instance
```
xSemaphoreCreateBinary()
```

ecursively obtain, or ‘take’, a mutex type semaphore
```
xSemaphoreTakeRecursive(xMutex, xBlockTime)
```

release a semaphore
```
xSemaphoreGive(xSemaphore)
```

release for a period of time
```
xSemaphoreGiveRecursive(xMutex)
```

Release using an interrupt
```
xSemaphoreGiveFromISR(xSemaphore, pxHigherPriorityTaskWoken)
```

## Event Group

make an event group 
```
xEventGroupCreate(void)
```
