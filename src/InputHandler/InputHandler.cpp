#include "InputHandler.h"

#define ENCODER_A 28
#define ENCODER_B 27

#define PISO_DATA 4
#define PISO_CLOCK 26
#define PISO_LATCH 14

#define HOLD_THRESHOLD 30
#include "./SerialLinkDriver/SerialLinkDriver.h"
#include "./Apps/Timer/TimerApp.h"
#include "./Apps/MenuApp/MenuApp.h"

#include "./AppManager/AppManager.h"




extern InputHandler inputHandler;
extern SerialLinkDriver serialLinkDriver;
extern TimerApp timerApp;
extern AppManager appManager;

extern MenuApp menuApp;




void encoderISR1() { inputHandler.handleEncoderRotation(); }

void InputHandler::init()
{

    pinMode(PISO_DATA, INPUT);
    pinMode(PISO_CLOCK, OUTPUT);
    pinMode(PISO_LATCH, OUTPUT);

    buttons.assign(16, ButtonState());

    // Init the encoder
    encoder.rotary = new Rotary(28, 27);
    encoder.position = 0;
    attachInterrupt(digitalPinToInterrupt(ENCODER_A), encoderISR1, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_B), encoderISR1, CHANGE);
}

void InputHandler::registerCallback(InputEventCallback callback)
{
    callbacks.push_back(callback);
}

bool InputHandler::isButtonPressed(uint8_t buttonId)
{
    readButtons();
    //Serial.println(buttons.size());

    if (buttonId < buttons.size())
    { // Check if buttonId is valid
        return buttons[buttonId].state == BUTTON_PRESSED || buttons[buttonId].state == BUTTON_HOLDING;
    }
    return false;
}


void InputHandler::update(){
     if (inputHandler.getButtonState(RIGHT_BLUET_BUTTON) == BUTTON_PRESSED)
    {
        serialLinkDriver.changeSpeedMode(2);
    }
    else if (inputHandler.getButtonState(RIGHT_BLUEM_BUTTON) == BUTTON_PRESSED)
    {
        serialLinkDriver.changeSpeedMode(1);
    }
    else if (inputHandler.getButtonState(RIGHT_BLUEB_BUTTON) == BUTTON_PRESSED)
    {
        serialLinkDriver.changeSpeedMode(0);
    }


     if (inputHandler.getButtonState(LEFT_WHITE_BUTTON) == BUTTON_PRESSED)
    {
        serialLinkDriver.changeRideMode(0);
    }
    else if (inputHandler.getButtonState(LEFT_GREEN2_BUTTON) == BUTTON_PRESSED)
    {
        serialLinkDriver.changeRideMode(1);
    }
    else if (inputHandler.getButtonState(LEFT_GREEN1_BUTTON) == BUTTON_PRESSED)
    {
        serialLinkDriver.changeRideMode(2);
    }


//Fact check correct buttons
    if(inputHandler.getButtonState(MIDDLE_BLUE1_BUTTON)== BUTTON_PRESSED){//LEFT
        serialLinkDriver.setLedMode(10);

    }
    else if (inputHandler.getButtonState(MIDDLE_YELLOWB_BUTTON)== BUTTON_PRESSED){//RIGHT
        serialLinkDriver.setLedMode(20);

    }
    else if (inputHandler.getButtonState(BOTTOM_RED_BUTTON)== BUTTON_PRESSED){
        serialLinkDriver.setLedMode(30);

    }

        if(inputHandler.getButtonState(BOTTOM_WHITE1_BUTTON)== BUTTON_PRESSED){//LEFT
        timerApp.lap();

    }
    else if (inputHandler.getButtonState(BOTTOM_WHITE2_BUTTON)== BUTTON_PRESSED){//RIGHT
        timerApp.startStop();

    }

    if(inputHandler.getButtonState(RIGHT_BLACKB_BUTTON)==BUTTON_PRESSED&&appManager.menuActive == false){
        appManager.setApp(&menuApp);
        appManager.menuActive = true;
    }

    if(inputHandler.getButtonState(MIDDLE_BLUE2_BUTTON)==BUTTON_PRESSED){
        serialLinkDriver.changeBreak(false);
    }
    else if (inputHandler.getButtonState(MIDDLE_YELLOWT_BUTTON)==BUTTON_PRESSED)
    {
        serialLinkDriver.changeBreak(true);
        
    }
    
}

ButtonStateType InputHandler::getButtonState(uint8_t buttonId)
{

    readButtons();
    //Serial.println(buttons.size());

    if (buttonId < buttons.size())
    { // Check if buttonId is valid
        return buttons[buttonId].state;
    }

    return BUTTON_UNPRESSED;
}
int InputHandler::getEncoderPosition()
{
    return encoder.position;
}

void InputHandler::readButtons()
{

    int bitCount = 15;

    digitalWrite(PISO_LATCH, LOW);
    digitalWrite(PISO_LATCH, HIGH);

    uint32_t currentTime = millis();

    for (int i = 0; i < bitCount; i++)
    {
        int bit = digitalRead(PISO_DATA);

        if (bit == 1)
        { // Button is pressed
            if (!buttons[i].isPressed)
            { // Transition from UNPRESSED to PRESSED
                buttons[i].state = BUTTON_PRESSED;
                buttons[i].pressTime = currentTime;
                Serial.println(i);
            }
            else if (currentTime - buttons[i].pressTime > HOLD_THRESHOLD)
            { // Holding
                buttons[i].state = BUTTON_HOLDING;
            }
        }
        else
        { // Button is not pressed
            buttons[i].state = BUTTON_UNPRESSED;
            buttons[i].pressTime = 0;
        }

        buttons[i].isPressed = (bit == 1);

        digitalWrite(PISO_CLOCK, HIGH); // Shift out the next bit
        digitalWrite(PISO_CLOCK, LOW);
    }
}

void InputHandler::handleEncoderRotation()
{
    unsigned char result = encoder.rotary->process();

    if (result == DIR_CW)
    {
        encoder.position++;
        for (auto &callback : callbacks)
        {
            callback({ROTATE_CW, 0});
        }
    }
    else if (result == DIR_CCW)
    {
        encoder.position--;
        for (auto &callback : callbacks)
        {
            callback({ROTATE_CCW, 0});
        }
    }
}
