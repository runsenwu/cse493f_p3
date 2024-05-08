#include <Keyboard.h>
#include <Dictionary.h>
#include <string.h>

const int ARCADE_BUTTON_INPUT_PIN = 2;
const int ARCADE_BUTTON_LED_PIN = 3;

const int MAX_FADE_VAL = 100;
const int minButtonLongPressDuration = 250;
const int intervalButton = 20;
const int pause = 1000;

String curr_char;

int buttonStatePrevious = LOW;

unsigned long buttonLongPressMillis;
unsigned long previousButtonMillis;
unsigned long buttonPressDuration;
unsigned long currentMillis;
unsigned long lastInput;

bool buttonStateLongPress = false;

const String dictArray[] = {
  "sl", "lsss", "lsls", "lss", "s", "ssls", "lls", "ssss", "ss", "slll",
  "lsl", "slss", "ll", "ls", "lll", "slls", "llsl", "sls", "sss", "l",
  "ssl", "sssl", "sll", "lssl", "lsll", "llss", "sllll", "sslll", "sssll",
  "ssssl", "sssss", "lssss", "llsss", "lllss", "lllls", "lllll"
};

bool _btnPressed = false;
Dictionary<String, String> dict;

// std::vector<int> sequence;

void setup() {
  curr_char = "";
  // dict = Dictionary<String, String>();
  populate_mapping();

  previousButtonMillis = 0;
  lastInput = 0;
  currentMillis = millis();

  Serial.begin(9600);
  Keyboard.begin();
  pinMode(ARCADE_BUTTON_INPUT_PIN, INPUT_PULLUP);
  pinMode(ARCADE_BUTTON_LED_PIN, OUTPUT);
  Serial.println("wtf");
}

void loop() {
  currentMillis = millis();
  readButtonState();


  if (currentMillis - lastInput > pause) {
    Serial.println("we here");
    print_character();
    curr_char = "";
  } 
}

void readButtonState() {
  if (currentMillis - previousButtonMillis > intervalButton) {
    int buttonState = digitalRead(ARCADE_BUTTON_INPUT_PIN);

    // our button might be reversed right here
    if (buttonState == LOW && buttonStatePrevious == HIGH && !buttonStateLongPress) {
      buttonLongPressMillis = currentMillis;
      buttonStatePrevious = LOW;
      digitalWrite(ARCADE_BUTTON_LED_PIN, HIGH);
    }

    buttonPressDuration = currentMillis - buttonLongPressMillis;

    if (buttonState == LOW && !buttonStateLongPress && buttonPressDuration >= minButtonLongPressDuration) {
      buttonStateLongPress = true;
    }

    if (buttonState == HIGH && buttonStatePrevious == LOW) {
      buttonStatePrevious = HIGH;
      buttonStateLongPress = false;
      lastInput = currentMillis;

      if (!buttonStateLongPress && buttonPressDuration < minButtonLongPressDuration) {
        curr_char += "s";
      } else {
        curr_char += "l";
      }
      digitalWrite(ARCADE_BUTTON_LED_PIN, LOW);
    }

    previousButtonMillis = currentMillis;
  }
}

void print_character() {
  if (curr_char == "ssll") {
    Keyboard.press(8);
    Keyboard.release(8);
    return;
  } else if (curr_char == "llll"){
    Keyboard.press(10);
    Keyboard.release(10);
    return;
  } else if (curr_char == "slsl") {
    Keyboard.press(32);
    Keyboard.release(32);
  }

  Serial.println("printing the character");
  Serial.println("current character: " + curr_char);
  if (curr_char == "") {
    Serial.println("early return");
    return;
  }

  bool seen = false;

  Serial.print("pre for loop");

  for (int i = 0; i < 36; i++) {
    if (dictArray[i] == curr_char) {
      seen = true;
      break;
    }
  }

  Serial.println("passed the for loop");

  if (seen) {
    String value = dict.get(curr_char);    
    Serial.println("we made it and seen the string");
    Keyboard.press(value[0]);
    Serial.println("value pressed: " + value);
    Keyboard.release(value[0]);
  }
}

void populate_mapping() {
  dict.set("sl", "a");
  dict.set("lsss", "b");
  dict.set("lsls", "c");
  dict.set("lss", "d");
  dict.set("s", "e");
  dict.set("ssls", "f");
  dict.set("lls", "g");
  dict.set("ssss", "h");
  dict.set("ss", "i");
  dict.set("slll", "j");
  dict.set("lsl", "k");
  dict.set("slss", "l");
  dict.set("ll", "m");
  dict.set("ls", "n");
  dict.set("lll", "o");
  dict.set("slls", "p");
  dict.set("llsl", "q");
  dict.set("sls", "r");
  dict.set("sss", "s");
  dict.set("l", "t");
  dict.set("ssl", "u");
  dict.set("sssl", "v");
  dict.set("sll", "w");
  dict.set("lssl", "x");
  dict.set("lsll", "y");
  dict.set("llss", "z");
  dict.set("sllll", "1");
  dict.set("sslll", "2");
  dict.set("sssll", "3");
  dict.set("ssssl", "4");
  dict.set("sssss", "5");
  dict.set("lssss", "6");
  dict.set("llsss", "7");
  dict.set("lllss", "8");
  dict.set("lllls", "9");
  dict.set("lllll", "0");
}
