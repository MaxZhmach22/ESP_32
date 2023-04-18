#include "main.h"
//-------------------------------------------------------------
static const char *TAG = "main";
//-------------------------------------------------------------
void app_main(void)
{
  wifi_ap_record_t info;
  gpio_reset_pin(CONFIG_LED_GPIO);
  gpio_set_direction(CONFIG_LED_GPIO, GPIO_MODE_OUTPUT);
  gpio_set_level(CONFIG_LED_GPIO, 0);
  //Initialize NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ret = nvs_flash_erase();
    ESP_LOGI(TAG, "nvs_flash_erase: 0x%04x", ret);
    ret = nvs_flash_init();
    ESP_LOGI(TAG, "nvs_flash_init: 0x%04x", ret);
  }
  ESP_LOGI(TAG, "nvs_flash_init: 0x%04x", ret);
  wifi_init_sta();
  xTaskCreate(udp_task, "udp_task", 4096, NULL, 5, NULL);
  while (true) {
    //ESP_LOGI(TAG, "Heap free size:%u", xPortGetFreeHeapSize());
    ret = esp_wifi_sta_get_ap_info(&info);
    ESP_LOGI(TAG, "esp_wifi_sta_get_ap_info: 0x%04x", ret);
    if(ret==0) ESP_LOGI(TAG, "SSID: %s", info.ssid);
    else{
      gpio_set_level(CONFIG_LED_GPIO, 0);
      wifi_init_sta();
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
