#include <Wire.h>

#define SLAVE_ADDRESS 0x04
#include "Adafruit_HT1632.h"
#define HT_DATA 2
#define HT_WR   3
#define HT_CS   4
#define HT_CS2  5
Adafruit_HT1632LEDMatrix matrix = Adafruit_HT1632LEDMatrix(HT_DATA, HT_WR, HT_CS, HT_CS2);
#define LED  13
int number = 0;
int LED1  = 8;
int LED2 = 9;
int LED3 = 10;
int BUTTON = 11;
int BUTTON2 = 12;
int option = 0;

void setup() {

  pinMode(LED, OUTPUT);

  Serial.begin(9600);

  Wire.begin(SLAVE_ADDRESS);

  Wire.onReceive(receiveData);

  Wire.onRequest(sendData);

  Serial.println("Ready!");

  Serial.begin(9600);
  matrix.begin(ADA_HT1632_COMMON_16NMOS);
  matrix.fillScreen();
  delay(500);
  matrix.clearScreen();
  matrix.setTextWrap(false);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(BUTTON, INPUT);
  pinMode(BUTTON2, INPUT);
  option = 1;


}
bool Button1_Press()
{
  if (digitalRead(BUTTON) == HIGH && digitalRead(BUTTON2) == LOW)
  {
    option++ ;
    delay(200);
    return true;
  }
  return false;
}
bool Button2_Press()
{
  if (digitalRead(BUTTON2) == HIGH && digitalRead(BUTTON) == LOW)
  {
    option-- ;
    delay(200);
    return true;
  }
  return false;
}
void loop() {

  delay(100);
  if (number == 1)
  {
    if (digitalRead(BUTTON) == HIGH && digitalRead(BUTTON2) == LOW)
    {
      option++ ;
      delay(200);
    }
    if (digitalRead(BUTTON2) == HIGH && digitalRead(BUTTON) == LOW)
    {
      option-- ;
      delay(200);
    }
    if (option == 1)
    {
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      Serial.println(option);
      matrix.clearScreen();
      matrix.setCursor(0, 5);
      matrix.print("Hi Owner");
      matrix.writeScreen();

    }

    if (option == 3)
    {
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, HIGH);
      for (uint8_t i = 0; i < 4; i++)
      {
        if (Button2_Press())
        {
          goto endofloop;
        }
        matrix.setRotation(i);
        matrix.clearScreen();
        matrix.setCursor(0, 0);
        matrix.print("Hi");
        matrix.writeScreen();
        delay(1000);
        for (uint8_t y = 0; y < matrix.height(); y++)
        {
          if (Button2_Press())
          {
            goto endofloop;
          }
          for (uint8_t x = 0; x < matrix.width(); x++)
          {
            if (Button2_Press())
            {
              goto endofloop;
            }
            matrix.setPixel(x, y);
            matrix.writeScreen();
          }

        }
        // blink!
        matrix.blink(true);
        delay(2000);
        matrix.blink(false);

        // Adjust the brightness down
        for (int8_t i = 15; i >= 0; i--)
        {
          if (Button2_Press())
          {
            goto endofloop;
          }
          matrix.setBrightness(i);
          delay(100);
        }
        // then back up
        for (uint8_t i = 0; i < 16; i++)
        {
          if (Button2_Press())
          {
            goto endofloop;
          }
          matrix.setBrightness(i);
          delay(100);
        }

        for (uint8_t y = 0; y < matrix.height(); y++)
        {
          if (Button2_Press())
          {
            goto endofloop;
          }
          for (uint8_t x = 0; x < matrix.width(); x++)
          {
            if (Button2_Press())
            {
              goto endofloop;
            }
            matrix.clrPixel(x, y);
            matrix.writeScreen();
          }
        }
      }
endofloop:;
      Serial.println(option);
    }
    if (option == 2)
    {
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, LOW);
      Serial.println(option);
      matrix.clearScreen();
      matrix.setCursor(0, 5);
      matrix.print("CHEEEERS");
      matrix.writeScreen();
    }
    if (option > 3)
    {
      option = 3;
    }
    if (option < 1)
    {
      option = 1;
    }

  }
  else if (number == 2)
  {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    matrix.clearScreen();
    matrix.setCursor(4, 5);
    matrix.print("Welcome!");
    matrix.writeScreen();
    delay(2000);
  }
  else if (number == 0)
  {
    matrix.clearScreen();
    matrix.setCursor(0, 5);
    matrix.print("PARTYPAC");
    matrix.writeScreen();
    delay(2000);
  }
  else if(number == 4)
  {
    matrix.clearScreen();
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    for (int i= -90; i < 48 ; i++)
    {
      matrix.clearScreen();
      matrix.setCursor(i, 5);
      matrix.print("ACCESS DENIED");
      matrix.writeScreen();
      delay(50);
    }
  }
  else
  {
    matrix.clearScreen();
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
  }

}

void receiveData(int byteCount) {

  Serial.print("receiveData");

  while (Wire.available()) {

    number = Wire.read();

    Serial.print("data received: ");

    Serial.println(number);

    if (number == 1) {

      Serial.println(" LED ON");

      digitalWrite(LED, HIGH);

    } else {

      Serial.println(" LED OFF");

      digitalWrite(LED, LOW);

    }

  }

}

void sendData() {

  Wire.write(number);

}
