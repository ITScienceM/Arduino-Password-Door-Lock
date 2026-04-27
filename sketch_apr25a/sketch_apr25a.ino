#include <LiquidCrystal.h>
#include <Servo.h>

// LCD পিন কনফিগারেশন (আপনার ডায়াগ্রাম অনুযায়ী)
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
Servo doorServo;

// কী-প্যাড পিন কনফিগারেশন
byte rowPins[] = {2, 3, 4, 5}; 
byte colPins[] = {6, 7, 8};    

char keys[4][3] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

String inputPassword = "";
String correctPassword = "1234"; // এখানে আপনার পাসওয়ার্ড সেট করুন

void setup() {
  lcd.begin(16, 2);
  doorServo.attach(9);
  doorServo.write(0); // শুরুতেই লক থাকবে
  
  // পিন মোড সেট করা
  for (int i = 0; i < 4; i++) pinMode(rowPins[i], INPUT_PULLUP);
  for (int i = 0; i < 3; i++) pinMode(colPins[i], OUTPUT);

  lcd.print("Enter Password:");
}

void loop() {
  char pressedKey = getCustomKey();
  
  if (pressedKey != '\0') {
    if (pressedKey == '#') { // '#' চাপলে পাসওয়ার্ড চেক হবে
      checkPassword();
    } else if (pressedKey == '*') { // '*' চাপলে রিসেট হবে
      inputPassword = "";
      lcd.clear();
      lcd.print("Reset Done!");
      delay(1000);
      lcd.clear();
      lcd.print("Enter Password:");
    } else {
      inputPassword += pressedKey;
      lcd.setCursor(inputPassword.length() - 1, 1);
      lcd.print('*'); // সিকিউরিটির জন্য স্টার দেখাবে
    }
  }
  delay(100);
}

// কী-প্যাড রিড করার ফাংশন (লাইব্রেরি ছাড়া)
char getCustomKey() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(colPins[0], HIGH);
    digitalWrite(colPins[1], HIGH);
    digitalWrite(colPins[2], HIGH);
    digitalWrite(colPins[i], LOW);
    
    for (int j = 0; j < 4; j++) {
      if (digitalRead(rowPins[j]) == LOW) {
        delay(200); // বাটন বাউন্স কমানোর জন্য
        while (digitalRead(rowPins[j]) == LOW);
        return keys[j][i];
      }
    }
  }
  return '\0';
}

void checkPassword() {
  lcd.clear();
  if (inputPassword == correctPassword) {
    lcd.print("Access Granted!");
    doorServo.write(90); // দরজা খুলবে
    delay(3000);
    doorServo.write(0);  // ৩ সেকেন্ড পর বন্ধ হবে
  } else {
    lcd.print("Wrong Password!");
    delay(2000);
  }
  inputPassword = "";
  lcd.clear();
  lcd.print("Enter Password:");
}
