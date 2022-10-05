#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"

void print_studentID(void *pvParameter)
{
    while (1)
    {
        printf("\nTask1: 1915676 - %lld\n", esp_timer_get_time()/1000000);
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}
void read_button(void *param)
{
    gpio_pad_select_gpio(GPIO_NUM_19);
    gpio_pad_select_gpio(GPIO_NUM_12);
    gpio_set_direction(GPIO_NUM_19, GPIO_MODE_INPUT);
    gpio_set_direction(GPIO_NUM_12, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(GPIO_NUM_19, GPIO_PULLUP_ONLY);
    bool currentStatus = 1, previousStatus = 1;
    int count = 0, toggle = 0;

    while (1)
    {
        currentStatus = gpio_get_level(GPIO_NUM_19);
        if (currentStatus == previousStatus && currentStatus == 0)
        {
            if (count == 0)
            {
                gpio_set_level(GPIO_NUM_12, toggle%2);
                if(toggle%2 == 1)
                    printf("ESP32 - led on\n");
                else
                    printf("ESP32 - led off\n");
                toggle ++;
            }
            count++;
        }
        else
        {
            previousStatus = currentStatus;
            count = 0;
        }
        vTaskDelay(20 / portTICK_RATE_MS);
    }
}
void app_main(void)
{
    xTaskCreate(print_studentID, "Print student ID", 2048, NULL, 5, NULL);
    xTaskCreate(read_button, "Reading from button", 2048, NULL, 6, NULL);
}