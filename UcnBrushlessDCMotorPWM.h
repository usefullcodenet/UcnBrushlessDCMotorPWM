//////////////////////////////////////////////////////////////////////////
//
// UcnBrushlessDCMotorPWM
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

#ifndef UcnBrushlessDCMotorPWM_h
#define UcnBrushlessDCMotorPWM_h




class UcnBrushlessDCMotorPWM
{
protected:

	const uint8_t PWM_MAX = 255;
	const uint8_t PWM_MIN = 0;

	uint16_t _nSinTableSize;

	int16_t _nPhaseIndex1;
	int16_t _nPhaseIndex2;
	int16_t _nPhaseIndex3;

	uint8_t* _pnSinTable;

	uint8_t _nPower = 100;

	volatile bool _bAbortLoop;

	uint8_t _nMotorPoleCount;

	uint8_t _nPinIn1;
	uint8_t _nPinIn2;
	uint8_t _nPinIn3;

#if defined(ESP32)
	uint8_t _nChannelIn1_eps32 = 0;
	uint8_t _nChannelIn1_eps32 = 1;
	uint8_t _nChannelIn1_eps32 = 2;
#endif

	int8_t _nPinEn1;
	int8_t _nPinEn2;
	int8_t _nPinEn3;



	virtual void PrepareSinTable(uint16_t nSinTableSize)
	{
		if (nSinTableSize == 0)
			return;

		_nSinTableSize = nSinTableSize;

		double rad = 0;
		double diff = 2.0 * PI / _nSinTableSize / 1;
		int16_t tmp;

		if (_pnSinTable != NULL)
			delete _pnSinTable;

		_pnSinTable = new uint8_t[_nSinTableSize];
		for (int i = 0; i < _nSinTableSize; i++)
		{
			// degree: 0 to 360,  value: PWM_MIN to PWM_MAX
			tmp = (int)((sin(rad) + 1.0) * (PWM_MAX - PWM_MIN) / 2) + PWM_MIN;
			if (tmp < PWM_MIN)
				tmp = PWM_MIN;
			if (tmp > PWM_MAX)
				tmp = PWM_MAX;

			_pnSinTable[i] = tmp;

			rad += diff;
		}

		_nPhaseIndex1 = 0;
		_nPhaseIndex2 = _nSinTableSize / 3;
		_nPhaseIndex3 =  _nPhaseIndex2 + _nPhaseIndex2;
	}



	// 0 - (_nSinTableSize-1)
	virtual inline uint8_t GetSinTableValue(int16_t nIndex) const
	{
		if (_nPower >= 100)
			return _pnSinTable[nIndex];

		uint16_t tmp = _nPower;
		tmp *= _pnSinTable[nIndex];
		tmp /= 100;

		if (tmp > PWM_MAX)
			tmp = PWM_MAX;

		return tmp;
	}


