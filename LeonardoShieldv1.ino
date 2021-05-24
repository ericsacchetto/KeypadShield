#include <Keypad.h>
#include "HID-Project.h"

// Rotary Encoder Inputs
#define Clock 5   //Clock pin connected to D9
#define Data 4    //Data pin connected to D8
#define Push 3   //Push button pin connected to D10
int counter = 0;                    //Use this variable to store "steps"
int currentStateClock;              //Store the status of the clock pin (HIGH or LOW)
int lastStateClock;                 //Store the PREVIOUS status of the clock pin (HIGH or LOW)
String currentDir ="";              //Use this to print text 
unsigned long lastButtonPress = 0;  //Use this to store if the push button was pressed or not


const byte ROWS = 3;
const byte COLS = 3;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  };

byte rowPins[ROWS] = {12, 11, 10}; 
byte colPins[COLS] = {9, 8, 7}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  //while(!SerialUSB);
  Serial.begin(9600);
  Consumer.begin();

  pinMode(Clock,INPUT_PULLUP);
  pinMode(Data,INPUT_PULLUP);
  pinMode(Push, INPUT_PULLUP);
  lastStateClock = digitalRead(Clock);
}

void OpenApp(String appAddress)
{
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_ESC);
  delay(100);
  Keyboard.releaseAll();
  Keyboard.print(appAddress);
  delay(100);
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();
}
  
void loop(){
  char customKey = customKeypad.getKey();
  currentStateClock = digitalRead(Clock);

  if (currentStateClock != lastStateClock  && currentStateClock == 1)
  {
    if (digitalRead(Data) != currentStateClock)
    {
      counter --;
      Consumer.write(MEDIA_VOLUME_DOWN);
    } 
    else 
    {
      // Encoder is rotating CW so increment
      counter ++;
      Consumer.write(MEDIA_VOLUME_UP);
    }
  }
  lastStateClock = currentStateClock;

  int btnState = digitalRead(Push);

  if (btnState == LOW) 
  {
    //if 50ms have passed since last LOW pulse, it means that the
    //button has been pressed, released and pressed again
    if (millis() - lastButtonPress > 50) {
      Consumer.write(MEDIA_VOLUME_MUTE);
    }

    // Remember last button press event
    lastButtonPress = millis();
  }

    
  switch (customKey) 
  {
  case '1':  
    Consumer.write(MEDIA_NEXT);
    break;
  case '2':
    Consumer.write(MEDIA_PLAY_PAUSE);
    break;
  case '3':
    Consumer.write(MEDIA_PREVIOUS);
    break;
  case '4':
    Consumer.write(MEDIA_VOLUME_DOWN);
    break;
  case '5':
    Consumer.write(MEDIA_VOLUME_MUTE);
    break;
  case '6':
    Consumer.write(MEDIA_VOLUME_UP);
    break;
  case '7':
    OpenApp("fireutils");
    break;
  case '8':
    OpenApp("firemap");
    break;
  case '9':
    OpenApp("spotify");
    break;
  }
}
