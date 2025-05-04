#include "BlockGame.h"

#include "./Display/Display.h"
#include "./InputHandler/InputHandler.h"
#include "./AppManager/AppManager.h"
#include "./Apps/MenuApp/MenuApp.h"

extern Display display;
extern InputHandler inputHandler;
extern AppManager appManager;

extern MenuApp menuApp;

void BlockGameApp::init()
{

    for (int y = 0; y < 20; y++)
    {
        for (int x = 0; x < 10; x++)
        {
            placed[y][x] = 10;
        }
    }
    current = GetPiece(1);

    encoderPos = inputHandler.getEncoderPosition();

    Serial.println("Innit");
}

void BlockGameApp::update()
{
    if ((display.isRegionTouched({0, 0, 60, 40}) && display.getTouchState() == PRESSED) || inputHandler.getButtonState(LEFT_WHITE_BUTTON) == BUTTON_PRESSED)
    {

        appManager.setApp(&menuApp);
    }

    uint32_t newTime = millis();

    if (newTime - oldTime > timePushDown)
    {
        bool can = true;
        for (int blocks = 0; blocks < 4; blocks++)
        {
            if (current.y[blocks] + 1 > 19)
            {
                can = false;

                blocks = 40;
            }
            else if (placed[current.y[blocks] + 1][current.x[blocks]] != 10)
            {
                can = false;

                blocks = 40;
            }
        }
        if (can)
        {
            current.PushDown();
        }

        for (int blocks = 0; blocks < 4; blocks++)
        {
            if (current.y[blocks] + 1 > 19)
            {
                MoveToPlaced();

                blocks = 40;
            }
            else if (placed[current.y[blocks] + 1][current.x[blocks]] != 10)
            {
                MoveToPlaced();

                blocks = 40;
            }
        }

        oldTime = newTime;
    }

    int newEncoderPos = inputHandler.getEncoderPosition();
    if (encoderPos != newEncoderPos)
    {
        encoderPos = newEncoderPos;

        Piece temp(current);
        temp.RotateRight();

        bool can = true;
        for (int blocks = 0; blocks < 4; blocks++)
        {
            if (temp.x[blocks] > 9 || temp.x[blocks] < 0)
            {
                blocks = 40;
                can = false;
            }
            else if (temp.y[blocks] < 0)
            {
                blocks = 40;
                can = false;
            }
            else if (placed[temp.y[blocks]][temp.x[blocks]] != 10)
            {
                blocks = 40;
                can = false;
            }
        }
        if (can)
        {
            current.RotateRight();
        }
    }

    if ((display.isRegionTouched({0, 60, 120, 200}) && display.getTouchState() == PRESSED) || inputHandler.getButtonState(BOTTOM_WHITE2_BUTTON) == BUTTON_PRESSED) // LEFT
    {

        bool can = true;
        for (int blocks = 0; blocks < 4; blocks++)
        {
            if (current.x[blocks] - 1 < 0)
            {
                can = false;
                blocks = 40;
            }
            else if (placed[current.y[blocks]][current.x[blocks] - 1] != 10)
            {
                can = false;

                blocks = 40;
            }
        }
        if (can)
        {
            current.MoveLeft();
        }
    }

    if ((display.isRegionTouched({200, 60, 120, 200}) && display.getTouchState() == PRESSED) || inputHandler.getButtonState(BOTTOM_WHITE1_BUTTON) == BUTTON_PRESSED) // RIGHT
    {

        bool can = true;
        for (int blocks = 0; blocks < 4; blocks++)
        {
            if (current.x[blocks] + 1 > 9)
            {
                can = false;
                blocks = 40;
            }
            else if (placed[current.y[blocks]][current.x[blocks] + 1] != 10)
            {
                can = false;

                blocks = 40;
            }
        }
        if (can)
        {
            current.MoveRight();
        }
    }

    if ((display.isRegionTouched({120, 0, 80, 120}) && display.getTouchState() == PRESSED) || inputHandler.getButtonState(LEFT_GREEN1_BUTTON) == BUTTON_PRESSED) // ROTATE
    {

        Piece temp(current);
        temp.RotateRight();

        bool can = true;
        for (int blocks = 0; blocks < 4; blocks++)
        {
            if (temp.x[blocks] > 9 || temp.x[blocks] < 0)
            {
                blocks = 40;
                can = false;
            }
            else if (temp.y[blocks] < 0)
            {
                blocks = 40;
                can = false;
            }
            else if (placed[temp.y[blocks]][temp.x[blocks]] != 10)
            {
                blocks = 40;
                can = false;
            }
        }
        if (can)
        {
            current.RotateRight();
        }
    }

    if ((display.isRegionTouched({120, 120, 80, 200}) && display.getTouchState() == PRESSED) || inputHandler.getButtonState(BOTTOM_RED_BUTTON) == BUTTON_PRESSED) // FALLDOWN
    {

        bool can = true;
        int diffnum = 0;
        while (can)
        {

            for (int blocks = 0; blocks < 4; blocks++)
            {
                if (current.y[blocks] + 1 > 19)
                {
                    can = false;

                    blocks = 40;
                }
                else if (placed[current.y[blocks] + 1][current.x[blocks]] != 10)
                {
                    can = false;

                    blocks = 40;
                }
            }
            if (can)
            {
                current.PushDown();
                diffnum++;
            }

            for (int blocks = 0; blocks < 4; blocks++)
            {
                if (current.y[blocks] + 1 > 19)
                {
                    MoveToPlaced();
                    can = false;

                    blocks = 40;
                }
                else if (placed[current.y[blocks] + 1][current.x[blocks]] != 10)
                {
                    MoveToPlaced();
                    can = false;

                    blocks = 40;
                }
            }
        }
        score += 2 * diffnum;
    }
    /*
        if (1)
        {


            if (input.joyUp)
            {
                Piece temp(current);
                temp.RotateRight();

                bool can = true;
                for (int blocks = 0; blocks < 4; blocks++)
                {
                    if (temp.x[blocks] > 9 || temp.x[blocks] < 0)
                    {
                        blocks = 40;
                        can = false;
                    }
                    else if (temp.y[blocks] < 0)
                    {
                        blocks = 40;
                        can = false;
                    }
                    else if (placed[temp.y[blocks]][temp.x[blocks]] != 10)
                    {
                        blocks = 40;
                        can = false;
                    }
                }
                if (can)
                {
                    current.RotateRight();
                }
            }
            if (input.joyDown)
            {
                current.PushDown();
                oldTime += 200;
                score += 1;
                for (int blocks = 0; blocks < 4; blocks++)
                {
                    if (current.y[blocks] + 1 > 19)
                    {
                        MoveToPlaced();

                        blocks = 40;
                    }
                    else if (placed[current.y[blocks] + 1][current.x[blocks]] != 10)
                    {
                        MoveToPlaced();

                        blocks = 40;
                    }
                }
            }
            if (input.joyMid)
            {
                bool can = true;
                int diffnum = 0;
                while (can)
                {

                    for (int blocks = 0; blocks < 4; blocks++)
                    {
                        if (current.y[blocks] + 1 > 19)
                        {
                            can = false;

                            blocks = 40;
                        }
                        else if (placed[current.y[blocks] + 1][current.x[blocks]] != 10)
                        {
                            can = false;

                            blocks = 40;
                        }
                    }
                    if (can)
                    {
                        current.PushDown();
                        diffnum++;
                    }

                    for (int blocks = 0; blocks < 4; blocks++)
                    {
                        if (current.y[blocks] + 1 > 19)
                        {
                            MoveToPlaced();
                            can = false;

                            blocks = 40;
                        }
                        else if (placed[current.y[blocks] + 1][current.x[blocks]] != 10)
                        {
                            MoveToPlaced();
                            can = false;

                            blocks = 40;
                        }
                    }
                }
                score += 2 * diffnum;
            }
        }
    */
    drawScreen();
}

