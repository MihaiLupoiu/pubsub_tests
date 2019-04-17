#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "pubsub.h"

void subscribe_task(void *pvParam) {
	char *task_name = pcTaskGetTaskName(NULL);
	ps_subscriber_t *sub = ps_new_subscriber(10, STRLIST("main.evt.random"));
	ESP_LOGI(task_name, "Started task.");
	ps_msg_t *msg;
	while (1) {
		msg = ps_get(sub, 500);
		if (msg != NULL) {
			if (IS_INT(msg))
				ESP_LOGI(task_name, "Got %lld", msg->int_val);
			else
				ESP_LOGI(task_name, "Messege not an integer");
			ps_unref_msg(msg);
		} else {
			ESP_LOGI(task_name, "Messege is NULL");
		}
	}
}

void publish_task(void *pvParam) {
	char *task_name = pcTaskGetTaskName(NULL);
	ESP_LOGI(task_name, "Started task.");
	while (1) {
		int rand = esp_random();
		ESP_LOGI(task_name, "Sending random value %d", rand);
		PUB_INT("main.evt.random", rand);
		vTaskDelay(5000 / portTICK_PERIOD_MS);
	}
}

void app_main() {

	ps_init();

	ESP_LOGI("MAIN", "Starting tasks.");
	xTaskCreate(&subscribe_task, "subs_task_1", 2048, NULL, 5, NULL);
	xTaskCreate(&publish_task, "pub_task_1", 2048, NULL, 5, NULL);
	// xTaskCreate(&subscribe_task, "subs_task_2", 2048, NULL, 5, NULL);
	// xTaskCreate(&publish_task, "pub_task_2", 2048, NULL, 5, NULL);
}