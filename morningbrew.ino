//HARDWARE INPUT AND OUTPUT PINS
const int red = D3;
const int green = D2;
const int blue = D1;

const int heat = D7;
const int touch = D0;

const int onButton = D4;
const int offButton = D5;

const int touchSafe = A4;
const int touchUnsafe = A5;

const int decaf = A1;
const int medium = A2;
const int dark = A3;

//STATE TRACKER STRINGS
String status = "";
String preference = "";
String touchTemp = "";

//DELTA TIMING
unsigned long previousTime = 0;
unsigned long deltaTime = 350;

unsigned long autoClose = 0;

//SETUP PARTICLE
void setup() {
    Serial.write(9600);
    hardwareSetup();

    //ALL JS FUNCTIONS
    Particle.function("toDecaf", toDecaf);
    Particle.function("toMedium", toMedium);
    Particle.function("toDark", toDark);

    Particle.function("toOn", toOn);
    Particle.function("toOff", toOff);
    Particle.function("toTimer", toTimer);

    Particle.function("writeStatus", writeStatus);

    readMachine();
}

void readMachine() {
    /*
    if(onButtonPressed()) {
        status = "on";
    } else if (offButtonPressed()) {
        status = "off";
    }

    if(decafPressed()) {
        preference = "decaf";
    } else if(mediumPressed()) {
        preference = "medium";
    } else if(darkPressed()) {
        preference = "dark";
    }

    if(toTouchSafe()) {
        touchTemp = "safe";
    } else if (toTouchUnsafe()) {
        touchTemp = "unsafe";
    }
    */


    //SETS INITIAL STATE, BUT WOULD READ STATES
    //IF CONNECTED TO ACTUAL COFFEE MACHINE
    status = "off";
    preference = "decaf";
    touchTemp = "safe";

    stubOff();
    stubDecaf();
    stubTouchOff();
}

//ALL JS FUNCTIONS
//HANDLES WITH JS TIMER INPUT
int toTimer(String data) {
    int val = data.toInt();
    int closeTime = val*1000;

    autoClose = millis();
    while(millis() - autoClose < closeTime) {
        //check for any changes
    }
    updateOnStatus();
}

//SENDS CURRENT STATUS TO UI SO IT CAN UPDATE BASED ON PARTICLE STATES
int writeStatus(String x) {
    if(status == "on") {
        Particle.publish("on", "on");
    } else if (status == "off") {
        Particle.publish("off", "off");
    }

    if(preference == "decaf") {
        Particle.publish("decaf", "decaf");
    } else if(preference == "medium") {
        Particle.publish("medium", "medium");
    } else if(preference == "dark") {
        Particle.publish("dark", "dark");
    }

    if(touchTemp == "safe") {
        Particle.publish("safe", "safe");
    } else if (touchTemp == "unsafe") {
        Particle.publish("unsafe", "unsafe");
    }

}

//JS FUNCTION FOR DECAF
int toDecaf(String x) {
    updateDecaf();
}

//JS FUNCTION FOR MEDIUM
int toMedium(String x) {
    updateMedium();
}

//JS FUNCTION FOR DARK
int toDark(String x) {
    updateDark();
}

//JS FUNCTION FOR ON
int toOn(String x) {
    updateOnStatus();
}

//JS FUNCTION FOR OFF
int toOff(String x) {
    updateOffStatus();
}
//DONE WITH JS FUNCTIONS


//LOOP TO CHECK AND CHANGE STATES ACCORDINGLY
void loop() {

    if(decafPressed() && status == "off") {
        updateDecaf();
    } else if(mediumPressed() && status == "off") {
        updateMedium();
    } else if(darkPressed() && status == "off") {
        updateDark();
    }

    if(onButtonPressed()) {
        updateOnStatus();
    } else if(offButtonPressed()) {
        updateOffStatus();
    }

    if(toTouchSafe()) {
        updateTouchSafe();
    } else if(toTouchUnsafe()) {
        updateTouchUnsafe();
    }
}


//BOOLEAN FUNCTIONS, TAKE HARDWARE INPUT
//CHECK ON BUTTON PRESSED
boolean onButtonPressed() {
    if(digitalRead(onButton) == 0) {
        return true;
    }
    return false;
}

//CHECK OFF BUTTON PRESSED
boolean offButtonPressed() {
    if(digitalRead(offButton) == 0) {
        return true;
    }
    return false;
}

