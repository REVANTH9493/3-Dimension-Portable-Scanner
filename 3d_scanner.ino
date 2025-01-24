//---------------------------------------------------
// 1. Librerias
//---------------------------------------------------
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>

//----------------------------------------------------
// 2. Pins
//----------------------------------------------------
int xPin = A1;
int yPin = A0;
int buttonPin = 8;
int btn_score = 0;
int btn_count = 0;
int prev_btn_count = 0;
int start = 9;
int relay = 12;


//SDA            A4
//SCL            A5

//----------------------------------------------------
// 3. Variables y Comandos
//----------------------------------------------------
int tCount1;
int pic_count = 1;
int rotation_count = 0;
unsigned long prev_time = 0;
const long interval = 20000;
bool set = true;
int r = 0;
int r1 = 0;
bool refresh;  //lcd clear On/Off
//leerJoystick
int joyRead;
int joyPos;
int lastJoyPos;
long lastDebounceTime = 0;
long debounceDelay = 70;  //user define
//Control Joystick
int count = 0;
int prev_count = 0;
bool PQCP;
bool editMode;
//sistema de menus
int count1;
int count2;
int nre = 0;
//editmode
byte n[19];
int lastN;
int lcdX;
//int lcdY;
bool exiT;

// Define the number of steps per revolution for your stepper motors
const int stepsPerRevolution = 100;
const int angle = 800;
int revo = 800;
int stepsPerRevolution1 = revo;
int stepsPerRevolution2 = 2000;
int i_val = 15;
int SMSpeed = 100;  // Stepper Motor Speed

int initi=1;
// Define variables to store joystick data
int vrx_data = 0;
int vry_data = 0;

// Create instances of the Stepper class for X and Y motors
Stepper stepperX(stepsPerRevolution, 6, 7);
Stepper stepperY(stepsPerRevolution, 4, 5);
Stepper mystepper(stepsPerRevolution1, 6, 7);
Stepper mystepper1(stepsPerRevolution2, 4, 5);

//----------------------------------------------------
// 4. Objetos
//----------------------------------------------------
LiquidCrystal_I2C lcd(0x27, 16, 2);
byte c1[8] = {
  B00000,
  B01010,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
};  //Smile-1
byte c2[8] = {
  B00000,
  B01010,
  B00100,
  B00100,
  B00000,
  B01110,
  B10001,
};  //Smile-2
byte c10[8] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B11111,
  B01110,
  B00100,
};  //Down-Arrow
byte c9[8] = {
  B00100,
  B01110,
  B11111,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
};  //Up-Arrow
//====================================================
// SETUP
//====================================================
void setup() {
  //----------------------------------------------------
  // S1. Pines
  //----------------------------------------------------
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(relay, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(start, INPUT_PULLUP);
  // pinMode(vrx, INPUT);
  // pinMode(vry, INPUT);
  //----------------------------------------------------
  // S2. Objetos
  //----------------------------------------------------
  lcd.createChar(0, c1);  //Creating custom characters in CG-RAM
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);

  delay(1000);
  lcd.createChar(0, c1);
  lcd.setCursor(0, 0);
  lcd.print(char(0));
  lcd.setCursor(1, 0);
  lcd.print(char(0));
  lcd.setCursor(3, 0);
  lcd.print("Welcome To");
  lcd.setCursor(3, 1);
  lcd.print("3d Scanner");
  lcd.createChar(0, c1);
  lcd.setCursor(14, 1);
  lcd.print(char(0));
  lcd.setCursor(15, 1);
  lcd.print(char(0));

  delay(3000);
  lcd.clear();
  lcd.setCursor(7, 0);
  lcd.print("BY");
  lcd.setCursor(2, 1);
  lcd.print("FUSION SARVAM");
  delay(2000);

  mystepper.setSpeed(100);
  mystepper1.setSpeed(100);
  stepperX.setSpeed(60);
  stepperY.setSpeed(60);

  digitalWrite(relay, HIGH);

  Serial.begin(115200);  // Initialize serial communication
}

//====================================================
// LOOP
//====================================================
void loop() {
  if (set) {
    controlJoystick();
  } else if (!set) {
    joystick();
  }
  button();
  menu();
  if (digitalRead(start)==LOW){
    lcd.clear();
    count1=3;
  }
}

