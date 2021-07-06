/*
   This code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_event.h"
#include "mqtt_client.h"
#include "driver/twai.h"

#include "mqtt.h"

static const char *TAG = "SUB";

static EventGroupHandle_t s_mqtt_event_group;

#define MQTT_CONNECTED_BIT BIT0

extern QueueHandle_t xQueue_mqtt_tx;
extern QueueHandle_t xQueue_mqtt_rx;

static QueueHandle_t xQueueSubscribe;

static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event)
{
	switch (event->event_id) {
		case MQTT_EVENT_CONNECTED:
			ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
			xEventGroupSetBits(s_mqtt_event_group, MQTT_CONNECTED_BIT);
			//esp_mqtt_client_subscribe(mqtt_client, CONFIG_SUB_TOPIC, 0);
			break;
		case MQTT_EVENT_DISCONNECTED:
			ESP_LOGW(TAG, "MQTT_EVENT_DISCONNECTED");
			xEventGroupClearBits(s_mqtt_event_group, MQTT_CONNECTED_BIT);
			break;
		case MQTT_EVENT_SUBSCRIBED:
			ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
			break;
		case MQTT_EVENT_UNSUBSCRIBED:
			ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
			break;
		case MQTT_EVENT_PUBLISHED:
			ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
			break;
		case MQTT_EVENT_DATA:
			ESP_LOGI(TAG, "MQTT_EVENT_DATA");
			//ESP_LOGI(TAG, "TOPIC=%.*s\r", event->topic_len, event->topic);
			//ESP_LOGI(TAG, "DATA=%.*s\r", event->data_len, event->data);
			MQTT_t mqttBuf;
			mqttBuf.topic_type = SUBSCRIBE;
			mqttBuf.topic_len = event->topic_len;
			for(int i=0;i<event->topic_len;i++) {
				mqttBuf.topic[i] = event->topic[i];
				mqttBuf.topic[i+1] = 0;
			}
			mqttBuf.payload_len = event->data_len;
			for(int i=0;i<event->data_len;i++) {
				mqttBuf.payload[i] = event->data[i];
			}
			xQueueSend(xQueueSubscribe, &mqttBuf, 0);
			break;
		case MQTT_EVENT_ERROR:
			ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
			break;
		default:
			ESP_LOGI(TAG, "Other event id:%d", event->event_id);
			break;
	}
	return ESP_OK;
}

void mqtt_sub_task(void *pvParameters)
{
	ESP_LOGI(TAG, "Start Subscribe Broker:%s", CONFIG_BROKER_URL);

	esp_mqtt_client_config_t mqtt_cfg = {
		.uri = CONFIG_BROKER_URL,
		.event_handle = mqtt_event_handler,
		.client_id = "subscribe",
	};

	xQueueSubscribe = xQueueCreate( 10, sizeof(MQTT_t) );
	configASSERT( xQueueSubscribe );

	s_mqtt_event_group = xEventGroupCreate();
	esp_mqtt_client_handle_t mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
	xEventGroupClearBits(s_mqtt_event_group, MQTT_CONNECTED_BIT);
	esp_mqtt_client_start(mqtt_client);
	xEventGroupWaitBits(s_mqtt_event_group, MQTT_CONNECTED_BIT, false, true, portMAX_DELAY);
	ESP_LOGI(TAG, "Connect to MQTT Server");


	// Subscribe topic
	esp_mqtt_client_subscribe(mqtt_client, CONFIG_MQTT_TOPIC, 0);

	MQTT_t mqttBuf;
	MQTT_t mqttBuf2;
	while (1) {
		ESP_LOGI(TAG, "Wait for %s", CONFIG_MQTT_TOPIC);
		xQueueReceive(xQueueSubscribe, &mqttBuf, portMAX_DELAY);
		ESP_LOGI(TAG, "type=%d", mqttBuf.topic_type);

		if (mqttBuf.topic_type != SUBSCRIBE) continue;
		//ESP_LOGI(TAG, "TOPIC=%.*s\r", mqttBuf.topic_len, mqttBuf.topic);
		ESP_LOGI(TAG, "TOPIC=[%s]", mqttBuf.topic);
		ESP_LOG_BUFFER_HEXDUMP(TAG, mqttBuf.payload, mqttBuf.payload_len, ESP_LOG_INFO);
#if 0
		for(int i=0;i<mqttBuf.payload_len;i++) {
			ESP_LOGI(TAG, "DATA=0x%x", mqttBuf.payload[i]);
		}
#endif
		memcpy((char *)&mqttBuf2, (char *)&mqttBuf, sizeof(MQTT_t));
		if (xQueueSend(xQueue_mqtt_rx, &mqttBuf2, portMAX_DELAY) != pdPASS) {
			ESP_LOGE(TAG, "xQueueSend Fail");
		}
	} // end while

	// Never reach here
	ESP_LOGI(TAG, "Task Delete");
	esp_mqtt_client_stop(mqtt_client);
	vTaskDelete(NULL);
}
