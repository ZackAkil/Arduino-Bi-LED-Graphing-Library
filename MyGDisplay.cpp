/*
 MyGDisplay.cpp - Library for displaying on a My-G led array.
  Created by Zackary Akil, August 26, 2014.
  Copyright (C) 2014 Zackary Akil.
*/

#include "Arduino.h"
#include "ShiftRegister.h"
#include "MyGDisplay.h"

  MyGDisplay::MyGDisplay(int dataInPin, int latchPin, int clockPin, int outputSize) 
  : _display (dataInPin,latchPin,clockPin,outputSize)
  {
    _outputSize = outputSize;
    _pos = 0;
    _markerPos = 0;
    _posMoveRight = true;
    _posCenter = false;
    _markerEnable = false;
    _flashEnable = false;
    _flashRate = 100;
    _markerColour = 2; // default red
  }

/*
draw a bar graph based on passed varibles indecating
 the starting position, growth direction, colour and value
*/
 void MyGDisplay::drawBargraph(boolean fromCenter, boolean moveRight, int colour, int value){ 

  if(fromCenter){
    if(moveRight)
      setPosCenterMoveRight();
    else
      setPosCenterMoveLeft();
  }else{
    if(moveRight)
      setPosEdgeMoveRight();
    else
      setPosEdgeMoveLeft();
  }

  makeBar(colour, value);

  pushAllRemainderOff();

  if(_markerEnable){
    setLedAt(_markerPos, _markerColour);
    drawLedsOverMarker(value);
    if(_flashEnable)
      flashLedsOverMarker(value);
  }

  displayLeds();
}

/*
make the bar in memory based on colour and value passed,
also inverts colours if the position of them passes the marker 
whilsts the marker is enabled
*/
void MyGDisplay::makeBar(int colour, int value){

  for (int i = 0; i < value; i++){
      pushColour(colour);
  }
}

/*
provided that the code is looped frequently this function
will flash the pushed led on and off 
*/
void MyGDisplay::pushFlash(int colour, boolean invert){

  if((millis()%(_flashRate*2))>_flashRate)
    pushColour(colour, invert); //
  else
    pushOff();
}

/*
flashes leds that are passed the marker, the tempColour argument 
has a default value of 0 (off) 
*/
void MyGDisplay::flashLedsOverMarker(int val, int tempColour){

  if((millis()%(_flashRate*2))>_flashRate)
    tempColour = _markerColour;

  for(int i = 0; i <= val - _markerPos; i++){
    setLedAt(_markerPos + i, tempColour);
  }
}

/*
push marker coloured leds that are passed the marker
*/
void MyGDisplay::drawLedsOverMarker(int val){

  for(int i = 0; i <= val - _markerPos; i++){
    setLedAt(_markerPos + i, _markerColour);
  }
}


/*
display leds based on array of ints that corilate to
 the led's colour
*/
 void MyGDisplay::displayLeds(int leds[]){ 

  setPosEdgeMoveRight();

  for(int i = 0; i < _outputSize/2; i++){
    pushColour(leds[i]);
  }
  displayLeds();
}

/*
display leds using the predefine array of bits that
 corilate to the output states of the shift registers
*/
 void MyGDisplay::displayLeds(){ 

  _display.displayBits(_bits);
}

/*
display leds using the predefine array of bits that
 corilate to the output states of the shift registers
*/
 void MyGDisplay::displayBits(boolean bitsToDisplay[]){ 

  _display.displayBits(bitsToDisplay);
}


void MyGDisplay::allRed(){

  setPosEdgeMoveRight();
  pushAllRemainderRed();
  displayLeds();
}

void MyGDisplay::allGreen(){

  setPosEdgeMoveRight();
  pushAllRemainderGreen();
  displayLeds();
}

void MyGDisplay::allOff(){

  setPosEdgeMoveRight();
  pushAllRemainderOff();
  displayLeds();
}

void MyGDisplay::allRandom(){

  setPosEdgeMoveRight();
  pushAllRemainderRandom();
  displayLeds();
}

void MyGDisplay::pushRed(){

  if(_posMoveRight){
    _bits[_pos] = 0;
    movePos();
    _bits[_pos] = 1;
    movePos();
  }else{
    _bits[_pos] = 1;
    movePos();
    _bits[_pos] = 0;
    movePos();
  }
}

