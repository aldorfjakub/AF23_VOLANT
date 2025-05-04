#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#define BOTTOM_RED_BUTTON 0
#define BOTTOM_WHITE1_BUTTON 1
#define BOTTOM_WHITE2_BUTTON 2

#define LEFT_GREEN1_BUTTON 3
#define LEFT_GREEN2_BUTTON 4
#define LEFT_WHITE_BUTTON 5

#define MIDDLE_BLUE1_BUTTON 6
#define MIDDLE_BLUE2_BUTTON 7


#define MIDDLE_YELLOWT_BUTTON 8
#define MIDDLE_YELLOWB_BUTTON 9

#define RIGHT_BLUET_BUTTON 10
#define RIGHT_BLUEM_BUTTON 11
#define RIGHT_BLUEB_BUTTON 12


#define RIGHT_BLACKT_BUTTON 13
#define RIGHT_BLACKB_BUTTON 14

#define ENCODER_BUTTON 15






#include <Arduino.h>

#include <Rotary.h>  
#include <vector>   
#include <functional> 

//Various input types
enum InputEventType{
    BUTTON_PRESS,
    BUTTON_RELEASE,
    ROTATE_CW,
    ROTATE_CCW
};

enum ButtonStateType { BUTTON_UNPRESSED, BUTTON_PRESSED, BUTTON_HOLDING };

//Input event structure
struct InputEvent {
    InputEventType type;
    uint8_t inputId; //ID of input source | In later versions will be replaced with named enum 
};

//Defition for event callback
typedef std::function<void(InputEvent)> InputEventCallback;


class InputHandler {
public:
  void init();                          

            
  void registerCallback(InputEventCallback callback);
  bool isButtonPressed(uint8_t buttonId);
  ButtonStateType getButtonState(uint8_t buttonId);
  int getEncoderPosition();


    //Used by the interupt
    void handleEncoderRotation();
    void update();


private:
    struct ButtonState {
        ButtonStateType state = BUTTON_UNPRESSED;
        uint32_t pressTime = 0;
        bool isPressed =false; 
    };

    // Internal state for each encoder
    struct EncoderState {
        int position;    
        Rotary* rotary; 
    EncoderState() : position(0), rotary(nullptr) {}
 
    };

        // Internal helper for handling button state changes
    void handleButtonChange(uint8_t buttonId, bool pressed);

    void readButtons();
    
    

    //List of states
    std::vector<ButtonState> buttons;  
    EncoderState encoder; 
    std::vector<InputEventCallback> callbacks;
    

};

#endif
