int debugPin = 2;
boolean debug = false;

int waterPin = A1;
int waterLed = 13;
int waterAan = 11;
int waterValue = 0;
int waterLimitLower = 0;
int waterLimitUpper = 100;
boolean waterPulse = false;
boolean waterFirst = true;

int powerPin = A0;
int powerLed = 12;
int powerAan = 10;
int powerValue = 0;
int powerLimitLower = 0;
int powerLimitUpper = 1023;
boolean powerPulse = false;
boolean powerFirst = true;


void setup() {
  // set as input and enable the internal pull-up resistor
  pinMode(waterAan, INPUT_PULLUP);
  pinMode(waterLed, OUTPUT);
  pinMode(powerAan, INPUT_PULLUP);
  pinMode(powerLed, OUTPUT);
  
  pinMode(debugPin, INPUT_PULLUP);
  debug = !digitalRead(debugPin);
  
  Serial.begin(115200);

  Serial.println("METERMETER OK");
  Serial.print("powerpin: ");
  Serial.print(powerAan);
  Serial.print(",");
  if(digitalRead(powerAan)) {
    digitalWrite(powerLed, HIGH);
    Serial.println("1");
  }
  else {
    digitalWrite(powerLed, LOW);
    Serial.println("0");
  }
  
  Serial.print("waterpin: ");
  Serial.print(waterAan);
  Serial.print(",");
  if(digitalRead(waterAan)) {
    digitalWrite(waterLed, HIGH);
    Serial.println("1");
  }
  else {
    digitalWrite(waterLed, LOW);
    Serial.println("0");
  }
  
  if(debug) {
    Serial.println("DEBUG PARTY MODE!");

    for(int i = 1; i < 40; i++){

      digitalWrite(waterLed, i%2);
      digitalWrite(powerLed, (i+1)%2);
      delay(100);
    }

    digitalWrite(waterLed, HIGH);
    digitalWrite(powerLed, LOW);
  }
  else {
    Serial.println("NORMAL OPERATION MODE");
  }
}

// the loop routine runs over and over again forever:
void loop() {
  boolean checkWater = digitalRead(waterAan);
  boolean checkPower = digitalRead(powerAan);
  
  if(debug) {
    Serial.print("water: ");
    if(checkWater) {
      Serial.print(analogRead(waterPin));
    } else {
      Serial.print("x");
    }
    
    Serial.print("    power: ");
    if(checkPower) {
      Serial.println(analogRead(powerPin));
    } else {
      Serial.println("x");
    }
    
    digitalWrite(waterLed, !digitalRead(waterLed));
    digitalWrite(powerLed, !digitalRead(powerLed));
    delay(1000);
  }
  else {
    digitalWrite(waterLed, checkWater);
    // water
    if(checkWater) {
      boolean waterPulsePrevious = waterPulse;
  
      waterValue = analogRead(waterPin);
      if(waterValue < waterLimitLower || waterValue > waterLimitUpper) {
        waterPulse = true;
      }
      else {
        waterPulse = false;
      }
  
      if(waterPulsePrevious && !waterPulse) {
        if(!waterFirst) {
          Serial.println("water,1");
        } else {
          waterFirst = false;
        }
      }
    }
    
    // power
    digitalWrite(powerLed, checkPower);
    if(checkPower) {
      boolean powerPulsePrevious = powerPulse;
      
      powerValue = analogRead(powerPin);
      if(powerValue < powerLimitLower || powerValue > powerLimitUpper) {
        powerPulse = true;
      }
      else {
        powerPulse = false;
      }
      
      if(powerPulsePrevious && !powerPulse) {
        if(!powerFirst) {
          Serial.println("elec,1");
        } else {
          powerFirst = false;
        }
      }
    }

  }
}
