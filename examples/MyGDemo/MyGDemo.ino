#include <ShiftRegister.h> 
#include <MyGDisplay.h> 

MyGDisplay test (2,3,4,24);
boolean leds[24] = {
  1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0};
int val =0;
int marker = 0;
void setup() {
  Serial.begin(9600);


  test.displayBits(leds);
  delay(300);
  test.allRed();
  delay(300);
  test.allGreen();
  delay(300);
  test.allOff();
  delay(300);
  test.allRandom();
  delay(300);
  // test.allOff();
  //delay(300);
  test.enableMarker();
  test.setMarkerPos(marker,1);
  //test.enableFlashing();

}


void loop() {


  while(val < 12){
    test.drawBargraph(false, true,2, val);


    if((millis()%(200))==0)
      val ++;
  }
  while(val > 0){
    test.drawBargraph(false, true,2, val);

    if((millis()%(200))==0)
      val --;
  }

  test.setMarkerPos(marker++,1);

  if (marker >6)
    test.enableFlashing();

}

