#include "udp.h"
#include "sdkconfig.h"
#include "driver/ledc.h"
//-------------------------------------------------------------
static const char *TAG = "udp";
static const char *RTAG = "recv_udp";
//-------------------------------------------------------------
#define MOTOR_PWM_FREQ 1000
#define MOTOR_PWM_RES  10
//-------------------------------------------------------------

static void recv_task(void *pvParameters)
{
	ESP_LOGI(RTAG, "Start receiving...\n");
	char buf[10] = {};
	int *sock = (int*) pvParameters;
	char str1[11];
	TickType_t delay = pdMS_TO_TICKS(500);
	//-engine
	 int motor_pwm_pin = CONFIG_MOTOR_PWM_PIN;
	 ledc_timer_config_t timer_conf = {
	         .speed_mode = LEDC_HIGH_SPEED_MODE,
	         .timer_num = LEDC_TIMER_0,
	         .duty_resolution = MOTOR_PWM_RES,
	         .freq_hz = MOTOR_PWM_FREQ
	     };
	     ledc_timer_config(&timer_conf);

	 ledc_channel_config_t channel_conf = {
	          .channel = LEDC_CHANNEL_0,
	          .timer_sel = LEDC_TIMER_0,
	          .gpio_num = motor_pwm_pin,
	          .duty = (1 << MOTOR_PWM_RES) - 1,
	          .speed_mode = LEDC_HIGH_SPEED_MODE,
	          .hpoint = 0
	     };
	     ledc_channel_config(&channel_conf);

	ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 0);
	ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
	for(short i=0;;i++)
	{
		recv(*sock, buf, sizeof(buf), 0);
		//snprintf(str1, sizeof(str1), "%6d", *(short*)buf);
		int length = sizeof(buf);
		memset(str1, 0, sizeof(str1));
		for(int i = 0; i < length; i++){
			str1[i] = (char)buf[i];
		}
		str1[length] = '\0';
		ESP_LOGI(RTAG, "Update...\n %s", str1);
		char led_on[] = "led_on";
		char led_off[] = "led_off";
		char *match_on = strstr(str1, led_on);
		char *match_off = strstr(str1, led_off);

		if(match_on != NULL){
			ESP_LOGI(RTAG, "LED ON");
			gpio_set_level(CONFIG_LED_GPIO_RECIEVE, 1);
		    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, (1 << MOTOR_PWM_RES) - 1);
		    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
		}
		else if(match_off != NULL){
			ESP_LOGI(RTAG, "LED OFF");
			gpio_set_level(CONFIG_LED_GPIO_RECIEVE, 0);
			ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 0);
			ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
		}else{
			vTaskDelay(delay);
		}
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
  const TickType_t xFrequency = 1000 / portTICK_PERIOD_MS;
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
