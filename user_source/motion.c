
#include "cpu.h"
#include "hal_common_includes.h" 
#include "pwm_hal.h"
#include "motor_hal.h"
#include "motion.h"
#include "encoder.h"
#include <stdio.h>


#define HIGH      400U
#define LOW       0U
#define MEDIUM    0U

extern float g_inches_f;


void move_forward()
{
  configure_channel(1, LOW);
  configure_channel(2, HIGH);
  configure_channel(3, LOW);
  configure_channel(4, HIGH + 04U);
}

void move_back()
{
  configure_channel(1, HIGH);
  configure_channel(2, LOW);
  configure_channel(3, HIGH);
  configure_channel(4, LOW);
}

void move_back_left()
{
  configure_channel(1, LOW);
  configure_channel(2, LOW);
  configure_channel(3, HIGH);
  configure_channel(4, LOW);
}

void move_back_right()
{
  configure_channel(1, HIGH);
  configure_channel(2, LOW);
  configure_channel(3, LOW);
  configure_channel(4, LOW);
}


void turn_f_left()
{
  configure_channel(1, LOW);
  configure_channel(2, HIGH);
  configure_channel(3, LOW);
  configure_channel(4, MEDIUM);
}

void turn_f_right()
{
  configure_channel(1, LOW);
  configure_channel(2, MEDIUM);
  configure_channel(3, LOW);
  configure_channel(4, HIGH);
}

void brake()
{
  configure_channel(1, LOW);
  configure_channel(2, LOW);
  configure_channel(3, LOW);
  configure_channel(4, LOW);
}

void move_robot(uint8_t next_dir)
{
  switch(next_dir)
  {
    case LEFT:
      brake();
      cpu_sw_delay(10U);
      encoders_enable();
      turn_f_left();
      
      while(g_rightEncoderCount < 35)
      {
          printf("\n\r");
      }

      encoders_disable();
      brake();

      cpu_sw_delay(20U);

      break;
    case RIGHT:
      brake();
      cpu_sw_delay(10U);

      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
      encoders_enable();

      move_forward();
      while(g_leftEncoderCount < 24 && g_rightEncoderCount < 24){
        printf("\n\r");
      }

      brake();
      cpu_sw_delay(40U);


      encoders_reset();
      turn_f_right();

      while(g_leftEncoderCount < 39){
        printf("\n\r");
      }

      encoders_disable();

     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
      brake();

      break;
    case FORWARD:
      move_forward();
      cpu_sw_delay(10U);
      brake();
      break;
    case BACK:
      move_back();
      break;
    case RECENTER_RIGHT:
      brake();
      cpu_sw_delay(04U);
      turn_f_right();
      cpu_sw_delay(01U);
      brake();
      break;

    case RECENTER_LEFT:
      brake();
      cpu_sw_delay(04U);
      turn_f_left();
      cpu_sw_delay(01U);
      brake();
      break;

     case PULL_IN_PARK:
        brake();
        cpu_sw_delay(10U);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);

        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
        encoders_enable();

        move_forward();
        while(g_leftEncoderCount < 26 && g_rightEncoderCount < 26){
          printf("\n\r");
        }

      brake();
        cpu_sw_delay(40U);
        encoders_reset();

  
        turn_f_right();

        while(g_leftEncoderCount < 36){
          printf("\n\r");
        }

        brake();
        cpu_sw_delay(40U);
        encoders_reset();

        move_forward();
        while(g_leftEncoderCount < 45 && g_rightEncoderCount < 45){
          printf("\n\r");
        }
        encoders_disable();

    

       brake();
       while(1)
       {

        //Red and BLUE LED will be on to show we parked
         HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
         HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
       }

      break;
    case STOP:
      brake();
      break;

  }
} 