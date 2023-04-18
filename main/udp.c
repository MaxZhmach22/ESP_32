#include "udp.h"
//-------------------------------------------------------------
static const char *TAG = "udp";
//-------------------------------------------------------------
void udp_task(void *pvParameters)
{
  TickType_t xLastWakeTime;
  int sockfd;
  struct sockaddr_in servaddr, cliaddr;
  ESP_LOGI(TAG, "Create socket...\n");
  if ( (sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) < 0 ) {
    ESP_LOGE(TAG, "socket not created\n");
    vTaskDelete(NULL);
  }
  memset(&servaddr, 0, sizeof(servaddr));
  memset(&cliaddr, 0, sizeof(cliaddr));
  cliaddr.sin_family    = AF_INET; // IPv4
  cliaddr.sin_addr.s_addr = INADDR_ANY;
  cliaddr.sin_port = htons(CONFIG_CLIENT_PORT);
  if (bind(sockfd, (const struct sockaddr *)&cliaddr,  sizeof(struct sockaddr_in)) < 0 )
  {
    ESP_LOGE(TAG, "socket not binded\n");
    vTaskDelete(NULL);
  }
  ESP_LOGI(TAG, "socket was binded\n");
  servaddr.sin_family    = AF_INET; // IPv4
  servaddr.sin_addr.s_addr = inet_addr(CONFIG_SERVER_IP);
  servaddr.sin_port = htons(CONFIG_SERVER_PORT);
  xLastWakeTime = xTaskGetTickCount();
  TickType_t delay = pdMS_TO_TICKS(1000);
  char message[100];
  for(short i=0;;i++)
  {
	ESP_LOGI(TAG, "Current value is: %d", i);
    sendto(sockfd, &i, 2,  0, (struct sockaddr*) &servaddr,  sizeof(servaddr));
    vTaskDelay(delay);
  }
  shutdown(sockfd, 0);
  close(sockfd);
  vTaskDelete(NULL);
}
//-------------------------------------------------------------
