#include "udp.h"
//-------------------------------------------------------------
static const char *TAG = "udp";
static const char *RTAG = "recv_udp";
//-------------------------------------------------------------
static void recv_task(void *pvParameters)
{
	ESP_LOGI(RTAG, "Start receiving...\n");
	char buf[10] = {};
	int *sock = (int*) pvParameters;
	char str1[10];
	TickType_t delay = pdMS_TO_TICKS(500);
	for(short i=0;;i++)
	{
		ESP_LOGI(RTAG, "Update...\n");
		recv(*sock, buf, sizeof(buf), 0);
		snprintf(str1, sizeof(str1), "%6d", *(short*)buf);
		printf("%s", str1);
		//if(strlen(str1) != 0){
		//	gpio_set_level(CONFIG_LED_GPIO_RECIEVE, 1);
		//	vTaskDelay(delay/2);
		//	gpio_set_level(CONFIG_LED_GPIO_RECIEVE, 0);
		//	vTaskDelay(delay/2);
		//}else{
		//	vTaskDelay(delay);
		//}
	}
}

void udp_task(void *pvParameters)
{
  TickType_t xLastWakeTime;
  TaskHandle_t xRecvTask = NULL;
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
  xTaskCreate(recv_task, "recv_task", 4096, (void*)&sockfd, 5, &xRecvTask);
  xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = 100 / portTICK_PERIOD_MS;
  TickType_t delay = pdMS_TO_TICKS(1000);
  char message[100];
  for(short i=0;;i++)
  {
	ESP_LOGI(TAG, "Current value is: %d", i);
    sendto(sockfd, &i, 2,  0, (struct sockaddr*) &servaddr,  sizeof(servaddr));
    vTaskDelayUntil(&xLastWakeTime, xFrequency)
  }
  shutdown(sockfd, 0);
  close(sockfd);
  vTaskDelete(xRecvTask);
  vTaskDelete(NULL);
}
//-------------------------------------------------------------
