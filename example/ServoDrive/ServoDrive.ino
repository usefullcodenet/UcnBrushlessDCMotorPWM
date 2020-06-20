//////////////////////////////////////////////////////////////////////////
//
// UcnBrushlessDCMotorPWM example
//
// Arduino library for driving brushless DC motor like as servo motor
//
//
// https://usefullcode.net/
// https://github.com/usefullcodenet/UcnBrushlessDCMotorPWM
//
// Copyright (c) 2020 usefullcode.net <usefullcode@gmail.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//
//////////////////////////////////////////////////////////////////////////
//
//# Abstruct
//
// This sample code show how to drive brushless DC motor with 180 degree rotation.
//
//
//# Prepare
//
//- MPU
//	- Arduino UNO board (ATMEGA328P)
//	- http://s.click.aliexpress.com/e/_dTG6WHQ
//
//- Motor driver
//	- L298N motor driver module
//	- http://s.click.aliexpress.com/e/_dVlgCao
//
//- Brushless DC motor
//	- DYS GM60-80T(24N22P) gimbal motor
//	- http://s.click.aliexpress.com/e/_d6vn8wY
//
//- External battery
//	- 5V mobile battery
//
//
//# Connection
//
//	Arduino UNO 9pin     ->    L298N modle IN1
//	Arduino UNO 10pin    ->    L298N modle IN2
//	Arduino UNO 11pin    ->    L298N modle IN3
//
//	L298N modle OUT1     ->    GM60-80T U
//	L298N modle OUT2     ->    GM60-80T V
//	L298N modle OUT3     ->    GM60-80T W
//
//	mobile battery 5V    ->    L298N modle 5V
//	mobile battery 5V    ->    L298N modle VIN
//	mobile battery GND   ->    L298N modle GND
//
//	mobile battery GND   ->    Arduino UNO GND
//
//	*L298N modle ENA/ENB jumpers are closed
//
//
//# ATTENTION
//
//	The brushless DC motor needs a large current. Be sure to use ampere meter.
//	Mobile battery with built-in safety circuit is recommended for the motor power supply,
//	and USB current meter is useful.
//
//	USB current meter	http://s.click.aliexpress.com/e/_dZpiddS
//


#include <UcnBrushlessDCMotorPWM.h>


UcnBrushlessDCMotorPWM _bldc;

//PWM output pin for BLDC driver
#define BLDC_DRV_IN1	9
#define BLDC_DRV_IN2	10
#define BLDC_DRV_IN3	11

//(option) ENABLE pin for BLDC driver
#define BLDC_DRV_EN1	-1	//4
#define BLDC_DRV_EN2	-1	//5
#define BLDC_DRV_EN3	-1	//6

//Pole count of BLDC motor
//#define BLDC_MOTOR_POLE		14		//12N14P
#define BLDC_MOTOR_POLE		22		//24N22P
//#define BLDC_MOTOR_POLE		26		//24N26P
//#define BLDC_MOTOR_POLE		28		//24N28P
//#define BLDC_MOTOR_POLE		32		//24N32P


void setup()
{
	_bldc.begin(BLDC_MOTOR_POLE, BLDC_DRV_IN1, BLDC_DRV_IN2, BLDC_DRV_IN3, BLDC_DRV_EN1, BLDC_DRV_EN2, BLDC_DRV_EN3);

	_bldc.SetPower(80);		//motor power 80%
}

void loop()
{
	//180 degree rotation with 3000 usec delay loop
	_bldc.DoRotateLoop(180, 3000);
	
	//180 degree inverse rotation with 3000 usec delay loop
	_bldc.DoRotateLoop(-180, 3000);
}

