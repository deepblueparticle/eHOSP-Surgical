/**
Copyright 2018 Alexios Karadimos

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
**/

#include <ros.h>
#include <Arduino.h>
#include <std_msgs/Empty.h>
#include <std_msgs/Int8.h>
#include "A4988.h"

// Debugging LED
#define RGB_GREEN 0
#define RGB_BLUE 1
#define RGB_RED 2

// CNC Motors
// using a 200-step motor (most common)
#define MOTOR_STEPS 200
// Configuring STEP and DIR pins for A4988 motor driver
#define MOTOR_X1_STEP 3
#define MOTOR_X1_DIR 4
#define MOTOR_X2_STEP 5
#define MOTOR_X2_DIR 6
#define MOTOR_Y_STEP 7
#define MOTOR_Y_DIR 8
#define MOTOR_Z_STEP 10
#define MOTOR_Z_DIR 9

// Robotic Arms Motors

// Global coordinate system
double X, Y, Z;
// Local coordinate system
double x, y, z;

// Initialize ROS node
ros::NodeHandle  nh;

/*************************************************************************
*       STEPPER MOTORS
*************************************************************************/

A4988 stepper_X1_axis(MOTOR_STEPS, MOTOR_X1_DIR, MOTOR_X1_STEP);
A4988 stepper_X2_axis(MOTOR_STEPS, MOTOR_X2_DIR, MOTOR_X2_STEP);
A4988 stepper_Y_axis(MOTOR_STEPS, MOTOR_Y_DIR, MOTOR_Y_STEP);
A4988 stepper_Z_axis(MOTOR_STEPS, MOTOR_Z_DIR, MOTOR_Z_STEP);

void startMotorsX(const std_msgs::Int8& rpm){
  // blink the led, to indicate stepper motors of X axis have started
  digitalWrite(RGB_GREEN, HIGH);
  stepper_X1_axis.begin(rpm.data, 16);
  stepper_X2_axis.begin(rpm.data, 16);
  stepper_X1_axis.rotate(360);
  stepper_X2_axis.rotate(-360);
  digitalWrite(RGB_GREEN, LOW);
}

void startMotorY(const std_msgs::Empty& toggle_msg){
  // blink the blue led, to indicate motorY has started
  digitalWrite(RGB_BLUE, HIGH);
  stepper_Y_axis.begin(15, 16);
  stepper_Y_axis.rotate(360);
  digitalWrite(RGB_BLUE, LOW);
}

void startMotorZ(const std_msgs::Empty& toggle_msg){
  // blink the red led, to indicate motorZ has started
  digitalWrite(RGB_RED, HIGH);
  stepper_Z_axis.begin(15, 16);
  stepper_Z_axis.rotate(360);
  digitalWrite(RGB_RED, LOW);
}

ros::Subscriber<std_msgs::Int8> motorsX("motorsX/start", &startMotorsX);
ros::Subscriber<std_msgs::Empty> motorY("motorY/start", &startMotorY);
ros::Subscriber<std_msgs::Empty> motorZ("motorZ/start", &startMotorZ);



/*************************************************************************
*       MAIN PROGRAM
*************************************************************************/

void setup() { 
  pinMode(RGB_GREEN, OUTPUT);
  pinMode(RGB_BLUE, OUTPUT);
  pinMode(RGB_RED, OUTPUT);
  
  nh.initNode();
  
  nh.subscribe(motorsX);
  nh.subscribe(motorY);
  nh.subscribe(motorZ);
}

void loop() {  
  nh.spinOnce();
  delay(1);
}