	virtual void setupPWM(uint32_t nPwmFreq = 0)
	{
#if defined(ARDUINO_ARCH_AVR)

		//nPwmFreq is ignored, use 31.37KHz

		if (_nPinIn1 == 5 || _nPinIn1 == 6 || _nPinIn2 == 5 || _nPinIn2 == 6 || _nPinIn3 == 5 || _nPinIn3 == 6)
		{
			// timer0
			// 5pin and 6pin PWM to 31.37KHz
			TCCR0B &= B11111000;
			TCCR0B |= B00000001;
		}

		if (_nPinIn1 == 9 || _nPinIn1 == 10 || _nPinIn2 == 9 || _nPinIn2 == 10 || _nPinIn3 == 9 || _nPinIn3 == 10)
		{
			// timer1
			// 9pin and 10pin PWM to 31.37KHz
			TCCR1B &= B11111000;
			TCCR1B |= B00000001;
		}

		if (_nPinIn1 == 3 || _nPinIn1 == 11 || _nPinIn2 == 3 || _nPinIn2 == 11 || _nPinIn3 == 3 || _nPinIn3 == 11)
		{
			// timer2
			// 3pin and 11pin PWM to 31.37KHz
			TCCR2B &= B11111000;
			TCCR2B |= B00000001;
		}

		pinMode(_nPinIn1, OUTPUT);
		pinMode(_nPinIn2, OUTPUT);
		pinMode(_nPinIn3, OUTPUT);
#endif

#if defined(ESP32)

	if (nPwmFreq == 0)
		nPwmFreq = 48000;				//default 48KHz PWM

	ledcSetup(_nPinIn1, nPwmFreq, 8);	//8bit PWM
	ledcSetup(_nPinIn2, nPwmFreq, 8);	//8bit PWM
	ledcSetup(_nPinIn3, nPwmFreq, 8);	//8bit PWM

	ledcAttachPin(_nPinIn1, _nChannelIn1_eps32);
	ledcAttachPin(_nPinIn2, _nChannelIn2_eps32);
	ledcAttachPin(_nPinIn3, _nChannelIn3_eps32);
#endif

#if defined(ESP8266)

	if (nPwmFreq == 0)
		nPwmFreq = 10000;		//default 10KHz PWM (high freq cause invalid PWM pulse)

	analogWriteRange(255);		//8bit PWM
	analogWriteFreq(nPwmFreq);
#endif

#if defined(ARDUINO_ARCH_STM32)

		if (nPwmFreq == 0)
			nPwmFreq = 100000;			//default 100KHz PWM
		
		analogWriteResolution(8);		//8bit PWM
		analogWriteFrequency(nPwmFreq);
#endif
		
	}




	virtual inline void WritePwmValue(uint8_t nPinOrChannel, uint8_t nValue) const
	{
#if defined(ESP32)
		ledcWrite(nPinOrChannel, nValue);
#else
		analogWrite(nPinOrChannel, nValue);
#endif
	}


	virtual inline void WritePwm(uint8_t nPinOrChannel, uint16_t nPhaseIndex) const
	{
		WritePwmValue(nPinOrChannel, GetSinTableValue(nPhaseIndex));
	}

	

	virtual inline void WritePwm123(void) const
	{
#if defined(ESP32)
		WritePwm(_nChannelIn1_eps32, _nPhaseIndex1);
		WritePwm(_nChannelIn2_eps32, _nPhaseIndex2);
		WritePwm(_nChannelIn3_eps32, _nPhaseIndex3);
#else
		WritePwm(_nPinIn1, _nPhaseIndex1);
		WritePwm(_nPinIn2, _nPhaseIndex2);
		WritePwm(_nPinIn3, _nPhaseIndex3);
#endif
	}





public:

	virtual void suspend(uint8_t value = 0)
	{
#if defined(ESP32)
		WritePwmValue(_nChannelIn1_eps32, value);
		WritePwmValue(_nChannelIn2_eps32, value);
		WritePwmValue(_nChannelIn3_eps32, value);
#else
		WritePwmValue(_nPinIn1, value);
		WritePwmValue(_nPinIn2, value);
		WritePwmValue(_nPinIn3, value);
#endif
	}

	
	


#if defined(ESP32)
	UcnBrushlessDCMotorPWM(uint8_t nChannelIn1_eps32 = 0, uint8_t nChannelIn2_eps32 = 1, uint8_t nChannelIn3_eps32 = 2)
#else
	UcnBrushlessDCMotorPWM()
#endif
	{
		_pnSinTable = NULL;
		_bAbortLoop = false;
		_nSinTableSize = 200;

		_nPinEn1 = -1;
		_nPinEn2 = -1;
		_nPinEn3 = -1;

#if defined(ESP32)
		_nChannelIn1_eps32 = nChannelIn1_eps32;
		_nChannelIn2_eps32 = nChannelIn2_eps32;
		_nChannelIn3_eps32 = nChannelIn3_eps32;
#endif
	}


	virtual ~UcnBrushlessDCMotorPWM()
	{
		end();
	}


