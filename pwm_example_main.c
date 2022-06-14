/* pwm example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_log.h"
#include "esp_system.h"
#include "esp_err.h"

#include "esp8266/gpio_register.h"
#include "esp8266/pin_mux_register.h"

#include "driver/adc.h"
#include "driver/pwm.h"

static const char *TAG1 = "adc example";

#define PWM_0_OUT_IO_NUM   15
//#define PWM_1_OUT_IO_NUM   13
//#define PWM_2_OUT_IO_NUM   14
//#define PWM_3_OUT_IO_NUM   12

// PWM period 1000us(1Khz), same as depth
#define PWM_PERIOD1    (100)
#define PWM_PERIOD2    (75)
#define PWM_PERIOD3   (30)
static const char *TAG2 = "pwm_example";

// pwm pin number
const uint32_t pin_num =    PWM_0_OUT_IO_NUM ;//,
   // PWM_1_OUT_IO_NUM,
  //  PWM_2_OUT_IO_NUM,
    //PWM_3_OUT_IO_NUM


// duties table, real_duty = duties[x]/PERIOD
uint32_t duties_percent =  50 ;//, 500, 500, 500,


// phase table, delay = (phase[x]/360)*PERIOD
float phase = 0;//, 0, 90.0, -90.0,

static uint16_t adc_reading;
static void adc_task()
{
    int x;
    uint16_t adc_data[100];

    while (1) {
        // if (ESP_OK == adc_read(&adc_data[0])) {
        //     ESP_LOGI(TAG1, "adc read: %d", adc_data[0]);
        //     adc_reading = adc_data[0];
        // }

        // // ESP_LOGI(TAG1, "adc read fast:\r\n");

        uint32_t adc_sum = 0;
        if (ESP_OK == adc_read_fast(adc_data, 100)) {
            for (x = 0; x < 100; x++) {
                adc_sum = adc_sum + adc_data[x];
                // printf("%d\n", adc_data[x]);
                
            }
            uint32_t adc_avg = adc_sum/100;
            int nilai = (adc_avg * 2935) /1023;
            // int nilai = (adc_avg * 2.8796) - 97.785;
            ESP_LOGI(TAG1, "ADC Avg : %u", adc_avg);
            //ESP_LOGI(TAG1,  "Voltage : %i", voltage);
            printf ("voltage : %d\n", nilai);
        }

            vTaskDelay(1000 / portTICK_RATE_MS);
        
    }
}

// esp_err_t pwm_task()
// {
//       int16_t count = 0;  
    
//     // while (1) {
//     //     if () {
//     //        
//     //     } else if (count == 30) {
//     //         pwm_start();
//     //         ESP_LOGI(TAG2, "PWM re-start\n");
//     //         count = 0;
            
//     //     }
//         vTaskDelay (1000/ portTICK_RATE_MS);
//         count++;
       
//     }
// }

void app_main()
{
     // 1. init adc
    adc_config_t adc_config;

    // Depend on menuconfig->Component config->PHY->vdd33_const value
    // When measuring system voltage(ADC_READ_VDD_MODE), vdd33_const must be set to 255.
    adc_config.mode = ADC_READ_TOUT_MODE;
    adc_config.clk_div = 8; // ADC sample collection clock = 80MHz/clk_div = 10MHz
    ESP_ERROR_CHECK(adc_init(&adc_config));

    // 2. Create a adc task to read adc value
    xTaskCreate(adc_task, "adc_task", 1024*2, NULL, 5, NULL);

    // pwm_init(PWM_PERIOD1, &duties, 1, &pin_num);
    // pwm_set_phases(&phase);
    // ESP_LOGI (TAG2, "Frekuensi 1");
    // pwm_start();
        uint32_t duties = duties_percent*PWM_PERIOD1/100;
         pwm_init(PWM_PERIOD1, &duties, 1, &pin_num);
while (1) 
    {
            
    duties = duties_percent*PWM_PERIOD1/100;
    pwm_set_duties (&duties);
    pwm_set_period (PWM_PERIOD1);
    pwm_set_phases(&phase);
    // ESP_LOGI (TAG2, "Frekuensi 1");
    pwm_start();
    vTaskDelay (10000/ portTICK_RATE_MS);
    pwm_stop (0x0);

     duties = duties_percent*PWM_PERIOD2/100;
     pwm_set_duties(&duties);
     pwm_set_period (PWM_PERIOD2);
    pwm_set_phases(&phase);
    // ESP_LOGI (TAG2, "Frekuensi 2");
    pwm_start();
    vTaskDelay (10000/ portTICK_RATE_MS);
    pwm_stop (0x0);

     duties = duties_percent*PWM_PERIOD3/100;
     pwm_set_duties(&duties);
     pwm_set_period (PWM_PERIOD3);
    pwm_set_phases(&phase);
    // ESP_LOGI (TAG2, "Frekuensi 3");
    pwm_start();
    vTaskDelay (10000/ portTICK_RATE_MS);
    pwm_stop (0x0);
    // xTaskCreate(pwm_task, "pwm_task", 1024*2, NULL, 5, NULL);
    }
}

