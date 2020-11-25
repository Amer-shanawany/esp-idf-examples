#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"

static char WIFI_SSID[32] = "myWifiName";
static char WIFI_PASS[64] = "myStrongPassword";

static void get_wifi_credentials(void)

{
    esp_err_t err;
    nvs_handle_t nvs_wifi_handle;
    char wifi_ssid[33] = {0};
    char wifi_passwd[65] = {0}; 

    size_t len;

    err = nvs_open("wificonfig", NVS_READWRITE, &nvs_wifi_handle);
    if(err != ESP_OK)
    {
        printf("Error (%s) while opening NVS handle\n", esp_err_to_name(err));
    }
    else
    {
        printf("opening NVS handle Done\n");
        // Read
        printf("Get ssid and key from NVS ... \n");
        //get ssid
        len = sizeof(wifi_ssid);
        err = nvs_get_str(nvs_wifi_handle, "ssid", wifi_ssid, &len);
        if(err==ESP_OK)
        {
            printf("Get ssid success!\n");
            printf("ssid=%s\n",wifi_ssid);
            printf("ssid_len=%d\n",len);
        }
        else
        {
           printf("get err =0x%x\n",err);
           printf("Get ssid fail!\n");
        }

        //get password
        len = sizeof(wifi_passwd);
        err = nvs_get_str(nvs_wifi_handle, "key", wifi_passwd, &len);     
         if(err==ESP_OK)
        {
            printf("Get key success!\n");
            printf("password=%s\n",wifi_passwd);
            printf("password_len=%d\n",len);
        }
        else
        {
           printf("get err =0x%x\n",err);
           printf("Get ssid fail!\n");
        }
        err = nvs_commit(nvs_wifi_handle);
        printf((err != ESP_OK) ? "nvs_commit Failed!\n" : " nvs_commit Done\n");
    }
    // Close
    nvs_close(nvs_wifi_handle);
}


static void set_wifi_credentials(void)
{
    esp_err_t err;
    nvs_handle_t nvs_wifi_handle;  
    
    err = nvs_open("wificonfig", NVS_READWRITE, &nvs_wifi_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } 
    else 
    {
        printf("opening NVS handle Done\n");
        printf("Set ssid and key from NVS ... \n");
        //write WIFI_SSID
        err=nvs_set_str(nvs_wifi_handle,"ssid",WIFI_SSID);
        if(err==ESP_OK)
            printf("set ssid success!\n");
        else
        {
            printf("set ssid fail!\n");
        }  
        //write WIFI_PASS
        err=nvs_set_str(nvs_wifi_handle,"key",WIFI_PASS);
        if(err==ESP_OK)
            printf("set password success!\n");
        else
        {
            printf("set password fail!\n");
        }   
        err = nvs_commit(nvs_wifi_handle);
        printf((err != ESP_OK) ? "nvs_commit Failed!\n" : "nvs_commit Done\n");
    }
    // Close
    nvs_close(nvs_wifi_handle);
    printf("--------------------------------------\n");
}

void app_main(void)
{

    printf("[APP] Startup..\n");
    printf("[APP] Free memory: %d bytes\n", esp_get_free_heap_size());
    printf("[APP] IDF version: %s\n", esp_get_idf_version());

    // Initilize NVS
    esp_err_t err = nvs_flash_init();

    if(err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        printf("Partitions were truncated, should be earesed");
        
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
        ESP_ERROR_CHECK( err );

    set_wifi_credentials();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    get_wifi_credentials();
    
}
