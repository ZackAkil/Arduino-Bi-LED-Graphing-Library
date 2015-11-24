/*
 MyGDisplay.cpp - Library for displaying on a My-G led array.
  Created by Zackary Akil, August 26, 2014.
  Copyright (C) 2014 Zackary Akil.
*/
  
#ifndef MyGDisplay_h
#define MyGDisplay_h

#include "Arduino.h"
#include "ShiftRegister.h"

class MyGDisplay
{
	public: 
	MyGDisplay(int dataInPin, int latchPin, int clockPin, int outputSize);
	void drawBargraph(boolean fromCenter, boolean moveRight, int colour, int value);
	void displayLeds(int leds[]);
	void displayBits(boolean bitsToDisplay[]);
	void displayLeds();
	int ledsLeftToWrite();
	void allRed();
	void allGreen();
	void allOff();
	void allRandom();
	void pushRed();
	void pushGreen();
	void pushOff();
	void pushRandom();
	void pushAllRemainderRed();
	void pushAllRemainderGreen();
	void pushAllRemainderOff();
	void pushAllRemainderRandom();
	void enableMarker();
	void disableMarker();
	void setMarkerPos(int markerPos, int colour);
	void setLedAt(int pos, int colour);
	void enableFlashing();
	void disableFlashing();
	void setFlashRate(int rate);
	void pushFlash(int colour, boolean invert);
	void pushColour(int colour, boolean invert = false);

	private:
	void pushBit(boolean bitToAdd);
	void latchBits();
	void boundaryCheckPos();
	void movePos();
	void movePosWithCheck();
	void setPosCenterMoveRight();
	void setPosCenterMoveLeft();
	void setPosEdgeMoveRight();
	void setPosEdgeMoveLeft();
	void makeBar(int colour, int value);
	int getRelativePos(int pos);
	void flashLedsOverMarker(int val, int tempColour = 0);
	void drawLedsOverMarker(int val);

	int _outputSize;
	int _pos;
	int _markerPos;
	int _tempPos;
	int _flashRate; // in millie seconds of duration of on/off segment
	int _markerColour;
	boolean _posMoveRight;
    boolean _posCenter;
    boolean _markerEnable;
    boolean _flashEnable;
    boolean _bits[64]; // limits the number of output pins from Shift registers to 64 (increase if required)

    ShiftRegister _display;
};

#endif