#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define PIN_LED_RED 13
#define PIN_LED_GREEN 11
#define PIN_LED_BLUE 12

#define PIN_BUZZER 10

#define PIN_BUTTON_A 5
#define PIN_BUTTON_B 6

bool gFlagLED = false;
bool gFlagBUZZER = false;

void Task_LED() 
{
    gpio_init(PIN_LED_RED);
    gpio_set_dir(PIN_LED_RED, GPIO_OUT);
    gpio_init(PIN_LED_GREEN);
    gpio_set_dir(PIN_LED_GREEN, GPIO_OUT);
    gpio_init(PIN_LED_BLUE);
    gpio_set_dir(PIN_LED_BLUE, GPIO_OUT);

    while (1) 
    {
        if(gFlagLED)
        {
            gpio_put(PIN_LED_BLUE, 0);
            gpio_put(PIN_LED_RED, 1);
            vTaskDelay(pdMS_TO_TICKS(500));
            
            gpio_put(PIN_LED_RED, 0);
            gpio_put(PIN_LED_GREEN, 1);
            vTaskDelay(pdMS_TO_TICKS(500));
            
            gpio_put(PIN_LED_GREEN, 0);
            gpio_put(PIN_LED_BLUE, 1);
            vTaskDelay(pdMS_TO_TICKS(500));
        }
        vTaskDelay(10);
    }
}

void Task_BUZZER() 
{
    const uint divider = 125;
    const uint period = 1000;
    const uint duty = 500;

    gpio_set_function(PIN_BUZZER, GPIO_FUNC_PWM);
    uint pwm_slice = pwm_gpio_to_slice_num(PIN_BUZZER);
    pwm_set_clkdiv(pwm_slice, divider);
    pwm_set_wrap(pwm_slice, period - 1);
    pwm_set_gpio_level(PIN_BUZZER, duty);
    pwm_set_enabled(pwm_slice, false);

    while (1) 
    {
        if(gFlagBUZZER) 
        {
            pwm_set_enabled(pwm_slice, true);
            vTaskDelay(pdMS_TO_TICKS(1000));
            gFlagBUZZER = 0;
        } 
        else 
        {
            pwm_set_enabled(pwm_slice, false);
        }
        vTaskDelay(10);
    }
}

void Task_BUTTONS()
{
    gpio_init(PIN_BUTTON_A);
    gpio_set_dir(PIN_BUTTON_A, GPIO_IN);
    gpio_pull_up(PIN_BUTTON_A);

    gpio_init(PIN_BUTTON_B);
    gpio_set_dir(PIN_BUTTON_B, GPIO_IN);
    gpio_pull_up(PIN_BUTTON_B);

    bool lastA = true, lastB = true;

    while (1) 
    {
        bool currentA = gpio_get(PIN_BUTTON_A);
        bool currentB = gpio_get(PIN_BUTTON_B);

        if (!currentA && lastA) 
        {
            gFlagLED = !gFlagLED;
        }

        if (!currentB && lastB) 
        {
            gFlagBUZZER = !gFlagBUZZER;
        }

        lastA = currentA;
        lastB = currentB;

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

int main() 
{
    stdio_init_all();

    xTaskCreate(Task_LED, "LED", 256, NULL, 1, NULL);
    xTaskCreate(Task_BUZZER, "BUZZER", 256, NULL, 1, NULL);
    xTaskCreate(Task_BUTTONS, "BUTTONS", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    while (1) {};
}