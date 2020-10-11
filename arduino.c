////////////////////////////////////////////////////////////////////////
//
// Chorded Keyboard Arduino Code
//
// Yonathan Vardi 2020.09.30
//
////////////////////////////////////////////////////////////////////////

#include "Keyboard.h"

#define RESOLVE_T 100   //temporary value, seems better than 50
#define COMB 136        //the arbitrary non-ascii character that a combo represents
#define COMBO_MAX_SIZE 20   //the maximum number of keys in a combo. Should not exceed 255.
#define NOINPUT 137     //arbitrary value of no-input

int comboOn = 0;
unsigned char combo[COMBO_MAX_SIZE];
int comboIndex = 0;
unsigned char outputKeystates[256] = {0};

unsigned char encoding[512] = { //the chord to key encoding table
        NOINPUT, KEY_RIGHT_ARROW, KEY_UP_ARROW, ' ', KEY_DOWN_ARROW, KEY_DELETE, NOINPUT, NOINPUT, 
        KEY_LEFT_ARROW, KEY_RETURN, KEY_BACKSPACE, NOINPUT, KEY_TAB, NOINPUT, NOINPUT, COMB, 
        '1', NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, 
        NOINPUT, NOINPUT, '\\', NOINPUT, 'z', NOINPUT, 'q', NOINPUT, 
        '2', 'e', NOINPUT, NOINPUT, 'a', 't', NOINPUT, NOINPUT, 
        'h', 'k', NOINPUT, NOINPUT, 'd', 'u', NOINPUT, NOINPUT, 
        '3', NOINPUT, NOINPUT, NOINPUT, 'i', NOINPUT, NOINPUT, NOINPUT, 
        's', NOINPUT, NOINPUT, NOINPUT, 'j', NOINPUT, NOINPUT, NOINPUT, 
        '4', 'g', 'p', 'b', NOINPUT, NOINPUT, NOINPUT, NOINPUT, 
        'r', 'c', 'v', 'l', NOINPUT, NOINPUT, NOINPUT, NOINPUT, 
        '5', NOINPUT, 'y', NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, 
        'w', NOINPUT, 'x', NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, 
        '6', 'o', NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, 
        'f', 'n', NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, 
        '7', NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, 
        'm', NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, 
        '8', '.', ',', ']', ';', '\'', '[', '/', 
        NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, 
        '9', NOINPUT, KEY_LEFT_GUI, NOINPUT, KEY_RIGHT_GUI, NOINPUT, '`', NOINPUT, 
        NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, 
        '0', NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, 
        NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, 
        KEY_LEFT_ALT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, 
        NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, 
        '-', NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, 
        NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, 
        KEY_LEFT_CTRL, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, 
        NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, 
        KEY_ESC, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, 
        NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, 
        KEY_CAPS_LOCK, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, 
        NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT, NOINPUT
    }; 

////////////////////////////////////////////////////////////////////////

int getKeystate1h() {
    return digitalRead(7) == LOW;
}
int getKeystate2h() {
    return digitalRead(6) == LOW;
}
int getKeystate3h() {
    return digitalRead(8) == LOW;
}
int getKeystate4h() {
    return digitalRead(9) == LOW;
}
int getKeystate1l() {
    return digitalRead(3) == LOW;
}
int getKeystate2l() {
    return digitalRead(2) == LOW;
}
int getKeystate3l() {
    return digitalRead(4) == LOW;
}
int getKeystate4l() {
    return digitalRead(5) == LOW;
}
int getKeystateShift() {
    return digitalRead(11) == LOW;
}

////////////////////////////////////////////////////////////////////////

void press(unsigned char key) {
    Keyboard.write(key);
}

void keydown(unsigned char key) {
    Keyboard.press(key);
    outputKeystates[key] |= 1;  //mark as down
}

void keyup(unsigned char key) {
    Keyboard.release(key);
    outputKeystates[key] &= ~1; //mark as up
}

unsigned int getKeystates() {
    /* keystates = 0b0 0000 0000 
                     |    |   ||
                    shift |   |1 low
                        1high  2 low*/
    unsigned int keystates = 0;
    keystates ^= (getKeystate1l() << 0);
    keystates ^= (getKeystate2l() << 1);
    keystates ^= (getKeystate3l() << 2);
    keystates ^= (getKeystate4l() << 3);

    keystates ^= (getKeystate1h() << 4);
    keystates ^= (getKeystate2h() << 5);
    keystates ^= (getKeystate3h() << 6);
    keystates ^= (getKeystate4h() << 7);

    // keystates ^= (getKeystateShift() << 8);
    if (getKeystateShift()) {
        if (!outputKeystates[KEY_LEFT_SHIFT]) {
            keydown(KEY_LEFT_SHIFT);
        }
    } else if (outputKeystates[KEY_LEFT_SHIFT]) {
        keyup(KEY_LEFT_SHIFT);
    }
    return keystates;
}