void BlockGameApp::MoveToPlaced()
{

    for (int i = 0; i < 4; i++)
    {
        placed[current.y[i]][current.x[i]] = current.shape;
    }
    srand(to_ms_since_boot(get_absolute_time()));
    int shape = rand() % 7;
    current = GetPiece(shape);
    current.MoveRight();
    current.MoveRight();

    int clearedLines = 0;

    for (int y = 0; y < 20; y++)
    {
        bool clearLine = true;
        for (int x = 0; x < 10; x++)
        {
            if (placed[y][x] == 10)
            {
                clearLine = false;
            }
        }

        if (clearLine)
        {

            for (int ly = y; ly > 0; ly--)
            {
                for (int lx = 0; lx < 10; lx++)
                {

                    placed[ly][lx] = placed[ly - 1][lx];
                }
            }
            for (int lx = 0; lx < 10; lx++)
            {
                placed[0][lx] = 10;
            }
            //+Score;
            y--;
            clearedLines++;
        }
    }

    if (clearedLines == 1)
    {
        score += 100 * level;
    }
    else if (clearedLines == 2)
    {
        score += 300 * level;
    }
    else if (clearedLines == 3)
    {
        score += 500 * level;
    }
    else if (clearedLines == 4)
    {
        score += 800 * level;
    }
    lines += clearedLines;
    level = 1 + lines / 10;

    for (int i = 0; i < 4; i++)
    {
        if (placed[current.y[i]][current.x[i]] != 10)
        {

            GameOver();
        }
    }
}

