#include "scalemenu.h"

ScaleMenu::ScaleMenu()
{
    //ctor
}

ScaleMenu::ScaleMenu(int width, int height)
    : m_screenWidth(width), m_screenHeight(height)
{
    int space = m_screenWidth / 4;
    int w = space - 20;
    m_xscale = {"X SCALE", 10, m_screenHeight-35, w, 25, BLACK, "", false};
    m_yscale = {"Y SCALE", space + 10, m_screenHeight-35, w, 25, BLACK, "", false};
    m_zscale = {"Z SCALE", (space * 2) + 10, m_screenHeight-35, w, 25, BLACK, "", false};
    m_update = {"UPDATE",(space * 3) + 10, m_screenHeight-35, w, 25 };
    m_close = {"CLOSE", 10, m_screenHeight - 80, m_screenWidth - 20,25};

}

ScaleMenu::~ScaleMenu()
{
    //dtor
}


void ScaleMenu::handleInput(STATE * currentState,COLLISION & mesh, std::vector<MODELX> & models)
{
   if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Vector2 mpos = GetMousePosition();


        if(CheckCollisionPointRec(mpos, buttonRectangle(m_close)))
        {
            *currentState = MAIN_STATE;
            setVisible(false);
            mesh.hitInfo.hit = false;
        }

        if(CheckCollisionPointRec(mpos, buttonRectangle(m_update)))
        {
            changeSelectedModelScale(stringsToVector3(m_xscale.contents,m_yscale.contents,m_zscale.contents),models);
            mesh.hitInfo.hit = false;
        }

        if(CheckCollisionPointRec(mpos, textBoxRectangle(m_xscale)))
        {
            setTextBoxSelection(m_xscale, true);
            setTextBoxSelection(m_yscale, false);
            setTextBoxSelection(m_zscale, false);
            mesh.hitInfo.hit = false;

        }
        else if(CheckCollisionPointRec(mpos, textBoxRectangle(m_yscale)))
        {
            setTextBoxSelection(m_xscale, false);
            setTextBoxSelection(m_yscale, true);
            setTextBoxSelection(m_zscale, false);
            mesh.hitInfo.hit = false;
        }
        else if(CheckCollisionPointRec(mpos, textBoxRectangle(m_zscale)))
        {
            setTextBoxSelection(m_xscale, false);
            setTextBoxSelection(m_yscale, false);
            setTextBoxSelection(m_zscale, true);
            mesh.hitInfo.hit = false;
        }
        else
        {
            setTextBoxSelection(m_xscale, false);
            setTextBoxSelection(m_yscale, false);
            setTextBoxSelection(m_zscale, false);
        }

    }

    int key = GetCharPressed();
    if ((key >= 32) && (key <= 125))
    {
        if(m_xscale.selected)
            m_xscale.contents += (char)key;
        else if(m_yscale.selected)
            m_yscale.contents += (char)key;
        else if(m_zscale.selected)
            m_zscale.contents += (char)key;
    }

    if(IsKeyPressed(KEY_BACKSPACE))
    {
        if(m_xscale.selected && (m_xscale.contents.size() > 0))
            m_xscale.contents.pop_back();
        else if(m_yscale.selected && (m_yscale.contents.size() > 0))
            m_yscale.contents.pop_back();
        else if(m_zscale.selected && (m_zscale.contents.size() > 0))
            m_zscale.contents.pop_back();

    }
}

void ScaleMenu::drawMenu(void)
{
    DrawRectangleLines( m_xscale.xpos, m_xscale.ypos, m_xscale.width, m_xscale.height, m_xscale.boxColor);
    DrawText(m_xscale.name, m_xscale.xpos, m_xscale.ypos - 15, 10, BLACK );
    DrawText(m_xscale.contents.c_str(), m_xscale.xpos + 5, m_xscale.ypos + 8, 10, BLACK );

    DrawRectangleLines( m_yscale.xpos, m_yscale.ypos, m_yscale.width, m_yscale.height, m_yscale.boxColor);
    DrawText(m_yscale.name, m_yscale.xpos, m_yscale.ypos - 15, 10, BLACK );
    DrawText(m_yscale.contents.c_str(), m_yscale.xpos + 5, m_yscale.ypos + 8, 10, BLACK );

    DrawRectangleLines( m_zscale.xpos, m_zscale.ypos, m_zscale.width, m_zscale.height, m_zscale.boxColor);
    DrawText(m_zscale.name, m_zscale.xpos, m_zscale.ypos - 15, 10, BLACK );
    DrawText(m_zscale.contents.c_str(), m_zscale.xpos + 5, m_zscale.ypos + 8, 10, BLACK );

    DrawRectangle(m_update.xpos, m_update.ypos, m_update.width, m_update.height, GRAY);
    DrawText(m_update.name, (m_update.xpos + (m_update.width/2) - 20), m_update.ypos + 8, 10, WHITE);

    DrawRectangle(m_close.xpos, m_close.ypos, m_close.width, m_close.height, GRAY);
    DrawText(m_close.name, (m_close.xpos + (m_close.width/2) - 20), m_close.ypos + 8, 10, WHITE);


}