	virtual void begin(uint8_t nMotorPoleCount, uint8_t nPinIn1, uint8_t nPinIn2, uint8_t nPinIn3, int8_t nPinEn1 = -1, int8_t nPinEn2 = -1, int8_t nPinEn3 = -1, uint16_t nSinTableSize = 200, uint32_t nPwmFreq = 0)
	{
		_nPinIn1 = nPinIn1;
		_nPinIn2 = nPinIn2;
		_nPinIn3 = nPinIn3;

		_nPinEn1 = nPinEn1;
		_nPinEn2 = nPinEn2;
		_nPinEn3 = nPinEn3;

		_nMotorPoleCount = nMotorPoleCount;

		PrepareSinTable(nSinTableSize);

		setupPWM(nPwmFreq);

		// set ENABLE1 pin to HIGH
		if(nPinEn1 >= 0)
		{
			pinMode(nPinEn1, OUTPUT);
			digitalWrite(nPinEn1, HIGH);
		}

		// set ENABLE2 pin to HIGH
		if(nPinEn2 >= 0)
		{
			pinMode(nPinEn2, OUTPUT);
			digitalWrite(nPinEn2, HIGH);
		}

		// set ENABLE3 pin to HIGH
		if(nPinEn3 >= 0)
		{
			pinMode(nPinEn3, OUTPUT);
			digitalWrite(nPinEn3, HIGH);
		}
	}


	virtual void end(void)
	{
		// set ENABLE1 pin to LOW
		if(_nPinEn1 >= 0)
		{
			digitalWrite(_nPinEn1, LOW);
		}

		// set ENABLE2 pin to LOW
		if(_nPinEn2 >= 0)
		{
			digitalWrite(_nPinEn2, LOW);
		}

		// set ENABLE3 pin to LOW
		if(_nPinEn3 >= 0)
		{
			digitalWrite(_nPinEn3, LOW);
		}

		if (_pnSinTable)
			delete _pnSinTable;
		_pnSinTable = NULL;
	}



	// power: 0 to 100 (%)
	void SetPower(uint8_t nPower)
	{
		if (nPower > 100)
			nPower = 100; 
		_nPower = nPower;
	}


	virtual void SetAbortFlag(void)
	{
		_bAbortLoop = true;
	}

	virtual void ClearAbortFlag(void)
	{
		_bAbortLoop = false;
	}





	
	
	virtual inline void DoRotate(int16_t nStep = 1)
	{
		WritePwm123();

		_nPhaseIndex1 += nStep;
		_nPhaseIndex2 += nStep;
		_nPhaseIndex3 += nStep;

		if (nStep > 0)
		{
			if (_nPhaseIndex1 >= _nSinTableSize)
				_nPhaseIndex1 = 0;
			if (_nPhaseIndex2 >= _nSinTableSize)
				_nPhaseIndex2 = 0;
			if (_nPhaseIndex3 >= _nSinTableSize)
				_nPhaseIndex3 = 0;
		}
		else
		{
			if (_nPhaseIndex1 < 0)
				_nPhaseIndex1 = _nSinTableSize - 1;
			if (_nPhaseIndex2 < 0)
				_nPhaseIndex2 = _nSinTableSize - 1;
			if (_nPhaseIndex3 < 0)
				_nPhaseIndex3 = _nSinTableSize - 1;
		}
	}




	inline uint32_t calcRequireStepCount(int16_t nRotateDegree)
	{
		//360degree rotate need ((_nMotorPoleCount + 1) / 2.0 * _nSinTableSize) steps
		return (uint32_t)((double)nRotateDegree / 360.0 * (_nMotorPoleCount + 1) / 2.0 * _nSinTableSize);
	}


	void DoRotateLoop(int16_t nRotateDegree, uint16_t nDelayMicroSec, int8_t nStep = 1)
	{
		if (_pnSinTable == NULL)
			return;

		if (nRotateDegree < 0)
		{
			nRotateDegree = -nRotateDegree;
			nStep = -nStep;
		}
		uint8_t nStepAbs = abs(nStep);

		uint32_t nRequireStep = calcRequireStepCount(nRotateDegree);

		while (_bAbortLoop == false)
		{
			DoRotate(nStep);

			nRequireStep -= nStepAbs;
			if (nRequireStep == 0)
				break;

			delayMicroseconds(nDelayMicroSec);
		}
	}

};


#endif
