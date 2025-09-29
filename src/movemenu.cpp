#include "movemenu.h"

MoveMenu::MoveMenu()
{
    //ctor
}

MoveMenu::MoveMenu(int width, int height)
    : m_screenWidth(width), m_screenHeight(height)
{
    int space = m_screenWidth / 4;
    int w = space - 20;
    m_xpos = {"X POSITION", 10, m_screenHeight-35, w, 25, BLACK, "", false};
    m_ypos = {"Y POSITION", space + 10, m_screenHeight-35, w, 25, BLACK, "", false};
    m_zpos = {"Z POSITION", (space * 2) + 10, m_screenHeight-35, w, 25, BLACK, "", false};
    m_relative = {"Relative Move",m_screenWidth - 20, m_screenHeight - 95, 8.0,false};
    m_update = {"UPDATE",(space * 3) + 10, m_screenHeight-35, w, 25 };
    m_close = {"CLOSE", 10, m_screenHeight - 80, m_screenWidth - 20,25};

}

MoveMenu::~MoveMenu()
{
    //dtor
}


void MoveMenu::handleInput(STATE * currentState,RayCollision collision,COLLISION & mesh, std::vector<MODELX> & models)
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
            changeSelectedModelLocation(stringsToVector3(m_xpos.contents, m_ypos.contents, m_zpos.contents), models, m_relative.value);
            mesh.hitInfo.hit = false;
        }
        if(CheckCollisionPointCircle(mpos,{m_relative.xpos,m_relative.ypos}, m_relative.radius))
        {
            if(m_relative.value)
                m_relative.value = false;
            else
                m_relative.value = true;

        }

        if(CheckCollisionPointRec(mpos, textBoxRectangle(m_xpos)))
        {
            setTextBoxSelection(m_xpos, true);
            setTextBoxSelection(m_ypos, false);
            setTextBoxSelection(m_zpos, false);
            mesh.hitInfo.hit = false;

        }
        else if(CheckCollisionPointRec(mpos, textBoxRectangle(m_ypos)))
        {
            setTextBoxSelection(m_xpos, false);
            setTextBoxSelection(m_ypos, true);
            setTextBoxSelection(m_zpos, false);
            mesh.hitInfo.hit = false;
        }
        else if(CheckCollisionPointRec(mpos, textBoxRectangle(m_zpos)))
        {
            setTextBoxSelection(m_xpos, false);
            setTextBoxSelection(m_ypos, false);
            setTextBoxSelection(m_zpos, true);
            mesh.hitInfo.hit = false;
        }
        else
        {
            setTextBoxSelection(m_xpos, false);
            setTextBoxSelection(m_ypos, false);
            setTextBoxSelection(m_zpos, false);
        }

    }

    int key = GetCharPressed();
    if ((key >= 32) && (key <= 125))
    {
        if(m_xpos.selected)
            m_xpos.contents += (char)key;
        else if(m_ypos.selected)
            m_ypos.contents += (char)key;
        else if(m_zpos.selected)
            m_zpos.contents += (char)key;
    }

    if(IsKeyPressed(KEY_BACKSPACE))
    {
        if(m_xpos.selected && (m_xpos.contents.size() > 0))
            m_xpos.contents.pop_back();
        else if(m_ypos.selected && (m_ypos.contents.size() > 0))
            m_ypos.contents.pop_back();
        else if(m_zpos.selected && (m_zpos.contents.size() > 0))
            m_zpos.contents.pop_back();

    }

    if(IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE) && collision.hit)
    {
        m_xpos.contents = std::to_string(collision.point.x);
        m_ypos.contents = std::to_string(collision.point.y);
        m_zpos.contents = std::to_string(collision.point.z);
    }
}

void MoveMenu::drawMenu(void)
{
    DrawRectangleLines( m_xpos.xpos, m_xpos.ypos, m_xpos.width, m_xpos.height, m_xpos.boxColor);
    DrawText(m_xpos.name, m_xpos.xpos, m_xpos.ypos - 15, 10, BLACK );
    DrawText(m_xpos.contents.c_str(), m_xpos.xpos + 5, m_xpos.ypos + 8, 10, BLACK );

    DrawRectangleLines( m_ypos.xpos, m_ypos.ypos, m_ypos.width, m_ypos.height, m_ypos.boxColor);
    DrawText(m_ypos.name, m_ypos.xpos, m_ypos.ypos - 15, 10, BLACK );
    DrawText(m_ypos.contents.c_str(), m_ypos.xpos + 5, m_ypos.ypos + 8, 10, BLACK );

    DrawRectangleLines( m_zpos.xpos, m_zpos.ypos, m_zpos.width, m_zpos.height, m_zpos.boxColor);
    DrawText(m_zpos.name, m_zpos.xpos, m_zpos.ypos - 15, 10, BLACK );
    DrawText(m_zpos.contents.c_str(), m_zpos.xpos + 5, m_zpos.ypos + 8, 10, BLACK );

    DrawRectangle(m_update.xpos, m_update.ypos, m_update.width, m_update.height, GRAY);
    DrawText(m_update.name, (m_update.xpos + (m_update.width/2) - 20), m_update.ypos + 8, 10, WHITE);

    DrawRectangle(m_close.xpos, m_close.ypos, m_close.width, m_close.height, GRAY);
    DrawText(m_close.name, (m_close.xpos + (m_close.width/2) - 20), m_close.ypos + 8, 10, WHITE);

    if(m_relative.value)
        DrawCircle(m_relative.xpos, m_relative.ypos, m_relative.radius,BLACK);
    else
        DrawCircleLines(m_relative.xpos, m_relative.ypos, m_relative.radius,BLACK);
    DrawText(m_relative.name, m_relative.xpos - 85, m_relative.ypos - 4,10,BLACK);


}
