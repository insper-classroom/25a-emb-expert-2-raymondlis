/*
 * LED blink with FreeRTOS
 */
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>


#include "pico/stdlib.h"
#include <stdio.h>

void task_1(void *p) {
    while (1) {
	printf("hello from task 1 \n");
	vTaskDelay(pdMS_TO_TICKS(500));
    }
}


void task_2(void *p) {
    char cnt = 15;
    while (1) {

	printf("hello from task 2 \n");
	vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main() {
    stdio_init_all();

    xTaskCreate(task_1, "Task 1", 4095, NULL, 1, NULL);
    xTaskCreate(task_2, "Task 2", 4095, NULL, 1, NULL);
    
    vTaskStartScheduler();

    while (true)
        ;
}
