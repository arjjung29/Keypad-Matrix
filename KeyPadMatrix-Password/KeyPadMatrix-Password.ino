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

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); // Initializing the Keypad object

String password = "1234"; // Default password
String input = ""; //User's Input
int count = 0; //To loop over the user's input until the max length of password is reached
int wrongAttempts = 0;

void setup() {
  Serial.begin(9600); //Begin Serial communication for displaying in the Serial Monitor
}

void loop() {

  if (wrongAttempts >= 4) { //Cross check with number of wrong attempts so far
    Serial.println("Too many wrong attempts! Please wait for 1 minute.");
    delay(60000); // Delay of 1 minute
    wrongAttempts = 0; // Reset the wrong attempts counter
  }

  char key = keypad.getKey();
  if (key) {
    if (key == 'A') { //Validation of your password or simply entering your password for access to be granted
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
      } else {
        Serial.println("Password Incorrect!");
        wrongAttempts++; //Incrementing the wrong attempt counter
      }
    }
    
    else if (key == 'B') { //Case for Resetting your password
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
      } else {
        Serial.println("Password doesn't match!");
        wrongAttempts++;
      }
    }
    

  }
}
