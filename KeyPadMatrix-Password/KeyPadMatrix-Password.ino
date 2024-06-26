#include <Keypad.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; // Connect to the row pinouts of the keypad
byte colPins[COLS] = {11, 10, 9, 8}; // Connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); // Initializing the Keypad object
SoftwareSerial GSM(7, 6); // SoftwareSerial(rxPin, txPin, inverse_logic), creating object for sending AT Commands to GSM Module

// LCD Configuration
LiquidCrystal_I2C lcd(0x3F, 16, 2); // Set the LCD I2C address and dimensions (16x2)

String password = "1234"; // Default password
String input = ""; // User's Input
int count = 0; // To loop over the user's input until the max length of password is reached
int wrongAttempts = 0;

void setup() {
  Serial.begin(9600); // Begin Serial communication for displaying in the Serial Monitor
  GSM.begin(9600); // Begin Serial communication with GSM for sending AT commands
  lcd.init(); // Initialize the LCD
  lcd.backlight(); // Turn on the backlight
  lcd.clear(); // Clear the LCD screen
}

void loop() {
  if (wrongAttempts >= 4) { // Cross check with number of wrong attempts so far
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Too many wrong");
    lcd.setCursor(0, 1);
    lcd.print("attempts!");
    delay(3000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Wait for");
    lcd.setCursor(0,1);
    lcd.print("One minute");
    delay(3000);
    Serial.println("Too many wrong attempts! Please wait for 1 minute.");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Alerting....");
    Serial.println("Calling the Registered Phone Number !");
    callup(); // Call the registered mobile number and hang up after 20 seconds
    delay(60000); // Delay of 1 minute
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("One minute done");
    lcd.setCursor(0,1);
    lcd.print("You can Continue");
    delay(2000);
    lcd.clear();
    wrongAttempts = 0; // Reset the wrong attempts counter
    lcd.clear();
  }

  char key = keypad.getKey();
  if (key) {
    lcd.clear();
    if (key == 'A') { // Validation of your password or simply entering your password for access to be granted
      lcd.setCursor(0, 0);
      lcd.print("Enter Password:");
      Serial.println("Enter Password: ");
      input = ""; // for storing user's input
      count = 0;
      lcd.setCursor(0,1);
      while (count < 4) {
        char key = keypad.getKey();
        if (key) {
          Serial.print(key);
          input += key; // appending the keystrokes to the input string
          lcd.print(key); // display on LCD
          count++;
        }
      }
      Serial.println();
      lcd.setCursor(0, 1);
      if (input == password) { // Checking for match (Validating)
        Serial.println("Password Matched!");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Password Matched!");
        delay(2000);
        lcd.clear();
      } else {
        lcd.clear();
        Serial.println("Password Incorrect!");
        lcd.setCursor(0,0);
        lcd.print("Password");
        lcd.setCursor(0,1);
        lcd.print("Incorrect");
        delay(2000);
        lcd.clear();
        wrongAttempts++; // Incrementing the wrong attempt counter
      }
    }
    
    else if (key == 'B') { // Case for Resetting your password
      lcd.setCursor(0, 0);
      lcd.print("Enter old");
      lcd.setCursor(0,1);
      lcd.print("Password:");
      Serial.println("Enter your old password: ");
      input = "";
      count = 0;
      while (count < 4) {
        char key = keypad.getKey();
        if (key) {
          Serial.print(key);
          input += key;
          lcd.print(key); // display on LCD
          count++;
        }
      }
      Serial.println();
      lcd.clear();
      lcd.setCursor(0, 0);
      if (input == password) { // Validating if you're the previous person who set the last password
        lcd.print("Password Reset");
        Serial.println("Password Reset Successfully! Enter your new password:");
        lcd.setCursor(0, 1);
        lcd.print("Successfully!");
        delay(2000);
        password = ""; // Clear the old password
        input = "";
        count = 0;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Enter New");
        lcd.setCursor(0,1);
        lcd.print("Password:");
        while (count < 4) {
          char key = keypad.getKey();
          if (key) {
            Serial.print(key);
            password += key;
            lcd.print(key); // display on LCD
            count++;
          }
        }
        Serial.println();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("New Password Set!");
        delay(2000);
        lcd.clear();
      } else {
        Serial.println("Password doesn't match!");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Password doesn't");
        lcd.setCursor(0,1);
        lcd.print("Match !");
        delay(2000);
        lcd.clear();
        wrongAttempts++;
      }
    }
  }
  /*else{
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Press A for");
   lcd.setCursor(0,1);
   lcd.print("Validation");
   delay(100);
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Press B for");
   lcd.setCursor(0,1);
   lcd.print("Password Reset");
   delay(100);

  }*/
}

void update(){
  delay(500);
  while (Serial.available()) {
    GSM.write(Serial.read()); // Forward what Serial received to Software Serial Port
  }
  while(GSM.available()) {
    Serial.write(GSM.read()); // Forward what Software Serial received to Serial Port
  }
}

void callup(){
  Serial.println("Initializing..."); 
  delay(1000);

  GSM.println("AT"); // If initialization is success GSM Module will return "OK"
  update();
  GSM.println("ATD+916369727213;"); // phone number to dial
  update();
  delay(20000); // wait for 20 seconds...
  GSM.println("ATH"); // hang up the call
  update();
}