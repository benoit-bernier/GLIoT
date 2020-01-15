#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN   52
#define DATA_PIN  51
#define CS_PIN    53

// HARDWARE SPI
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// Scrolling parameters
#if USE_UI_CONTROL
const uint8_t SPEED_IN = A5;
const uint8_t DIRECTION_SET = 8;  // change the effect
const uint8_t INVERT_SET = 9;     // change the invert

const uint8_t SPEED_DEADBAND = 5;
#endif // USE_UI_CONTROL

uint8_t scrollSpeed = 15;    // default frame delay value
uint16_t scrollPause = 250; // in milliseconds
textEffect_t scrollleft = PA_SCROLL_LEFT;
textEffect_t scrollright = PA_SCROLL_RIGHT;
textPosition_t scrollAlign = PA_LEFT;

// Global message buffers shared by Serial and Scrolling functions
#define	BUF_SIZE	75
char curMessage[BUF_SIZE] = {""};
char TURN_LEFT[BUF_SIZE] = {"<< <<"};
char TURN_RIGHT[BUF_SIZE] = {">> >>"};
bool newMessageAvailable = true;

void readSerial(void)
{
  static char *cp = TURN_LEFT;

  while (Serial.available())
  {
    *cp = (char)Serial.read();
    if ((*cp == '\n') || (cp - TURN_LEFT >= BUF_SIZE-2)) // end of message character or full buffer
    {
      *cp = '\0'; // end the string
      // restart the index for next filling spree and flag we have a message waiting
      cp = TURN_LEFT;
      newMessageAvailable = true;
    }
    else  // move char pointer to next position
      cp++;
  }
}

void setup()
{
  Serial.begin(57600);
  Serial.print("\n[Parola Scrolling Display]\nType a message for the scrolling display\nEnd message line with a newline");
  P.begin();
  P.displayText(curMessage, scrollAlign, scrollSpeed, scrollPause, scrollleft, scrollright);
}

char d[BUF_SIZE]=TURN_LEFT ;
void loop()
{
  if (P.displayAnimate())
  {
    if (newMessageAvailable)
    {
      strcpy(curMessage,d );
      newMessageAvailable = false;
    }
    P.displayReset();
  }
  readSerial();

  d= TURN_RIGHT;

}
