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
// This sample code show how to drive two brushless DC motors.
//
//
//# Prepare
//
//- MPU
//	- Arduino UNO board (ATMEGA328P)
//	- http://s.click.aliexpress.com/e/_dTG6WHQ
//
//- Motor driver (2pcs)
//	- L298N motor driver module
//	- http://s.click.aliexpress.com/e/_dVlgCao
//
//- Brushless DC motor 1
//	- DYS GM60-80T(24N22P) gimbal motor
//	- http://s.click.aliexpress.com/e/_d6vn8wY
//
//- Brushless DC motor 2
//	- EMAX MT2204-2300KV(12N14P) drone motor
//	- http://s.click.aliexpress.com/e/_dXdxFgg
//
//- External battery
//	- 5V mobile battery
//
//
//# Connection
//
//	Arduino UNO 3pin     ->    L298N modle 1 IN1
//	Arduino UNO 5pin     ->    L298N modle 1 IN2
//	Arduino UNO 6pin     ->    L298N modle 1 IN3
//
//	L298N modle 1 OUT1   ->    GM60-80T U
//	L298N modle 1 OUT2   ->    GM60-80T V
//	L298N modle 1 OUT3   ->    GM60-80T W
//
//	mobile battery 5V    ->    L298N modle 1 5V
//	mobile battery 5V    ->    L298N modle 1 VIN
//	mobile battery GND   ->    L298N modle 1 GND
//
//	Arduino UNO 9pin     ->    L298N modle 2 IN1
//	Arduino UNO 10pin    ->    L298N modle 2 IN2
//	Arduino UNO 11pin    ->    L298N modle 2 IN3
//
//	L298N modle 2 OUT1   ->    MT2204-2300KV U
//	L298N modle 2 OUT2   ->    MT2204-2300KV V
//	L298N modle 2 OUT3   ->    MT2204-2300KV W
//
//	mobile battery 5V    ->    L298N modle 2 5V
//	mobile battery 5V    ->    L298N modle 2 VIN
//	mobile battery GND   ->    L298N modle 2 GND
//
//	mobile battery GND   ->    Arduino UNO GND
//
//	*Both L298N modles' ENA/ENB jumpers are closed
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


UcnBrushlessDCMotorPWM _bldc1;
UcnBrushlessDCMotorPWM _bldc2;

//PWM output pin for BLDC driver
#define BLDC1_DRV_IN1	3
#define BLDC1_DRV_IN2	5
#define BLDC1_DRV_IN3	6

//PWM output pin for BLDC driver
#define BLDC2_DRV_IN1	9
#define BLDC2_DRV_IN2	10
#define BLDC2_DRV_IN3	11


//Pole count of BLDC motor
#define BLDC1_MOTOR_POLE		22		//24N22P
#define BLDC2_MOTOR_POLE		14		//12N14P


void setup()
{
	_bldc1.begin(BLDC1_MOTOR_POLE, BLDC1_DRV_IN1, BLDC1_DRV_IN2, BLDC1_DRV_IN3);
	_bldc1.SetPower(80);  		//motor power 80%
	
	_bldc2.begin(BLDC2_MOTOR_POLE, BLDC2_DRV_IN1, BLDC2_DRV_IN2, BLDC2_DRV_IN3);
	_bldc2.SetPower(40);  		//motor power 40%
}


uint32_t _nBldcRequreStep1 = 0;
int8_t _nBldcStep1 = 1;

uint32_t _nBldcRequreStep2 = 0;
int8_t _nBldcStep2 = 1;

void loop()
{
	if (_nBldcRequreStep1 == 0)
	{
		_nBldcRequreStep1 = _bldc1.calcRequireStepCount(90);
		_nBldcStep1 = -_nBldcStep1;
	}
	
	if (_nBldcRequreStep2 == 0)
	{
		_nBldcRequreStep2 = _bldc2.calcRequireStepCount(180);
		_nBldcStep2 = -_nBldcStep2;
	}

	_bldc1.DoRotate(_nBldcStep1);
	_bldc2.DoRotate(_nBldcStep2);

	_nBldcRequreStep1--;
	_nBldcRequreStep2--;

	delayMicroseconds(500);
}


