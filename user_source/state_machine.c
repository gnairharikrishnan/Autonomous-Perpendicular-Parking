
/*
 Code to setup the states and define its operation

 */
#include "cpu.h"
#include "hal_common_includes.h"
#include "stm32f3xx_hal.h"
#include "sensor_timer.h"
#include "motion.h"
#include "state_machine.h"
#include <stdio.h>


int g_current_state;
#define TURN_FORWARD_DISTANCE_THRESH 15
#define DIP_RISE_THRESH				 100
#define REF_DIST_FROM_RGT_WALL 8

typedef uint8_t bool;
#define TRUE 1U
#define FALSE 0U


bool g_ldip = FALSE;
bool g_lrise = FALSE;
bool g_rdip = FALSE;
bool g_rrise = FALSE;
bool parkFlag = FALSE;

float g_old_ld, g_old_rd;

uint32_t g_exit_counter = 0;

int8_t g_parking_dir = LEFT;

void state_init(void)
{
	g_current_state = STATE_IDLE;
}

void state_next(int next_state)
{
	g_current_state = next_state;
}

uint8_t state_execute(void)
{
	float ld, rd, fd;
	bool to_park = FALSE;
	uint8_t next_dir = FORWARD;
	switch(g_current_state)
	{
		case STATE_IDLE:
			state_next(STATE_FOLLOW_WALL);
		break;

		case STATE_FOLLOW_WALL:
			ld = usensor_check_distance(sensor_left);
			rd = usensor_check_distance(sensor_right);
			fd = usensor_check_distance(sensor_front);
		
	
			if(rd > 20 && ld > 60 ){
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
				next_dir = PULL_IN_PARK;
				break;
			}
			else if(rd > 20 )
			{
				next_dir = RIGHT;
				break;
			}

			if (fd < TURN_FORWARD_DISTANCE_THRESH)
				next_dir = LEFT; //calculate_next_dominant_turn(ld, rd);
			else if(rd > REF_DIST_FROM_RGT_WALL)
				next_dir = RECENTER_RIGHT; //Move towards the wall 
				else if(rd < REF_DIST_FROM_RGT_WALL - 4)
					next_dir = RECENTER_LEFT;
		            else
					next_dir = FORWARD;
		break;

	}
	return next_dir;
}

// #endif //CURR_MODE_PERPENDICULAR