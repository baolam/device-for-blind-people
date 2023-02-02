#include <Wire.h>
#include "LiquidCrystal_I2C.h"
#include <SPI.h>
#include <SD.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
File myFile;

int PUL1 = 7; // Sliding
int DIR1 = 6; //
int ENA1 = 5; //
//
int PUL2 = 4; // Rotating
int DIR2 = 3; //
int ENA2 = 2; //

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

// --------------
// Điều khiển động cơ
void forward(int step, int dl = 200);
void backward(int step, int dl = 200);
void left(int step, int dl = 200);
void right(int step, int dl = 200);
// --------------
void reset();
void initallize();
void build_char(char key);
void decode();
// --------------
String FORWARD = "TF";
String BACKWARD = "TB";
String LEFT = "TL";
String RIGHT = "TR";
String TESTCO = "TG";
String SENS = "SE";
String REQUEST_CONFIG = "RC";
String LOAD_SEN = "LO";
String STOP_UPDATING = "SO";
String ALLOW_UPDATING = "AL";
String ip = "";
String command = "";
bool finish_sens = false;
// --------------

void setup() {
  // put your setup code here, to run once:
  pinMode (PUL1, OUTPUT);
  pinMode (DIR1, OUTPUT);
  pinMode (ENA1, OUTPUT);
  pinMode (PUL2, OUTPUT);
  pinMode (DIR2, OUTPUT);
  pinMode (ENA2, OUTPUT);
  pinMode(inButton, INPUT);
  
 //
  Serial.begin(9600);
  delay(5000);
  // Serial.print("Initializing SD card...");
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
  lcd.print("Ready");
  lcd.setCursor(0, 1);
  //

  // Set up column
  Column0[0] = 0;
  Column1[0] = 0;

  /// Khối lệnh hiển thị địa chỉ IP
  Serial.println("ST");
  while (ip.length() == 0)
  {
    Serial.println(REQUEST_CONFIG);
    delay(1000);
    if (Serial.available())
    {
      ip = Serial.readStringUntil('#');
    }
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Server's address");
  lcd.setCursor(0, 1);
  lcd.print(ip);

  while(digitalRead(inButton) == 0)
  {}

  lcd.clear();

  Serial.println('D' + ip);
  delay(1000);
  Serial.println(LOAD_SEN);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available()) {
    command = Serial.readStringUntil('#');
  }

  // Thử in
  // if (command.length() != 0)
  // {
  //   Serial.println('D' + command);
  // }

  // Phần mã lệnh và điều khiển
  if (command == FORWARD) 
    forward(800);
  else if (command == BACKWARD)
    backward(800);
  else if (command == LEFT)
    left(800);
  else if (command == RIGHT)
    right(800);
  else if (command == TESTCO)
    Ready = 1;
  else if (command == SENS)
  {
    reset();
    initalize();
    finish_sens = false;
  }

  Nextword = digitalRead(inButton);
  if (Nextword == 1)
    Ready = 1;
  if (! finish_sens)
  {
    //Serial.println("DDECODE");
    decode();
  }
  command = "";
}

void reset()
{
  Nextword = 0;
  Rolling = 0;
  spaceIndex = 1;
  lastIndex = 1;
  spaceMark = 1;
  moveStep = 1;
  Ready = 0;
  stringIndex = 1;
}

void initalize()
{
  // Gửi lệnh này để nhận phản hổi điều khiển
  delay(1000);
  Serial.println("RE");
  
  // Xóa màn hình
  lcd.clear(); 
  lcd.setCursor(0, 0);
  lcd.print("Sentence");
  lcd.setCursor(0, 1);

  String f = "";
  while(f.length() == 0)
  {
    f = Serial.readStringUntil('#');
  }
  Serial.println('D' + f);
  
  for (int i = 0; i < f.length(); i++)
  {
    inputChar = f[i];
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
      stringIndex++;
    }
  }

  // Set up column
  Column0[0] = 0;
  Column1[0] = 0;
}

void forward(int step, int dl = 200)
{
  for (int i = 0; i < step; i++)
  {
    digitalWrite(DIR1, LOW);
    digitalWrite(ENA1, HIGH);
    digitalWrite(PUL1, HIGH);
    delayMicroseconds(dl);
    digitalWrite(PUL1, LOW);
    delayMicroseconds(dl);
  }
}

