/*
 * Braille Display
 * SD card to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 10
 * 
Tung Le 2018
*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <SD.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int PUL1 = 7; // Sliding
int DIR1 = 6; //
int ENA1 = 5; //
//
int PUL2 = 4; // Rotating
int DIR2 = 3; //
int ENA2 = 2; //
//
char inChar = 0;

File myFile;

const uint8_t chipSelect = 10;
const uint8_t inButton = 8;
uint8_t Nextword = 0;
uint8_t Rolling = 0;

char inputString[50];
char inputChar;
uint8_t stringIndex = 1;

char Column0[100];
char Column1[100];
uint8_t moveStep = 1;

char spacePos[10];
uint8_t spaceIndex = 1;
uint8_t lastIndex = 1;
uint8_t spaceMark = 1;

uint8_t Ready = 0;

void setup()
{
  pinMode (PUL1, OUTPUT);
  pinMode (DIR1, OUTPUT);
  pinMode (ENA1, OUTPUT);
  pinMode (PUL2, OUTPUT);
  pinMode (DIR2, OUTPUT);
  pinMode (ENA2, OUTPUT);
  pinMode(inButton, INPUT);
  //
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
  //
  lcd.init();
  //
  lcd.backlight();
  //
  lcd.setCursor(0, 0);
  lcd.print("Starting...");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Reading SD card");
  //
  if (!SD.begin(chipSelect))
  {
    Serial.println("Card failed, or not present");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("No SD card");
    //
    while (1);
  }
  Serial.println("card initialized.");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ready");
  lcd.setCursor(0, 1);
  //
  myFile = SD.open("test.txt");
  //
  if (myFile)
  {
    while (myFile.available())
    {
      inputChar = myFile.read();
      Serial.write(inputChar);

      //
      if (inputChar != '$')
      {
        inputString[stringIndex] = inputChar;
        lcd.print(inputString[stringIndex]);
        //
        if (inputChar == ' ')
        {
          spacePos[spaceIndex] = stringIndex;
          spaceIndex++;
        }
        //
        stringIndex++;
      }
      else
      {
        Serial.println(stringIndex);
      }

    }
    myFile.close();
  }
  //
  else
  {
    Serial.println("error opening datalog.txt");
  }
  //
  Column0[0] = 0;
  Column1[0] = 0;
}

void loop()
{
  //
  if (Serial.available())
  {
    inChar = (char)Serial.read();
    //
    if (inChar == 'F')
    {
      for (int i = 0; i < 800; i++) //Forward
      {
        digitalWrite(DIR1, LOW);
        digitalWrite(ENA1, HIGH);
        digitalWrite(PUL1, HIGH);
        delayMicroseconds(200);
        digitalWrite(PUL1, LOW);
        delayMicroseconds(200);
      }
      //
      inChar = 0;
    }
    //
    if (inChar == 'B')
    {
      for (int i = 0; i < 800; i++) //Backward
      {
        digitalWrite(DIR1, HIGH);
        digitalWrite(ENA1, HIGH);
        digitalWrite(PUL1, HIGH);
        delayMicroseconds(200);
        digitalWrite(PUL1, LOW);
        delayMicroseconds(200);
      }
      //
      inChar = 0;
    }
    //
    if (inChar == 'L')
    {
      for (int i = 0; i < 800; i++) // Rotate left
      {
        digitalWrite(DIR2, HIGH);
        digitalWrite(ENA2, HIGH);
        digitalWrite(PUL2, HIGH);
        delayMicroseconds(200);
        digitalWrite(PUL2, LOW);
        delayMicroseconds(200);
      }
      //
      inChar = 0;
    }
    //
    if (inChar == 'R')
    {
      for (int i = 0; i < 800; i++) // Rotate right
      {
        digitalWrite(DIR2, LOW);
        digitalWrite(ENA2, HIGH);
        digitalWrite(PUL2, HIGH);
        delayMicroseconds(200);
        digitalWrite(PUL2, LOW);
        delayMicroseconds(200);
      }
      //
      inChar = 0;
    }
    //
    //
    if (inChar == 'G')
    {
      Ready = 1;
      inChar = 0;
    }
  }
  //
  Nextword = digitalRead(inButton);
  //
  if (Nextword == 1)
  {
    Ready = 1;
  }
  //
  // spacemark gets 1 first, increase to spaceindex
  if ((Ready == 1) && (spaceMark <= spaceIndex))
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    //
    //Set zero here
    if (Rolling != 0)
    {
      lcd.print("Rolling back... ");
      //
      for (int j = moveStep; j > 1; j--)
      {
        //set back 1 step
        for (int i = 0; i < 4000; i++) //forward
        {
          digitalWrite(DIR1, LOW);
          digitalWrite(ENA1, HIGH);
          digitalWrite(PUL1, HIGH);
          delayMicroseconds(100);
          digitalWrite(PUL1, LOW);
          delayMicroseconds(100);
        }
        //
        moveStep--;
        //
        delay(200);
        // rotate column back to 0
        // column1 first
        if (Column1[moveStep] != Column0[moveStep])
        {
          if (Column1[moveStep] > Column0[moveStep])
          {
            for (int m = 0; m < (Column1[moveStep] - Column0[moveStep]); m++)
            {
              for (int i = 0; i < 800; i++) //Rotate right
              {
                digitalWrite(DIR2, LOW);
                digitalWrite(ENA2, HIGH);
                digitalWrite(PUL2, HIGH);
                delayMicroseconds(100);
                digitalWrite(PUL2, LOW);
                delayMicroseconds(100);
              }
            }
          }
          //
          else if (Column1[moveStep] < Column0[moveStep])
          {
            for (int m = 0; m < (Column0[moveStep] - Column1[moveStep]); m++)
            {
              for (int i = 0; i < 800; i++) //Rotate left
              {
                digitalWrite(DIR2, HIGH);
                digitalWrite(ENA2, HIGH);
                digitalWrite(PUL2, HIGH);
                delayMicroseconds(100);
                digitalWrite(PUL2, LOW);
                delayMicroseconds(100);
              }
            }
          }
        }
        //
        delay(200);
        //
        for (int i = 0; i < 4000; i++) //forward
        {
          digitalWrite(DIR1, LOW);
          digitalWrite(ENA1, HIGH);
          digitalWrite(PUL1, HIGH);
          delayMicroseconds(100);
          digitalWrite(PUL1, LOW);
          delayMicroseconds(100);
        }
        //
        delay(200);
        //
        if (Column0[moveStep] != Column1[moveStep - 1])
        {
          if (Column0[moveStep] > Column1[moveStep - 1])
          {
            for (int m = 0; m < (Column0[moveStep] - Column1[moveStep - 1]); m++)
            {
              for (int i = 0; i < 800; i++) //Rotate right
              {
                digitalWrite(DIR2, LOW);
                digitalWrite(ENA2, HIGH);
                digitalWrite(PUL2, HIGH);
                delayMicroseconds(100);
                digitalWrite(PUL2, LOW);
                delayMicroseconds(100);
              }
            }
          }
          else if (Column0[moveStep] < Column1[moveStep - 1])
          {
            for (int m = 0; m < (Column1[moveStep - 1] - Column0[moveStep]); m++)
            {
              for (int i = 0; i < 800; i++) //Rotate left
              {
                digitalWrite(DIR2, HIGH);
                digitalWrite(ENA2, HIGH);
                digitalWrite(PUL2, HIGH);
                delayMicroseconds(100);
                digitalWrite(PUL2, LOW);
                delayMicroseconds(100);
              }
            }
          }
        }
      }
      //
      Rolling = 0;
    }
    //
    lcd.setCursor(0, 0);
    lcd.print("Printing...     ");
    lcd.setCursor(0, 1);
    
    //
    for (int k = lastIndex;  k < spacePos[spaceMark]; k++)
    {
      lcd.print(inputString[lastIndex]);
      //
      switch (inputString[lastIndex])
      {
        case 'A':
          // decode
          Column0[moveStep] = 4;
          Column1[moveStep] = 0;
          break;
        case 'B':
          Column0[moveStep] = 6;
          Column1[moveStep] = 0;
          break;
        case 'C':
          Column0[moveStep] = 4;
          Column1[moveStep] = 4;
          break;
        case 'D':
          Column0[moveStep] = 4;
          Column1[moveStep] = 6;
          break;
        case 'E':
          Column0[moveStep] = 4;
          Column1[moveStep] = 2;
          break;
        case 'F':
          Column0[moveStep] = 6;
          Column1[moveStep] = 4;
          break;
        case 'G':
          Column0[moveStep] = 6;
          Column1[moveStep] = 6;
          break;
        case 'H':
          Column0[moveStep] = 6;
          Column1[moveStep] = 2;
          break;
        case 'I':
          Column0[moveStep] = 2;
          Column1[moveStep] = 4;
          break;
        case 'K':
          Column0[moveStep] = 5;
          Column1[moveStep] = 0;
          break;
        case 'L':
          Column0[moveStep] = 7;
          Column1[moveStep] = 0;
          break;
        case 'M':
          Column0[moveStep] = 5;
          Column1[moveStep] = 4;
          break;
        case 'N':
          Column0[moveStep] = 5;
          Column1[moveStep] = 6;
          break;
        case 'O':
          Column0[moveStep] = 5;
          Column1[moveStep] = 2;
          break;
        case 'P':
          Column0[moveStep] = 7;
          Column1[moveStep] = 4;
          break;
        case 'Q':
          Column0[moveStep] = 7;
          Column1[moveStep] = 6;
          break;
        case 'R':
          Column0[moveStep] = 7;
          Column1[moveStep] = 2;
          break;
        case 'S':
          Column0[moveStep] = 3;
          Column1[moveStep] = 4;
          break;
        case 'T':
          Column0[moveStep] = 3;
          Column1[moveStep] = 6;
          break;
        case 'U':
          Column0[moveStep] = 5;
          Column1[moveStep] = 1;
          break;
        case 'V':
          Column0[moveStep] = 7;
          Column1[moveStep] = 1;
          break;
        case 'W':
          Column0[moveStep] = 2;
          Column1[moveStep] = 7;
          break;
        case 'X':
          Column0[moveStep] = 5;
          Column1[moveStep] = 5;
          break;
        case 'Y':
          Column0[moveStep] = 5;
          Column1[moveStep] = 7;
          break;
        case 'Z':
          Column0[moveStep] = 5;
          Column1[moveStep] = 3;
          break;
        default:
          break;
      }
      //
      // rotate Column0
      // compare with previous
      if (Column0[moveStep] != Column1[moveStep - 1])
      {
        //smaller dots
        if (Column0[moveStep] < Column1[moveStep - 1])
        {
          for (int n = 0; n < (Column1[moveStep - 1] - Column0[moveStep]); n++)
          {
            for (int i = 0; i < 800; i++) //Rotate right
            {
              digitalWrite(DIR2, LOW);
              digitalWrite(ENA2, HIGH);
              digitalWrite(PUL2, HIGH);
              delayMicroseconds(100);
              digitalWrite(PUL2, LOW);
              delayMicroseconds(100);
            }
          }
        }
        //
        //greater dots
        else if (Column0[moveStep] > Column1[moveStep - 1])
        {
          for (int n = 0; n < (Column0[moveStep] - Column1[moveStep - 1]); n++)
          {
            for (int i = 0; i < 800; i++) //Rotate left
            {
              digitalWrite(DIR2, HIGH);
              digitalWrite(ENA2, HIGH);
              digitalWrite(PUL2, HIGH);
              delayMicroseconds(100);
              digitalWrite(PUL2, LOW);
              delayMicroseconds(100);
            }
          }
        }
      }
      //
      delay(200);
      // move 1 step
      for (int i = 0; i < 4000; i++) //back
      {
        digitalWrite(DIR1, HIGH);
        digitalWrite(ENA1, HIGH);
        digitalWrite(PUL1, HIGH);
        delayMicroseconds(100);
        digitalWrite(PUL1, LOW);
        delayMicroseconds(100);
      }
      //
      delay(200);
      // rotate Column1
      if (Column1[moveStep] != Column0[moveStep])
      {
        if (Column1[moveStep] > Column0[moveStep])
        {
          for (int n = 0; n < (Column1[moveStep] - Column0[moveStep]); n++)
          {
            for (int i = 0; i < 800; i++) //Rotate
            {
              digitalWrite(DIR2, HIGH);
              digitalWrite(ENA2, HIGH);
              digitalWrite(PUL2, HIGH);
              delayMicroseconds(100);
              digitalWrite(PUL2, LOW);
              delayMicroseconds(100);
            }
          }
        }
        //
        else if (Column1[moveStep] < Column0[moveStep])
        {
          for (int n = 0; n < (Column0[moveStep] - Column1[moveStep]); n++)
          {
            for (int i = 0; i < 800; i++) //Rotate
            {
              digitalWrite(DIR2, LOW);
              digitalWrite(ENA2, HIGH);
              digitalWrite(PUL2, HIGH);
              delayMicroseconds(100);
              digitalWrite(PUL2, LOW);
              delayMicroseconds(100);
            }
          }
        }
      }
      //
      delay(200);
      // move 1 step
      for (int i = 0; i < 4000; i++) //backward
      {
        digitalWrite(DIR1, HIGH);
        digitalWrite(ENA1, HIGH);
        digitalWrite(PUL1, HIGH);
        delayMicroseconds(100);
        digitalWrite(PUL1, LOW);
        delayMicroseconds(100);
      }
      //Increase plate counting
      moveStep++;
      lastIndex++;
      Rolling = 1;
    }
    //
    Ready = 0;
    lastIndex = spacePos[spaceMark] + 1;
    spaceMark++;

  }
}
