#include "rotatemenu.h"

RotateMenu::RotateMenu()
{
    //ctor
}

RotateMenu::RotateMenu(int width, int height)
    : m_screenWidth(width), m_screenHeight(height)
{
    int space = m_screenWidth / 4;
    int w = space - 20;
    m_xrot = {"X ROTATION", 10, m_screenHeight-35, w, 25, BLACK, "", false};
    m_yrot = {"Y ROTATION", space + 10, m_screenHeight-35, w, 25, BLACK, "", false};
    m_zrot = {"Z ROTATION", (space * 2) + 10, m_screenHeight-35, w, 25, BLACK, "", false};
    m_update = {"UPDATE",(space * 3) + 10, m_screenHeight-35, w, 25 };
    m_close = {"CLOSE", 10, m_screenHeight - 80, m_screenWidth - 20,25};

}

RotateMenu::~RotateMenu()
{
    //dtor
}


void RotateMenu::handleInput(STATE * currentState, std::vector<MODELX> & models)
{
   if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Vector2 mpos = GetMousePosition();


        if(CheckCollisionPointRec(mpos, buttonRectangle(m_close)))
        {
            *currentState = MAIN_STATE;
            setVisible(false);
        }

        if(CheckCollisionPointRec(mpos, buttonRectangle(m_update)))
        {
            changeSelectedModelRotation(m_xrot.contents,m_yrot.contents,m_zrot.contents,models);
        }

        if(CheckCollisionPointRec(mpos, textBoxRectangle(m_xrot)))
        {
            setTextBoxSelection(m_xrot, true);
            setTextBoxSelection(m_yrot, false);
            setTextBoxSelection(m_zrot, false);

        }
        else if(CheckCollisionPointRec(mpos, textBoxRectangle(m_yrot)))
        {
            setTextBoxSelection(m_xrot, false);
            setTextBoxSelection(m_yrot, true);
            setTextBoxSelection(m_zrot, false);
        }
        else if(CheckCollisionPointRec(mpos, textBoxRectangle(m_zrot)))
        {
            setTextBoxSelection(m_xrot, false);
            setTextBoxSelection(m_yrot, false);
            setTextBoxSelection(m_zrot, true);
        }
        else
        {
            setTextBoxSelection(m_xrot, false);
            setTextBoxSelection(m_yrot, false);
            setTextBoxSelection(m_zrot, false);
        }

    }

    int key = GetCharPressed();
    if ((key >= 32) && (key <= 125))
    {
        if(m_xrot.selected)
            m_xrot.contents += (char)key;
        else if(m_yrot.selected)
            m_yrot.contents += (char)key;
        else if(m_zrot.selected)
            m_zrot.contents += (char)key;
    }

    if(IsKeyPressed(KEY_BACKSPACE))
    {
        if(m_xrot.selected && (m_xrot.contents.size() > 0))
            m_xrot.contents.pop_back();
        else if(m_yrot.selected && (m_yrot.contents.size() > 0))
            m_yrot.contents.pop_back();
        else if(m_zrot.selected && (m_zrot.contents.size() > 0))
            m_zrot.contents.pop_back();

    }
}

void RotateMenu::drawMenu(void)
{
    DrawRectangleLines( m_xrot.xpos, m_xrot.ypos, m_xrot.width, m_xrot.height, m_xrot.boxColor);
    DrawText(m_xrot.name, m_xrot.xpos, m_xrot.ypos - 15, 10, BLACK );
    DrawText(m_xrot.contents.c_str(), m_xrot.xpos + 5, m_xrot.ypos + 8, 10, BLACK );

    DrawRectangleLines( m_yrot.xpos, m_yrot.ypos, m_yrot.width, m_yrot.height, m_yrot.boxColor);
    DrawText(m_yrot.name, m_yrot.xpos, m_yrot.ypos - 15, 10, BLACK );
    DrawText(m_yrot.contents.c_str(), m_yrot.xpos + 5, m_yrot.ypos + 8, 10, BLACK );

    DrawRectangleLines( m_zrot.xpos, m_zrot.ypos, m_zrot.width, m_zrot.height, m_zrot.boxColor);
    DrawText(m_zrot.name, m_zrot.xpos, m_zrot.ypos - 15, 10, BLACK );
    DrawText(m_zrot.contents.c_str(), m_zrot.xpos + 5, m_zrot.ypos + 8, 10, BLACK );

    DrawRectangle(m_update.xpos, m_update.ypos, m_update.width, m_update.height, GRAY);
    DrawText(m_update.name, (m_update.xpos + (m_update.width/2) - 20), m_update.ypos + 8, 10, WHITE);

    DrawRectangle(m_close.xpos, m_close.ypos, m_close.width, m_close.height, GRAY);
    DrawText(m_close.name, (m_close.xpos + (m_close.width/2) - 20), m_close.ypos + 8, 10, WHITE);


}