void MyGDisplay::pushGreen(){

  if(_posMoveRight){
    _bits[_pos] = 1;
    movePos();
    _bits[_pos] = 0;
    movePos();
  }else{
    _bits[_pos] = 0;
    movePos();
    _bits[_pos] = 1;
    movePos();
  }
}

void MyGDisplay::pushOff(){

  _bits[_pos] = 0;
  movePos();
  _bits[_pos] = 0;
  movePos();
}

void MyGDisplay::pushRandom(){

  pushColour(random(0, 3));
}

void MyGDisplay::pushAllRemainderRed(){

  while(ledsLeftToWrite() > 0){
   pushRed(); 
 }
}

void MyGDisplay::pushAllRemainderGreen(){

  while(ledsLeftToWrite() > 0){
   pushGreen(); 
 }
}

void MyGDisplay::pushAllRemainderOff(){

  while(ledsLeftToWrite() > 0){
   pushOff(); 
 }
}

void MyGDisplay::pushAllRemainderRandom(){

  while(ledsLeftToWrite() > 0){
   pushRandom(); 
 }
}

void MyGDisplay::movePos(){

  if (_posMoveRight)
    _pos ++;
  else
    _pos --;
}

void MyGDisplay::movePosWithCheck(){

  movePos();
  boundaryCheckPos();
}

/*
catch the pos when it hits the led array boundary and
 resets to starting pos
*/
 void MyGDisplay::boundaryCheckPos(){

  if ((_pos < 0) || (_pos >= _outputSize)){
    if(_posCenter){
      if(_posMoveRight)
        _pos = _outputSize / 2;
      else
        _pos = (_outputSize / 2) - 1;
    }else{
      if(_posMoveRight)
        _pos = 0;
      else
        _pos = _outputSize - 1;
    }
  }
}

/*
 returns the number of leds that are left to write over in 
 order to have the current scope of leds all up-to-date
*/
 int MyGDisplay::ledsLeftToWrite(){ 

  if(_posMoveRight)
    return (_outputSize - _pos)/2;
  else
    return (_outputSize - (_outputSize - _pos - 1)) / 2;
}

void MyGDisplay::setPosCenterMoveRight(){

  _posCenter = true;
  _posMoveRight = true;
  _pos = _outputSize/2;
}

void MyGDisplay::setPosCenterMoveLeft(){

  _posCenter = true;
  _posMoveRight = false;
  _pos = (_outputSize/2) - 1;
}
void MyGDisplay::setPosEdgeMoveRight(){

  _posCenter = false;
  _posMoveRight = true;
  _pos = 0;
}

void MyGDisplay::setPosEdgeMoveLeft(){

  _posCenter = false;
  _posMoveRight = false;
  _pos = _outputSize - 1;
}

void MyGDisplay::enableMarker(){

  _markerEnable = true;
}

void MyGDisplay::disableMarker(){

  _markerEnable = false;
}

void MyGDisplay::setMarkerPos(int markerPos, int colour){

  _markerPos = markerPos;
  _markerColour = colour;
}

/*
set the colour of an led independant of the current position (_pos)
*/
void MyGDisplay::setLedAt(int pos, int colour){ //colour: 0 = off, 1 = green, 2 = red

  _tempPos = _pos; // use to hold position temporarily 
  _pos = getRelativePos(pos); //allow user to pass to number of the led and not the output pin starting from 1 (instead of index 0)
  pushColour(colour);
  _pos = _tempPos; // reset _pos to original position
}

/*
returns the relative position of the led position passed based on 
the current direction on origin point of the drawing mechanism
*/
int MyGDisplay::getRelativePos(int pos){

  pos = (pos-1)*2;
  if(_posMoveRight){
    if(_posCenter)
      pos = pos + (_outputSize/2);
  }else{
    if(_posCenter)
      pos = (_outputSize/2) - pos -1;
    else
      pos = _outputSize - pos -1;
  }
  
  return pos;
}

void MyGDisplay::enableFlashing(){
  _flashEnable = true;
}

void MyGDisplay::disableFlashing(){
  _flashEnable = false;
}

void MyGDisplay::setFlashRate(int rate){
  _flashRate = rate;
}

void MyGDisplay::pushColour(int colour, boolean invert){

    switch (colour){ 
    case 0:
      pushOff();
    break;
    
    case 1:
      if(invert)
        pushRed();
      else
        pushGreen();
    break;
    
    case 2:
      if(invert)
        pushGreen();
      else
        pushRed();
    break;

    case 3:
      pushRandom();
    break;
  }

}