//====================================================
// Menu
//====================================================
void menu() {
  switch (count1) {
    case 0:
      menu0();  //pantalla de inicio
      break;
    case 1:
      if (count2 == 1) {
        menu11();
      } else if (count2 == 2) {
        menu12();
      } else if (count2 == 3) {
        menu13();
      } else {
        menu1();
      }
      break;
    case 2:
      if (count2 == 1) {
        menu21();
      } else {
        menu2();
      }
      break;
    case 3:
      menu3();  //
      break;
  }
}

//----------------------------------------------------
// Welcome to 3D scanner
//----------------------------------------------------
void menu0() {
  set = true;
  if (refresh) {
    lcd.clear();
    refresh = 0;
  }
  lcd.setCursor(2, 0);
  lcd.print("Use Joystick ");
  lcd.setCursor(2, 1);
  lcd.print("To setup Scan");
  digitalWrite(relay, HIGH);

  //menu1();
}

//----------------------------------------------------
// Pictures
//----------------------------------------------------
void menu1() {
  set = true;
  if (refresh) {
    lcd.clear();
    refresh = 0;
  }
  lcd.setCursor(3, 0);
  lcd.print("Number of");
  lcd.setCursor(0, 1);
  lcd.print("Pictures [");
  lcd.print(i_val*4);
  lcd.print("] ");
  lcd.createChar(2, c10);
  lcd.setCursor(13, 1);
  lcd.print(char(2));
  delay(50);
  lcd.setCursor(14, 1);
  lcd.print(char(2));
  
  //++++++++++++++++++++
  while (editMode) {
    controlJoystick();
    lcd.setCursor(lcdX, 1);
    if (n[lcdX] != lastN) {
      lcd.print(n[lcdX]);
      lastN = n[lcdX];
    }
  }
}

//-------------------------------------------------1.1
void menu11() {
  set = true;
  if (refresh) {
    lcd.clear();
    refresh = 0;
  }
  lcd.setCursor(7, 0);
  lcd.print("60");
  lcd.setCursor(4, 1);
  lcd.print("Pictures");
  lcd.createChar(4, c9);
  lcd.setCursor(14, 0);
  lcd.print(char(4));
  lcd.print("+");
  lcd.createChar(5, c10);
  lcd.setCursor(14, 1);
  lcd.print(char(5));
  lcd.print("-");
  if (btn_count > prev_btn_count) {
    revo = 800;
    i_val = 20;
    btn_count = 0;
    prev_btn_count = 0;
    Serial.println("20 pics");
    count1 = 1;
  }
}

//-------------------------------------------------1.2
void menu12() {
  set = true;
  if (refresh) {
    lcd.clear();
    refresh = 0;
  }
  lcd.setCursor(7, 0);
  lcd.print("45");
  lcd.setCursor(4, 1);
  lcd.print("Pictures");
  lcd.createChar(4, c9);
  lcd.setCursor(14, 0);
  lcd.print(char(4));
  lcd.print("+");
  lcd.createChar(5, c10);
  lcd.setCursor(14, 1);
  lcd.print(char(5));
  lcd.print("-");
  if (btn_count > prev_btn_count) {
    revo = 1200;
    i_val = 15;
    btn_count = 0;
    prev_btn_count = 0;
    Serial.println("15 pics");
    count1 = 1;
  }
}

//-------------------------------------------------1.3
void menu13() {
  set = true;
  if (refresh) {
    lcd.clear();
    refresh = 0;
  }
  lcd.setCursor(7, 0);
  lcd.print("30");
  lcd.setCursor(4, 1);
  lcd.print("Pictures");
  lcd.createChar(4, c9);
  lcd.setCursor(14, 0);
  lcd.print(char(4));
  lcd.print("+");
  lcd.createChar(5, c10);
  lcd.setCursor(14, 1);
  lcd.print(char(5));
  lcd.print("-");
  if (btn_count > prev_btn_count) {
    revo = 1600;
    i_val = 10;
    btn_count = 0;
    prev_btn_count = 0;
    Serial.println("10 pics");
    count1 = 1;
  }
}

