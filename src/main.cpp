/* output freq calc:
 * fOut = (fclk / 2 * prescaler * ICR1)
 * fout * ICR1 = fclk / 2 * prescaler
 * ICR1 = fclk / 2 * prescaler * fout
 * Accuracy of maths = +- 0.01 Hz
 * Accuracy of hardware = +- 0.1 Hz
 * 
 * 
 */



#include <Arduino.h>
#include "Adafruit_SSD1306.h"

#define TONE_OUT 9
#define POT_IN A0

#define PWM_PRESCALER 8
#define CPU_FREQUENCY 16000000

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET 4
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



void OutputTone(float frequency);
void UpdateLCD(float frequency);
int ReadPot();

float const lookupArray[] = {67,69.3,71.9,74.4,77,79.7,82.5,85.4,88.5,91.5,94.8,97.4,100,103.5,107.2,110.9,114.8,118.8,123,127.3,131.8,136.5,141.3,146.2,151.4,156.7,162.2,167.9,173.8,179.9,186.2,192.8,203.5,210.7,218.1,225.7,233.6,241.8,250.3};


void setup()
{
    pinMode(TONE_OUT, OUTPUT);
    pinMode(POT_IN, INPUT);
    pinMode(13, OUTPUT);

    TCCR1A=_BV(COM1A1)|_BV(COM1B1); // set Fast PWM Mode
    TCCR1B=_BV(WGM13)|_BV(CS11);    // Activate PWM Phase, frequency correction Mode
    
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        for(;;);
    }
    
    display.display();
    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.cp437(true);
    display.print(F("TU8K"));
    display.display();
    delay(1000);

}

void loop()
{
    int choice = ReadPot();
    float frequency = lookupArray[choice];
    OutputTone(frequency);
    UpdateLCD(frequency);
}

int ReadPot()
{
    int rawData = analogRead(POT_IN);
    int choice = map(rawData, 0, 1024, 0, 39);
    return choice;
}

void OutputTone(float frequency)
{ 
    digitalWrite(13, HIGH);
    float timerVal = (CPU_FREQUENCY) / (2 * PWM_PRESCALER * frequency);
    long int timerValInt = timerVal + 0.5;
    ICR1 = timerValInt;
    OCR1A = timerValInt / 2;
    digitalWrite(13, LOW);
}

void UpdateLCD(float frequency)
{
    display.clearDisplay();
    display.setCursor(0,0);
    display.print(frequency, 1);
    display.display();
}