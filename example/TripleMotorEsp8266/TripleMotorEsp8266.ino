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
// This sample code show how to drive three brushless DC motors.
//
//
//# Prepare
//
//- MPU
//	- WeMos D1 mini (ESP8266)
//	- http://s.click.aliexpress.com/e/_dTOy2L6
//
//- Motor driver (3pcs)
//	- L298N motor driver module
//	- http://s.click.aliexpress.com/e/_dVlgCao
//
//- Brushless DC motor 1
//	- EMAX MT2204-2300KV(12N14P) drone motor
//	- http://s.click.aliexpress.com/e/_dXdxFgg
//
//- Brushless DC motor 2
//	- EMAX MT2204-2300KV(12N14P) drone motor
//	- http://s.click.aliexpress.com/e/_dXdxFgg
//
//- Brushless DC motor 3
//	- DYS GM60-80T(24N22P) gimbal motor
//	- http://s.click.aliexpress.com/e/_d6vn8wY
//
//- External battery
//	- 5V mobile battery
//
//
//# Connection
//
//	WeMos D1 mini D1     ->    L298N modle 1 IN1
//	WeMos D1 mini D2     ->    L298N modle 1 IN2
//	WeMos D1 mini D3     ->    L298N modle 1 IN3
//
//	mobile battery 5V    ->    L298N modle 1 5V
//	mobile battery 5V    ->    L298N modle 1 VIN
//	mobile battery GND   ->    L298N modle 1 GND
//
//	L298N modle 1 OUT1   ->    MT2204-2300KV U
//	L298N modle 1 OUT2   ->    MT2204-2300KV V
//	L298N modle 1 OUT3   ->    MT2204-2300KV W
//
//
//	WeMos D1 mini D0     ->    L298N modle 2 IN1
//	WeMos D1 mini D5     ->    L298N modle 2 IN2
//	WeMos D1 mini D6     ->    L298N modle 2 IN3
//
//	L298N modle 2 OUT1   ->    MT2204-2300KV U
//	L298N modle 2 OUT2   ->    MT2204-2300KV V
//	L298N modle 2 OUT3   ->    MT2204-2300KV W
//
//	mobile battery 5V    ->    L298N modle 2 5V
//	mobile battery 5V    ->    L298N modle 2 VIN
//	mobile battery GND   ->    L298N modle 2 GND
//
//
//	WeMos D1 mini D8     ->    L298N modle 3 IN1
//	WeMos D1 mini D7     ->    L298N modle 3 IN2
//	WeMos D1 mini D4     ->    L298N modle 3 IN3
//
//	L298N modle 3 OUT1   ->    GM60-80T U
//	L298N modle 3 OUT2   ->    GM60-80T V
//	L298N modle 3 OUT3   ->    GM60-80T W
//
//	mobile battery 5V    ->    L298N modle 3 5V
//	mobile battery 5V    ->    L298N modle 3 VIN
//	mobile battery GND   ->    L298N modle 3 GND
//
//
//	mobile battery GND   ->    Arduino UNO GND
//
//	*Both L298N modles' ENA/ENB jumpers are closed
//
//	*It may not work due to insufficient current, add mobile battery for each BLDC motor.
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
UcnBrushlessDCMotorPWM _bldc3;


#define D0	16
#define D1	5
#define D2	4
#define D3	0
#define D4	2
#define D5	14
#define D6	12
#define D7	13
#define D8	15


//PWM output pin for BLDC driver
#define BLDC1_DRV_IN1	D1
#define BLDC1_DRV_IN2	D2
#define BLDC1_DRV_IN3	D3

//PWM output pin for BLDC driver
#define BLDC2_DRV_IN1	D0
#define BLDC2_DRV_IN2	D5
#define BLDC2_DRV_IN3	D6

//PWM output pin for BLDC driver
#define BLDC3_DRV_IN1	D8
#define BLDC3_DRV_IN2	D7
#define BLDC3_DRV_IN3	D4


//Pole count of BLDC motor
#define BLDC1_MOTOR_POLE		14		//12N14P
#define BLDC2_MOTOR_POLE		14		//12N14P
#define BLDC3_MOTOR_POLE		22		//24N22P




void setup()
{
	_bldc1.begin(BLDC1_MOTOR_POLE, BLDC1_DRV_IN1, BLDC1_DRV_IN2, BLDC1_DRV_IN3, -1, -1, -1, 100);
	_bldc1.suspend();
	
	_bldc2.begin(BLDC2_MOTOR_POLE, BLDC2_DRV_IN1, BLDC2_DRV_IN2, BLDC2_DRV_IN3, -1, -1, -1, 100);
	_bldc2.suspend();
	
	_bldc3.begin(BLDC3_MOTOR_POLE, BLDC3_DRV_IN1, BLDC3_DRV_IN2, BLDC3_DRV_IN3);
	_bldc3.suspend();

	_bldc1.SetPower(40);  		//motor power 40%
	_bldc2.SetPower(40);  		//motor power 40%
	_bldc3.SetPower(80);  		//motor power 80%
}


uint32_t _nBldcRequreStep1 = 0;
int8_t _nBldcStep1 = 1;

uint32_t _nBldcRequreStep2 = 0;
int8_t _nBldcStep2 = 1;

uint32_t _nBldcRequreStep3 = 0;
int8_t _nBldcStep3 = 1;



void loop()
{
	if (_nBldcRequreStep1 == 0)
	{
		//90 degree rotation for BLDC motor 1
		_nBldcRequreStep1 = _bldc1.calcRequireStepCount(90);
		_nBldcStep1 = -_nBldcStep1;
	}
	
	if (_nBldcRequreStep2 == 0)
	{
		//180 degree rotation for BLDC motor 1
		_nBldcRequreStep2 = _bldc2.calcRequireStepCount(180);
		_nBldcStep2 = -_nBldcStep2;
	}
	
	if (_nBldcRequreStep3 == 0)
	{
		//360 degree rotation for BLDC motor 1
		_nBldcRequreStep3 = _bldc3.calcRequireStepCount(360);
		_nBldcStep3 = -_nBldcStep3;
	}

	_bldc1.DoRotate(_nBldcStep1);
	_bldc2.DoRotate(_nBldcStep2);
	_bldc3.DoRotate(_nBldcStep3);

	_nBldcRequreStep1--;
	_nBldcRequreStep2--;
	_nBldcRequreStep3--;

	delayMicroseconds(3000);
}