//----------------------------------------------------
// Set Positions
//----------------------------------------------------
void menu2() {
  set = true;
  if (refresh) {
    lcd.clear();
    refresh = 0;
  }
  lcd.setCursor(1, 0);
  lcd.print("Set Positions");
  lcd.createChar(6, c10);
  lcd.setCursor(7, 1);
  lcd.print(char(6));
  lcd.setCursor(8, 1);
  lcd.print(char(6));
}

//-------------------------------------------------2.1
void menu21() {
  set = true;
  if (refresh) {
    lcd.clear();
    refresh = 0;
  }
  lcd.setCursor(2, 0);
  lcd.print("Set Motors");
  lcd.setCursor(2, 1);
  // lcd.print
  // unsigned long time = millis();
  // if (time - prev_time < interval){
  //   set = false;
  // }
// for (btn_score == 0){}
nre==0;
if (digitalRead(buttonPin) == LOW) {
  btn_score = 1;
  nre=1;
} 
if (btn_score = 1){
  set = false;
}
if (nre==1){
  nre==0;
  //btn_score =0;
  set = true;
}
  //   else if (btn_score == 1) {
  //     set = true;
  //     btn_score = 0;
  //     lcd.clear();
  //     // menu3();
  //   }
  // }

}

//----------------------------------------------------
// Start
//----------------------------------------------------
void menu3() {
  set = true;
  if (refresh) {
    lcd.clear();
    refresh = 0;
  }
  if(initi==1){
    initi=0;
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("Checking ");
  lcd.setCursor(5, 1);
  lcd.print("Motor's");
  stepperX.step(-100);
  delay(1000);
  stepperX.step(100);
  delay(1000);
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("OK");
  delay(1000);
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("Checking ");
  lcd.setCursor(5, 1);
  lcd.print("Camera");
  delay(500);
  Serial.print("C");
  delay(2000);
  Serial.print("C");
  delay(2000);
  Serial.print("C");
  delay(2000);
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Capturing");
  lcd.setCursor(6, 1);
  lcd.print("OK");
  delay(1000);
  lcd.clear();
  }
  lcd.setCursor(5, 0);
  lcd.print("Start");
  lcd.setCursor(4, 1);
  lcd.print("Machine");
  

  motor();
}

void motor(){
  if (rotation_count < 3) {
    if(rotation_count==1){
      for (int i = 0; i < i_val*2; i++) {
      lcd.clear();
      lcd.setCursor(5,0);
      lcd.print("No. of ");
      lcd.setCursor(2,1);
      lcd.print("Pictures = ");
      lcd.print(pic_count);
      mystepper.step(stepsPerRevolution1 / 20);  // Rotate 1/20th of a full revolution
      delay(2000);
      Serial.print("C");
      delay(1000);  // Wait for 1 
      pic_count++;
    }
    }
    else{
      for (int i = 0; i < i_val; i++) {
        lcd.clear();
        lcd.setCursor(5,0);
        lcd.print("No. of ");
        lcd.setCursor(2,1);
        lcd.print("Pictures = ");
        lcd.print(pic_count);
        mystepper.step(stepsPerRevolution1 / 20);  // Rotate 1/20th of a full revolution
        delay(2000);
        Serial.print("C");
        delay(1000);  // Wait for 1 
        pic_count++;
        }
    }
    for (int j = 0; j < 1; j++) {
      mystepper1.step(-(angle / 20));  // Rotate 1/20th of a full revolution
      delay(1000);                  // Wait for 1 secondpic_count++;
    }
    rotation_count = rotation_count + 1;
  }
  else if (rotation_count == 3){
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Done Capturing!");
    delay(1000);
    init==1;
    count1 = 0;
    lcd.clear();
  }

}

