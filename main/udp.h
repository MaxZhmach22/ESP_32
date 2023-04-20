#ifndef MAIN_UDP_H_
#define MAIN_UDP_H_
//-------------------------------------------------------------
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>
#include <stdio.h>
//-------------------------------------------------------------
void udp_task(void *pvParameters);
//-------------------------------------------------------------
#endif /* MAIN_UDP_H_ */