void backward(int step, int dl = 200)
{
  for (int i = 0; i < step; i++)
  {
    digitalWrite(DIR1, HIGH);
    digitalWrite(ENA1, HIGH);
    digitalWrite(PUL1, HIGH);
    delayMicroseconds(dl);
    digitalWrite(PUL1, LOW);
    delayMicroseconds(dl);
  }
}

void left(int step, int dl = 200)
{
  for (int i = 0; i < 800; i++)
  {
    digitalWrite(DIR2, HIGH);
    digitalWrite(ENA2, HIGH);
    digitalWrite(PUL2, HIGH);
    delayMicroseconds(dl);
    digitalWrite(PUL2, LOW);
    delayMicroseconds(dl);
  }
}

void right(int step, int dl = 200)
{
  for (int i = 0; i < 800; i++)
  {
    digitalWrite(DIR2, LOW);
    digitalWrite(ENA2, HIGH);
    digitalWrite(PUL2, HIGH);
    delayMicroseconds(dl);
    digitalWrite(PUL2, LOW);
    delayMicroseconds(dl);
  }
}

void build_char(char key)
{
  switch (key)
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
}

void decode()
{
  if ((Ready == 1) && (spaceMark <= spaceIndex))
  {
    /// Gửi lệnh này để raspbery pi biết
    /// dừng việc nhận câu mới
    Serial.println(STOP_UPDATING);

    lcd.clear();
    lcd.setCursor(0, 0);

    // Set zero here
    if (Rolling != 0)
    {
      lcd.print("Rolling back... ");
      //
      for (int j = moveStep; j > 1; j--)
      {
        // Set back 1 step
        forward(4000, 100);
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
              right(800, 100);
            }
          }
          else if (Column1[moveStep] < Column0[moveStep])
          {
            for (int m = 0; m < (Column0[moveStep] - Column1[moveStep]); m++)
            {
              left(800, 100);
            }
          }
        }
        //
        delay(200);
        //
        forward(4000, 100);
        //
        delay(200);
        //
        if (Column0[moveStep] != Column1[moveStep - 1])
        {
          if (Column0[moveStep] > Column1[moveStep - 1])
          {
            for (int m = 0; m < (Column0[moveStep] - Column1[moveStep - 1]); m++)
            {
              right(800, 100);
            }
          }
          else if (Column0[moveStep] < Column1[moveStep - 1])
          {
            for (int m = 0; m < (Column1[moveStep - 1] - Column0[moveStep]); m++)
            {
              left(800, 100);
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
    for (int k = lastIndex; k < spacePos[spaceMark]; k++)
    {
      lcd.print(inputString[lastIndex]);
      build_char(inputString[lastIndex]);
      //
      // rotate Column0
      // compare with pevious
      if (Column0[moveStep] != Column1[moveStep - 1])
      {
        //smaller dots
        if (Column0[moveStep] < Column1[moveStep - 1])
        {
          for (int n = 0; n < (Column1[moveStep - 1] - Column0[moveStep]); n++)
          {
            right(800, 100);
          }
        }
        //
        //greater dots
        else if (Column0[moveStep] > Column1[moveStep - 1])
        {
          for (int n = 0; n < (Column0[moveStep] - Column1[moveStep - 1]); n++)
          {
            left(800, 100);
          }
        }
      }
      //
      delay(200);
      // move 1 step
      backward(4000, 100);
      //
      delay(200);
      // rotate Column 1
      if (Column1[moveStep] != Column0[moveStep])
      {
        if (Column1[moveStep] > Column0[moveStep])
        {
          for (int n = 0; n < (Column1[moveStep] - Column0[moveStep]); n++)
          {
            left(800, 100);
          }
        }
        //
        else if (Column1[moveStep] < Column0[moveStep])
        {
          for (int n = 0; n < (Column0[moveStep] - Column1[moveStep]); n++)
          {
            right(800, 100);
          }
        }
      }
      //
      delay(200);
      // move 1 step
      backward(4000, 100);
      // Increase plate counting
      moveStep++;
      lastIndex++;
      Rolling = 1;
    }
    //
    Ready = 0;
    lastIndex = spacePos[spaceMark] + 1;
    spaceMark++;
  }
  else if (spaceMark > spaceIndex)
  {
    // Hoàn thành chuỗi
    finish_sens = true;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Finish");
    Serial.println(ALLOW_UPDATING);
  }
}