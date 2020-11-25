#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"


void app_main(void)
{
    // Initilize NVS
    esp_err_t err = nvs_flash_init();

    if(err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        printf("Partitions were truncated, should be earesed");
        
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    
    // Open NVS
    printf("\n");
    nvs_handle_t my_nvs_handle;
    printf("Opening Non-Volatile Storage handle.. \n");
    // assiging spacename as storage
    err = nvs_open("storage", NVS_READWRITE, &my_nvs_handle);

    if(err  != ESP_OK)
    {
        printf("Error %s while opening NVS handle\n", esp_err_to_name(err));
    } else {
        printf("Done.\n");

        // Read NVS
        printf("Reading restart counter from NVS ...");
        int32_t restart_counter = 0; // value is 0 if not set in NVS
        err = nvs_get_i32(my_nvs_handle, "restart_counter", &restart_counter); // Read value of KEY=restart_counter
        switch(err)
        {
                case ESP_OK:
                    printf("Done reading value\n");
                    printf("Restart counter = %d\n", restart_counter);
                    break;
                case ESP_ERR_NVS_NOT_FOUND:
                    printf("The value is not initialized yet\n");
                    break;
                default:
                    printf("Error (%s) reading nvs!\n", esp_err_to_name(err));
        }

        // Write NVS
        printf("Updating restart counter in NVS\n");
        restart_counter++;
        err = nvs_set_i32(my_nvs_handle, "restart_counter", restart_counter);
        printf((err != ESP_OK) ? "Failed to write to nvs\n" : "Done writing to nvs\n");

        printf("Commiting values to nvs");
        err = nvs_commit(my_nvs_handle);
        printf((err != ESP_OK) ? "Failed to commit to nvs\n" : "Done commiting to nvs\n");

        // Close
        nvs_close(my_nvs_handle);

    }

    printf("\n");

    // Restart 
    for(int i = 0; i < 10 ; i++)
    {
       printf("Restarting in [%d] seconds\n", i);
       vTaskDelay(1000 / portTICK_PERIOD_MS); 
    }
    printf("restarting now.\n");
    fflush(stdout);
    esp_restart();
}
