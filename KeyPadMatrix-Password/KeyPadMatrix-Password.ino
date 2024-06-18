#include <Keypad.h>

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

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String password = "1234"; // Default password
String input = "";
int count = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    if (key == 'A') {
      Serial.println("Enter Password: ");
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
      if (input == password) {
        Serial.println("Password Matched!");
      } else {
        Serial.println("Password Incorrect!");
      }
    }
    
    else if (key == 'B') {
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
      if (input == password) {
        Serial.println("Password Reset Successfully! Press A to enter your new password.");
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
      } else {
        Serial.println("Password doesn't match!");
      }
    }
    
    else if (key == 'C') {
      Serial.println("Password Check!");
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
      if (input == password) {
        Serial.println("Password Matched!");
      } else {
        Serial.println("Password Incorrect!");
      }
    }
  }
}