//====================================================
// Control Joystick
//====================================================
void joystick() {
  vrx_data = analogRead(xPin);
  vry_data = analogRead(yPin);

  Serial.print("xPin:");
  Serial.println(vrx_data);
  Serial.print("yPin:");
  Serial.println(vry_data);

  // // Check if the button is pressed
  // if (button_data == HIGH) {

  // }

  // Check joystick X-axis movement
  if (vrx_data > 700) {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Set Motors");
  lcd.setCursor(2, 1);
    lcd.setCursor(2, 1);
    lcd.createChar(6, c10);
    lcd.setCursor(7, 1);
    lcd.print(char(6));
    delay(10);
    stepperY.setSpeed(SMSpeed);
    stepperY.step(-5);
    delay(5);  // Adjust this delay if necessary

    delay(200);
  } else if (vrx_data < 300) {
      lcd.clear();
      lcd.setCursor(2, 0);
  lcd.print("Set Motors");
  lcd.setCursor(2, 1);
    lcd.createChar(6, c9);
    lcd.setCursor(7, 1);
    lcd.print(char(6));
    delay(10);
    stepperY.setSpeed(SMSpeed);
    stepperY.step(5);
    delay(5);  // Adjust this delay if necessary
  }

  // Check joystick Y-axis movement
  if (vry_data > 700) {
    lcd.clear();
      lcd.setCursor(2, 0);
  lcd.print("Set Motors");
  lcd.setCursor(2, 1);
    lcd.setCursor(6, 1);
    lcd.print("->");

    stepperX.setSpeed(SMSpeed);
    stepperX.step(-100);
    delay(5);  // Adjust this delay if necessary

  } else if (vry_data < 300) {
    lcd.clear();
      lcd.setCursor(2, 0);
  lcd.print("Set Motors");
  lcd.setCursor(2, 1);
        lcd.setCursor(6, 1);
    lcd.print("<-");

    stepperX.setSpeed(SMSpeed);
    stepperX.step(100);
    delay(5);  // Adjust this delay if necessary
  }
}

void button() { 
  if (digitalRead(buttonPin) == 0) {
    prev_btn_count = btn_count;
    btn_count = 1;
  }
}

void controlJoystick() {
  leeJoystick();
  if (PQCP) {
    PQCP = 0;
    if (joyPos == 5) {
      editMode = !editMode;
    }
    switch (editMode) {
      case 1:
        lcd.blink();
        if (joyPos == 4 && n[lcdX] < 9) {  //above
          n[lcdX]++;
          refresh = 0;
        }
        if (joyPos == 3 && n[lcdX] > 0) {  //below
          n[lcdX]--;
          refresh = 0;
        }
        if (joyPos == 1 && lcdX < 19) {  //right
          lcdX++;
          refresh = 0;
        }
        if (joyPos == 2 && lcdX > 0) {  //left
          lcdX--;
          refresh = 0;
        }
        break;
      case 0:
        lcd.noBlink();
        if (count1 < 3 && joyPos == 3) {  //above
          count1++;
          refresh = 1;
          count2 = 0;
        }
        if (count1 > 0 && joyPos == 4) {  //below
          count1--;
          count2 = 0;
          refresh = 1;
        }
        if (count2 < 3 && joyPos == 1) {  //right
          count2++;
          refresh = 1;
        }
        if (count2 > 0 && joyPos == 2) {  //left
          count2--;
          refresh = 1;
        }
    }
  }
}

int leeJoystick() {
  int x = analogRead(xPin);
  int y = analogRead(yPin);
  bool k = digitalRead(buttonPin);

  if (x > 900) {
    joyRead = 1;  //x+
  } else if (x < 100) {
    joyRead = 2;  //x-
  } else if (y > 900) {
    joyRead = 3;  //y+
  } else if (y < 100) {
    joyRead = 4;  //y-
  } else if (!k) {
    joyRead = 5;
  } else {
    joyRead = 0;
  }

  if (joyRead != lastJoyPos) {
    lastDebounceTime = millis();
  }
  if (((millis() - lastDebounceTime) > debounceDelay) && (joyRead != joyPos)) {
    joyPos = joyRead;
    if (!PQCP) {
      PQCP = 1;
    }
  }
  if (((millis() - lastDebounceTime) > (5 * debounceDelay)) && (joyPos == 3 || joyPos == 4)) {
    joyPos = joyRead;  //repeat time only for UP/DOWN
    if (!PQCP) {
      PQCP = 1;
    }
  }
  lastJoyPos = joyRead;
}