//CHECK DECAF BUTTON PRESSED
boolean decafPressed() {
    if(digitalRead(decaf) == 0) {
        return true;
    }
    return false;
}

//CHECK MEDIUM BUTTON PRESSED
boolean mediumPressed() {
    if(digitalRead(medium) == 0) {
        return true;
    }
    return false;
}

//CHECK DARK BUTTON PRESSED
boolean darkPressed() {
    if(digitalRead(dark) == 0) {
        return true;
    }
    return false;
}

//CHECK SAFE BUTTON PRESSED
boolean toTouchSafe() {
    if(digitalRead(touchSafe) == 0) {
        return true;
    }
    return false;
}

//CHECK UNSAFE BUTTON PRESSED
boolean toTouchUnsafe() {
    if(digitalRead(touchUnsafe) == 0) {
        return true;
    }
    return false;
}
//DONE WITH BOOLEAN FUNCTIONS


//UPDATE FUNCTIONS, TAKE NO HARDWARE INPUT, GIVE NO OUTPUT
//UPDATE STATE TO ON
void updateOnStatus() {
    Particle.publish("on", "on");
    status = "on";

    stubOn();
    stubPreference();
}

//UPDATE STATE TO OFF
void updateOffStatus() {
    Particle.publish("off", "off");
    status = "off";

    stubOff();
    if(preference == "decaf") {
        stubDecaf();
    } else if (preference == "medium") {
        stubMedium();
    } else if (preference == "dark") {
        stubDark();
    }
}

//UPDATE STATE TO DECAF
void updateDecaf() {
    Particle.publish("decaf", "decaf");
    preference = "decaf";
    stubDecaf();
}

//UPDATE STATE TO MEDIUM
void updateMedium() {
    Particle.publish("medium", "medium");
    preference = "medium";
    stubMedium();
}

//UPDATE STATE TO DARK
void updateDark() {
    Particle.publish("dark", "dark");
    preference = "dark";
    stubDark();
}

//UPDATE STATE TO SAFE
void updateTouchSafe() {
    Particle.publish("safe", "safe");
    touchTemp = "safe";
    stubTouchOff();
}

//UPDATE STATE TO UNSAFE
void updateTouchUnsafe() {
    Particle.publish("unsafe", "unsafe");
    touchTemp = "unsafe";
    stubTouchOn();
}
//DONE WITH UPDATE FUNCTIONS


//STUB FUNCTIONS, GIVE HARDWARE OUTPUT
//HARDWARE OUTPUT FOR DECAF
void stubDecaf() {
    analogWrite(red, 0);
    analogWrite(green, 255);
    analogWrite(blue, 255);
}

//HARDWARE OUTPUT FOR MEDIUM
void stubMedium() {
    analogWrite(red, 255);
    analogWrite(green, 255);
    analogWrite(blue, 0);
}

//HARDWARE OUTPUT FOR DARK
void stubDark() {
    analogWrite(red, 255);
    analogWrite(green, 0);
    analogWrite(blue, 255);
}

//HARDWARE OUTPUT FOR NOT ABLE TO CHANGE PREFERENCE BECASE MACHINE IS ON
void stubPreference() {
    analogWrite(red, 255);
    analogWrite(green, 255);
    analogWrite(blue, 255);
}

//HARDWARE OUTPUT FOR MACHINE ON
void stubOn() {
    digitalWrite(heat, HIGH);
}

//HARDWARE OUTPUT FOR MACHINE OFF
void stubOff() {
    digitalWrite(heat, LOW);
}

//HARDWARE OUTPUT FOR TOUCH SAFE ON
void stubTouchOn() {
    digitalWrite(touch, HIGH);
}

//HARDWARE OUTPUT FOR TOUCH UNSAFE ON
void stubTouchOff() {
    digitalWrite(touch, LOW);
}

//PROPER SETUP FOR ALL HARDWARE
void hardwareSetup() {
    pinMode(red, OUTPUT);
    pinMode(green, OUTPUT);
    pinMode(blue, OUTPUT);

    pinMode(heat, OUTPUT);
    pinMode(touch, OUTPUT);

    pinMode(onButton, INPUT_PULLUP);
    pinMode(offButton, INPUT_PULLUP);
    pinMode(touchSafe, INPUT_PULLUP);
    pinMode(touchUnsafe, INPUT_PULLUP);

    pinMode(decaf, INPUT_PULLUP);
    pinMode(medium, INPUT_PULLUP);
    pinMode(dark, INPUT_PULLUP);
}
