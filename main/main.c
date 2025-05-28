#include "tft_lcd_ili9341/ili9341/ili9341.h"
#include "tft_lcd_ili9341/gfx/gfx.h"

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// backlight pin
#define LCD_BACKLIGHT_PIN   15

// LDR on ADC0 / GPIO26
#define LDR_ADC_CH          0
#define LDR_GPIO            26

// queue holds one uint16_t
static QueueHandle_t xLdrQueue;

static void sensor_task(void *pvParameters) {
    uint16_t val;
    for (;;) {
        adc_select_input(LDR_ADC_CH);
        val = adc_read();
        xQueueOverwrite(xLdrQueue, &val);
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}


static void display_task(void *pvParameters) {
    uint16_t ldr;

    LCD_initDisplay();
    LCD_setRotation(0);
    GFX_createFramebuf();

    gpio_init(LCD_BACKLIGHT_PIN);
    gpio_set_dir(LCD_BACKLIGHT_PIN, GPIO_OUT);
    gpio_put(LCD_BACKLIGHT_PIN, 1);

    for (;;) {
        if (xQueueReceive(xLdrQueue, &ldr, portMAX_DELAY) == pdTRUE) {
            GFX_clearScreen();
            GFX_setCursor(0, 10);
            GFX_printf("LDR: %4u\n", ldr);
            GFX_flush();
        }
    }
}

int main() {
    stdio_init_all();

    adc_init();
    adc_gpio_init(LDR_GPIO);

    xLdrQueue = xQueueCreate(1, sizeof(uint16_t));

    xTaskCreate(sensor_task,  "SENSOR",  256, NULL, 1, NULL);
    xTaskCreate(display_task, "DISPLAY", 1024, NULL, 1, NULL);

    vTaskStartScheduler();

    for (;;);
    return 0;
}
