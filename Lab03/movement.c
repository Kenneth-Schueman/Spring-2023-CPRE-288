/*
 * movement.c
 *
 *  Created on: Feb 1, 2023
 *      Author: npin
 */
#include "lcd.h"
#include "open_interface.h"
#include "Timer.h"
#include <string.h>

#define ANGLE_MOD 15
#define BUMP_BACKTRACK_MM 150
#define BUMP_SIDESTEP_MM 250

double move_forward(oi_t *sensor_data, double distance_mm, int moveSpeed){

    double sum = 0;
    oi_setWheels(moveSpeed, moveSpeed);

    while(sum < distance_mm){
        oi_update(sensor_data);
        sum+= sensor_data -> distance;
    }

    oi_setWheels(0,0);

    return sum;
}

double move_backward(oi_t *sensor_data, double distance_mm, int moveSpeed){

    double sum = 0;
    oi_setWheels(-moveSpeed, -moveSpeed);

    while(sum < distance_mm){
        oi_update(sensor_data);
        sum-= sensor_data -> distance;
    }

    oi_setWheels(0,0);

    return sum;
}




void turn_right(oi_t*sensor_data, double degrees){
    degrees -= ANGLE_MOD;
    double currAngle = 0;
    oi_setWheels(-250, 250);

    while(currAngle < degrees){
        oi_update(sensor_data);
        currAngle -= sensor_data -> angle;
    }
    oi_setWheels(0,0);
}

void turn_left(oi_t*sensor_data, double degrees){
    degrees -= ANGLE_MOD;
       double currAngle = 0;
       oi_setWheels(250, -250);

       while(currAngle < degrees){
           oi_update(sensor_data);
           currAngle += sensor_data -> angle;
       }
       oi_setWheels(0,0);
}

void move_forward_smart(oi_t *sensor_data, double distance_mm, int move_speed)
{
    double dist_moved_mm = 0;
    int r_bump_status = 0;
    int l_bump_status = 0;
    oi_setWheels(move_speed, move_speed);

    while(dist_moved_mm < distance_mm)
    {
        oi_update(sensor_data);
        dist_moved_mm += sensor_data -> distance;
        r_bump_status = sensor_data -> bumpRight;
        l_bump_status = sensor_data -> bumpLeft;
        lcd_printf("%f", &dist_moved_mm);


        if(l_bump_status != 0)
        {
            oi_setWheels(0,0);
            move_backward(sensor_data, BUMP_BACKTRACK_MM, 125);
            dist_moved_mm -= BUMP_BACKTRACK_MM;

            turn_right(sensor_data, 90);
            move_forward(sensor_data, BUMP_SIDESTEP_MM, 125);

            turn_left(sensor_data, 90);
            move_forward(sensor_data, BUMP_SIDESTEP_MM, 125);
            oi_setWheels(move_speed, move_speed);
        }
        if(r_bump_status != 0)
        {
            oi_setWheels(0,0);
            move_backward(sensor_data, BUMP_BACKTRACK_MM, 125);
            dist_moved_mm -= BUMP_BACKTRACK_MM;

            turn_left(sensor_data, 90);
            move_forward(sensor_data, BUMP_SIDESTEP_MM, 125);

            turn_right(sensor_data, 90);
            move_forward(sensor_data, BUMP_SIDESTEP_MM, 125);
            oi_setWheels(move_speed, move_speed);
        }
    }

   oi_setWheels(0,0);

}


