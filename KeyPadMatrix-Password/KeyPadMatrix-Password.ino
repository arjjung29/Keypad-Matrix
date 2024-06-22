#include <Keypad.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET     -1 
#define SCREEN_ADDRESS 0x3C

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
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

String password = "1234"; // Default password
String input = ""; //User's Input
int count = 0; //To loop over the user's input until the max length of password is reached
int wrongAttempts = 0;

void setup() {
  Serial.begin(9600); //Begin Serial communication for displaying in the Serial Monitor
  GSM.begin(9600); //Begin Serial communication with GSM for sending AT commands
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
  display.clearDisplay();
}

void loop() {

  if (wrongAttempts >= 4) { //Cross check with number of wrong attempts so far
    display.clearDisplay();
    display.setCursor(0,0);  
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.print("Too many wrong attempts! Please wait for 1 minute.");
    Serial.println("Too many wrong attempts! Please wait for 1 minute.");
    display.setCursor(1,0);
    display.print("Calling the Registered Phone Number !");
    Serial.println("Calling the Registered Phone Number !");
    callup(); // Call the registered mobile number and hang up after 20 seconds
    delay(60000); // Delay of 1 minute
    wrongAttempts = 0; // Reset the wrong attempts counter
    display.clearDisplay();
  }

  char key = keypad.getKey();
  if (key) {
    if (key == 'A') { //Validation of your password or simply entering your password for access to be granted
      display.clearDisplay();
      display.setCursor(0,0);  
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.print("Enter Password: ");
      Serial.println("Enter Password: ");
      input = ""; // for storing user's input
      count = 0;
      while (count < 4) {
        char key = keypad.getKey();
        if (key) {
          Serial.print(key);
          input += key; // appending the keystrokes to the input string
          count++;
        }
      }
      Serial.println();
      if (input == password) { // Checking for match (Validating)
        Serial.println("Password Matched!");
        display.clearDisplay();
        display.setCursor(0,0);  
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.print("Password Matched !"); // printing to the OLED Display
      } else {
        Serial.println("Password Incorrect!");
        display.clearDisplay();
        display.setCursor(0,0);  
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.print("Password Incorrect!");
        wrongAttempts++; //Incrementing the wrong attempt counter
      }
    }
    
    else if (key == 'B') { //Case for Resetting your password
      display.clearDisplay();
      display.setCursor(0,0);  
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.print("Enter your old Password: ");
      Serial.println("Enter your old password: ");
      input = "";
      count = 0;
      while (count < 4) {
        char key = keypad.getKey();
        if (key) {
          Serial.print(key);
          input += key;
          count++;
        }
      }
      Serial.println();
      if (input == password) { //Validating if you're the previous person who set the last password
        display.clearDisplay();
        display.setCursor(0,0);  
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.print("Password Reset Successfully! Enter your new password:");
        Serial.println("Password Reset Successfully! Enter your new password:"); // If success the user can enter their new password
        password = ""; // Clear the old password
        input = "";
        count = 0;
        while (count < 4) {
          char key = keypad.getKey();
          if (key) {
            Serial.print(key);
            password += key;
            count++;
          }
        }
        Serial.println();
        Serial.println("New Password Set!");
        display.clearDisplay();
        display.setCursor(0,0);  
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.print("New Password Set!");
        
      } else {
        Serial.println("Password doesn't match!");
        display.clearDisplay();
        display.setCursor(0,0);  
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.print("Password doesn't match");
        wrongAttempts++;
      }
    }
    

  }
}

void update(){
  delay(500);
  while (Serial.available()) 
  {
    GSM.write(Serial.read()); //Forward what Serial received to Software Serial Port
  }
  while(GSM.available()) 
  {
    Serial.write(GSM.read()); //Forward what Software Serial received to Serial Port
  }
}

void callup(){
  Serial.println("Initializing..."); 
  delay(1000);

  GSM.println("AT"); // If initialization is success GSM Module will return "OK"
  update();
  GSM.println("ATD+ +916369727213;"); //  phone number to dial
  update();
  delay(20000); // wait for 20 seconds...
  GSM.println("ATH"); //hang up the call
  update();
}
