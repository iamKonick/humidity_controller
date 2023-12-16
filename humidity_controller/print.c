// FreeRTOS
#include <FreeRTOS.h>
#include <task.h>

#include <stdio.h>

void task_print(void *pvParameters)
{
	while(1){
		printf("print task is also running in the background!\r\n");
		// wait for 1000ms
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
	// should never happen but would delete the task and free allocated resources
    vTaskDelete(NULL);  
}