void BlockGameApp::GameOver()
{
    init();
}

void BlockGameApp::drawScreen()
{

    display.sprite.fillRect(0, 0, 320, 240, TFT_BLACK);
    display.sprite.drawRect(0, 0, 122, 240, TFT_YELLOW);

    display.sprite.setTextColor(TFT_WHITE);
    display.sprite.fillRect(270, 40, 50, 100, TFT_BLACK);
    display.sprite.drawString("Score: " + String(score), 170, 50);
    display.sprite.drawString("Level: " + String(level), 170, 75);
    display.sprite.drawString("Lines: " + String(lines), 170, 100);

    for (int y = 0; y < 20; y++)
    {
        for (int x = 0; x < 10; x++)
        {
            if (placed[y][x] != 10)
            {
                display.sprite.drawRect(1 + 12 * x, 12 * y, 12, 12, GetPieceColor(placed[y][x]));
            }
            else
            {
                display.sprite.drawRect(1 + 12 * x, 12 * y, 12, 12, display.sprite.color565(36, 36, 36));
            }
        }
    }

    for (int i = 0; i < 4; i++)
    {
        display.sprite.drawRect(1 + 12 * this->current.x[i], 12 * this->current.y[i], 12, 12, GetPieceColor(this->current.shape));
    }

    display.sprite.pushSprite(0, 0);
}

Piece BlockGameApp::GetPiece(int type)
{

    Piece piece = Piece();

    piece.shape = type;
    if (type == 0) // O
    {
        piece.x[0] = 0;
        piece.y[0] = 0;

        piece.x[1] = 1;
        piece.y[1] = 0;

        piece.x[2] = 0;
        piece.y[2] = 1;

        piece.x[3] = 1;
        piece.y[3] = 1;
    }
    else if (type == 1) // I
    {
        piece.x[0] = 0;
        piece.y[0] = 0;

        piece.x[1] = 1;
        piece.y[1] = 0;

        piece.x[2] = 2;
        piece.y[2] = 0;

        piece.x[3] = 3;
        piece.y[3] = 0;
    }
    else if (type == 2) // Z
    {
        piece.x[0] = 0;
        piece.y[0] = 0;

        piece.x[1] = 1;
        piece.y[1] = 0;

        piece.x[2] = 1;
        piece.y[2] = 1;

        piece.x[3] = 2;
        piece.y[3] = 1;
    }
    else if (type == 3) // S
    {
        piece.x[0] = 2;
        piece.y[0] = 0;

        piece.x[1] = 1;
        piece.y[1] = 0;

        piece.x[2] = 1;
        piece.y[2] = 1;

        piece.x[3] = 0;
        piece.y[3] = 1;
    }
    else if (type == 4) // J
    {
        piece.x[0] = 0;
        piece.y[0] = 1;

        piece.x[1] = 1;
        piece.y[1] = 1;

        piece.x[2] = 2;
        piece.y[2] = 1;

        piece.x[3] = 1;
        piece.y[3] = 0;
    }
    else if (type == 5) // L
    {
        piece.x[0] = 0;
        piece.y[0] = 1;

        piece.x[1] = 1;
        piece.y[1] = 1;

        piece.x[2] = 2;
        piece.y[2] = 1;

        piece.x[3] = 2;
        piece.y[3] = 0;
    }
    else if (type == 6) // T
    {
        piece.x[0] = 0;
        piece.y[0] = 1;

        piece.x[1] = 1;
        piece.y[1] = 1;

        piece.x[2] = 2;
        piece.y[2] = 1;

        piece.x[3] = 0;
        piece.y[3] = 0;
    }

    return piece;
}

int BlockGameApp::GetPieceColor(int type)
{
    switch (type)
    {
    case 0:
        return (TFT_YELLOW);
        break;
    case 1:
        return (TFT_SKYBLUE);
        break;
    case 2:
        return (TFT_RED);
        break;
    case 3:
        return (TFT_GREEN);
        break;
    case 4:
        return (display.tft.color565(193, 71, 233));
        break;
    case 5:
        return (TFT_ORANGE);
        break;
    case 6:
        return (display.tft.color565(0, 0, 255));
        break;
    case 10:
        return (TFT_BLACK);
        break;
    }
    return 0;
}