unsigned char resolve(unsigned int chord) {
    /* This function resolves a chord to a character */
    return encoding[chord];
}

void addToCombo(unsigned char key) {
    //adds a key to the combo without pressing it
    if (comboIndex < COMBO_MAX_SIZE) {
        combo[comboIndex++] = key;
        outputKeystates[key] |= (1 << 1);   //turn on second bit to show it being in combo
        return;
    }
}
void clearCombo() {
    for (unsigned char i = 0; i < comboIndex; ++i) {
        outputKeystates[combo[i]] &= ~(1 << 1);  //turn off second bit to mark it as no longer in combo
    }
    comboIndex = 0;
}

void pressCombo() {
    //presses all the keys in combo
    for (unsigned char i = 0; i < comboIndex; ++i) {
        press(combo[i]);
    }
}
void keydownCombo() {
    //presses all the keys in combo
    for (unsigned char i = 0; i < comboIndex; ++i) {
        keydown(combo[i]);
    }
}
void keyupCombo() {
    //presses all the keys in combo
    for (unsigned char i = 0; i < comboIndex; ++i) {
        keyup(combo[i]);
    }
}

void resolveAndPress(unsigned int chord) {
    // printf("press "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(chord));
    unsigned char key = resolve(chord);
    if (key == NOINPUT) {
        return;
    }
    if (comboOn == 1) {
        if (key == COMB) {
            comboOn = 0;
            pressCombo();
            return;
        } else {
            addToCombo(key);
            return;
        }
    }
    if (key == COMB) {
        comboOn = 1;
        clearCombo();
        return;
    }
    press(key);
}
void resolveAndKeydown(unsigned int chord) {
    unsigned char key = resolve(chord);
    if (key == NOINPUT) {
        return;
    }
    if (comboOn == 1) {
        if (key == COMB) {
            comboOn = 0;
            keydownCombo();
            return;
        } else {
            addToCombo(key);
            return;
        }
    }
    if (key == COMB) {
        comboOn = 1;
        clearCombo();
        return;
    }
    keydown(key);
}
void resolveAndKeyup(unsigned int chord) {
    unsigned char key = resolve(chord);
    if (key == NOINPUT) {
        return;
    }
    if (comboOn == 1) {
        return;
    }
    if (key == COMB) {
        keyupCombo();
        return;
    }
    keyup(key);
}

////////////////////////////////////////////////////////////////////////

void setup() {
    pinMode(13, OUTPUT);  //for debugging
    
    pinMode(2, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP);
    pinMode(5, INPUT_PULLUP);  
    pinMode(6, INPUT_PULLUP);
    pinMode(7, INPUT_PULLUP);
    pinMode(8, INPUT_PULLUP);
    pinMode(9, INPUT_PULLUP);
    pinMode(11, INPUT_PULLUP);

    //begin the keyboard
    Keyboard.begin();
}

////////////////////////////////////////////////////////////////////////

void loop() {
    unsigned int chord = 0;
    while (getKeystates() == 0) {}; //wait for press

    unsigned long startTime = millis();    //startTimer()
    while (millis() - startTime < RESOLVE_T) { //while elapsed time < allotted time
        unsigned int keystates = getKeystates();
        chord |= keystates; //chord U keystates
        if (keystates == 0) {   //if no buttons pressed
            resolveAndPress(chord);
            return;
        }
    }

    unsigned int initialChord;    
    hold: //////////////////////////////////////////////////////////////
    initialChord = chord;
    resolveAndKeydown(chord);

    while (initialChord == chord) {chord = getKeystates();} //wait for a change in chord

    startTime = millis();    //startTimer()
    while (millis() - startTime < RESOLVE_T) { //while elapsed time < allotted time
        if (getKeystates() == 0) {
            resolveAndKeyup(initialChord);
            return;
        }
    }
    //if did not unpress but rather changed chord
    chord = getKeystates();
    resolveAndKeyup(initialChord);
    goto hold;
}
