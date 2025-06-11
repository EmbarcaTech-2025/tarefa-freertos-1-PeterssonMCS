#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

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

  while (true) 
  {
    if( gFlagLED )
    {
      gpio_put(PIN_LED_BLUE, 0);
      gpio_put(PIN_LED_RED, 1);
      vTaskDelay(1000);
      gpio_put(PIN_LED_RED, 0);
      gpio_put(PIN_LED_GREEN, 1);
      vTaskDelay(1000);
      gpio_put(PIN_LED_BLUE, 1);
      gpio_put(PIN_LED_GREEN, 0);
    }
    vTaskDelay(1000);
  }
}

void Task_BUZZER() 
{
  gpio_init(PIN_BUZZER);
  gpio_set_dir(PIN_BUZZER, GPIO_OUT);

  while (true) 
  {
    if( gFlagBUZZER )
    {
      for( int i = 0; i < 100 ; i++ )
      {
        gpio_put(PIN_BUZZER, 0);
        vTaskDelay(1);
        gpio_put(PIN_BUZZER, 1);
      }
    }
    vTaskDelay(10000);
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

  while (true) 
  {
    if( !gpio_get( PIN_BUTTON_A ))
    {
      gFlagLED = !gFlagLED;
    }else if ( !gpio_get( PIN_BUTTON_B ))
    {
      gFlagBUZZER = !gFlagBUZZER;
    }
    vTaskDelay(200);
  }
}

int main() 
{
  stdio_init_all();

  xTaskCreate( Task_LED , "Task_LED", 256, NULL, 1, NULL);
  xTaskCreate( Task_BUZZER , "Task_BUZZER", 256, NULL, 1, NULL);
  xTaskCreate( Task_BUTTONS , "Task_BUTTONS", 256, NULL, 1, NULL);

  vTaskStartScheduler();

  while(1){};
}