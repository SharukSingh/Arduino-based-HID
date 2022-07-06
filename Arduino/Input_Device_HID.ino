/*
 *
 * Program to function as a USB input device.
 * Communicate with companion python program to read
 * and display volume level.
 *
 * Ver 2.2 Using encoder library
 * Ver 2.3 Showing volume as bar graph
 * Ver 2.4 Adding buttons
 *
 * Jul 2022
 * Sharuk
 *
 */

#include "settings.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "HID-Project.h"
#include <Encoder.h>

Encoder myEnc(RE_DATA0, RE_DATA1);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()
{
  // Setup led
  pinMode(output_led, OUTPUT);

  // Setup switches
  pinMode(RE_SWITCH, INPUT_PULLUP);

  Serial.begin(BAUD_RATE);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }

  // Initialize display
  display.display();
  display.clearDisplay();

  // Initialize HID
  Consumer.begin();
}

long oldPosition = -999;
int volume = 50;
String msg;

void updateValue(int delta)
{
  volume = constrain(volume + delta, 0, 100);
}

void updateDisplay()
{
  display.clearDisplay();

  display.setTextColor(1);
  display.setCursor(42, 2);
  display.print("Volume");

  display.drawRoundRect(10, 12, 102, 9, 2, WHITE);
  display.fillRect(11, 13, volume, 7, WHITE);

  display.setCursor(20, 32);
  display.print("Undo");

  display.setCursor(74, 32);
  display.print("Redo");

  display.setCursor(20, 52);
  display.print("Play");

  display.setCursor(74, 52);
  display.print("Pause");

  display.display();
}

void blinkLed()
{
  digitalWrite(output_led, HIGH);
  delay(1000);
  digitalWrite(output_led, LOW);
  delay(1000);
}

void displayMessage(int message)
{
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Vol:");
  display.print(message);
  display.display();
}

void handleEncoder()
{
  long newPosition = myEnc.read();
  if (newPosition != oldPosition)
  {
    if ((newPosition - oldPosition) > 0)
    {
      Serial.println("clockwise");
      Consumer.write(MEDIA_VOLUME_UP);
    }
    else
    {
      Serial.println("anti-clockwise");
      Consumer.write(MEDIA_VOLUME_DOWN);
    }
    oldPosition = newPosition;
    Serial.println(newPosition);
  }
}

void loop()
{
  handleEncoder();

  if (digitalRead(RE_SWITCH) == LOW)
  {
    digitalWrite(output_led, HIGH);
    Serial.println("switch");
  }

  if (Serial.available() > 0)
  {
    msg = Serial.readStringUntil('\n');
    volume = msg.toInt();
    updateDisplay();
  }
}
