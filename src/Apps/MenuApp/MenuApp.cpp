#include "MenuApp.h"

#include "./Display/Display.h"
#include "./InputHandler/InputHandler.h"
#include "./AppManager/AppManager.h"
extern Display display;
extern InputHandler inputHandler;
extern AppManager appManager;

void MenuApp::init()
{
    display.sprite.fillScreen(TFT_WHITE);
    startEncoder = inputHandler.getEncoderPosition();
}

void MenuApp::update()
{ /*
if(input.joyUp){
if((this->activeItem-3)>=0){
this->activeItem-= 3;
}
else{
this->activeItem = (sizeof(parts)/sizeof(Part))-(3-this->activeItem);
}


}
if(input.joyRight){
if((this->activeItem+1)<=(sizeof(parts)/sizeof(Part))-1){
this->activeItem++;
}
else{
this->activeItem = 0;
}


}
if(input.joyDown){
if((this->activeItem+3)<=(sizeof(parts)/sizeof(Part))-1){
this->activeItem+=3;
}
else{
this->activeItem = 2 - ((sizeof(parts)/sizeof(Part))-(1+this->activeItem)) ;

}


}
if(input.joyLeft){
if((this->activeItem-1)>=0){
this->activeItem-=1;
}
else{
this->activeItem = (sizeof(parts)/sizeof(Part))-1;
}

}
if(input.joyMid){
//appManager.setApp()
// ChangeScreen(this->activeItem+1);
}
this->drawMenu();
*/

    u8_t encoderPos = inputHandler.getEncoderPosition();
    activeItem = (encoderPos - startEncoder) % 8;
    drawMenu();
}

void MenuApp::drawMenu()
{

    display.sprite.fillRect(0, 0, 320, 240, TFT_BLACK);

    display.sprite.setTextFont(0);
    display.sprite.setTextDatum(0);

    display.sprite.setTextColor(TFT_WHITE, TFT_BLACK);
    display.sprite.setTextSize(2);

    int active = this->activeItem;

    for (u8_t i = 0; i < sizeof(parts) / sizeof(parts[0]); i++)
    {
        //TODO write support for multiple screens

        // Check if left or right
        if (i % 2 == 0)
        {

            this->drawPart(8, 8 + 58 * (i / 2), parts[i].id == active, parts[i]);
        }
        // right
        else
        {
            this->drawPart(164, 8 + 58 * (i / 2), parts[i].id == active, parts[i]);
        }
    }

    display.sprite.pushSprite(0, 0);
}

void MenuApp::drawPart(u16_t x, u16_t y, bool active, Part part)
{
    u16_t color = TFT_WHITE;
    if (active)
    {
        color = TFT_GREEN;
    }
    display.sprite.drawRect(x, y, 148, 50, color);
    u8_t width = display.sprite.textWidth(part.name);

    display.sprite.drawString(part.name, x + (74) - (width / 2), y + 20);
    if (display.isRegionTouched({x, y, 95, 95}))
    {
        if (display.getTouchState() == PRESSED)
        {

            appManager.setApp(part.app);
            appManager.menuActive = false;
        }
    }

    if (active && (inputHandler.getButtonState(ENCODER_BUTTON) == BUTTON_PRESSED||inputHandler.getButtonState(RIGHT_BLACKT_BUTTON) == BUTTON_PRESSED))
    {
        
        appManager.setApp(part.app);
            appManager.menuActive = false;

    }
}