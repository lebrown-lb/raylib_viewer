#include "openmenu.h"


OpenMenu::OpenMenu()
{
    //ctor
}

OpenMenu::OpenMenu(int width, int height)
    : m_screenWidth(width), m_screenHeight(height)
{
    //ctor
    m_name = {"Name:", 10, m_screenHeight-125, m_screenWidth-20, 25, BLACK, "", false};
    m_model = {"Model Location:", 10, m_screenHeight-80, m_screenWidth-20, 25, BLACK, "", false};
    m_texture = {"Texture Location:", 10, m_screenHeight-35, m_screenWidth-20, 25, BLACK, "", false};
    int space = m_screenWidth / 3;
    int x = space * 2 + 10;
    int w = space - 20;

    m_load = {"LOAD", x, m_screenHeight - 170, w, 25};
    m_close = {"CLOSE", space + 10, m_screenHeight - 170, w, 25};
    m_clear = {"CLEAR", 10, m_screenHeight - 170, w, 25};

    m_timestamp = std::chrono::steady_clock::now();
}

OpenMenu::~OpenMenu()
{
    //dtor
}


void OpenMenu::handleInput(STATE * currentState, std::vector<MODELX> & models, std::vector<BoundingBox> & bboxs)
{
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Vector2 mpos = GetMousePosition();

        if(CheckCollisionPointRec(mpos, buttonRectangle(m_close)))
        {
            *currentState = MAIN_STATE;
            setVisible(false);
        }
        else if(CheckCollisionPointRec(mpos, buttonRectangle(m_load)))
        {
            loadModelx(m_name.contents, m_model.contents,m_texture.contents,models,bboxs);
            m_history.push_back({m_name.contents,m_model.contents,m_texture.contents});
            m_history_idx = m_history.size();
        }
        else if(CheckCollisionPointRec(mpos, buttonRectangle(m_clear)))
        {
            if(m_name.selected)
                m_name.contents = "";
            else if(m_model.selected)
                m_model.contents = "";
            else if(m_texture.selected)
                m_texture.contents = "";
        }

        if(CheckCollisionPointRec(mpos, textBoxRectangle(m_name)))
        {
            setTextBoxSelection(m_name, true);
            setTextBoxSelection(m_model, false);
            setTextBoxSelection(m_texture, false);

        }
        else if(CheckCollisionPointRec(mpos, textBoxRectangle(m_model)))
        {
            setTextBoxSelection(m_name, false);
            setTextBoxSelection(m_model, true);
            setTextBoxSelection(m_texture, false);
        }
        else if(CheckCollisionPointRec(mpos, textBoxRectangle(m_texture)))
        {
            setTextBoxSelection(m_name, false);
            setTextBoxSelection(m_model, false);
            setTextBoxSelection(m_texture, true);
        }
        else
        {
            setTextBoxSelection(m_name, false);
            setTextBoxSelection(m_model, false);
            setTextBoxSelection(m_texture, false);
        }
    }

    int key = GetCharPressed();
    if ((key >= 32) && (key <= 125))
    {
        if(m_name.selected)
            m_name.contents += (char)key;
        else if(m_model.selected)
            m_model.contents += (char)key;
        else if(m_texture.selected)
            m_texture.contents += (char)key;
    }

    if(IsKeyPressed(KEY_BACKSPACE))
    {
        if(m_name.selected && (m_name.contents.size() > 0))
            m_name.contents.pop_back();
        else if(m_model.selected && (m_model.contents.size() > 0))
            m_model.contents.pop_back();
        else if(m_texture.selected && (m_texture.contents.size() > 0))
            m_texture.contents.pop_back();

    }

    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_V))
    {
        std::chrono::steady_clock::time_point ts = std::chrono::steady_clock::now();
        long time = std::chrono::duration_cast<std::chrono::microseconds>(ts - m_timestamp).count();

        if(m_name.selected && (time > 1000000))
            m_name.contents += getClipboardText();
        else if(m_model.selected && (time > 1000000))
            m_model.contents += getClipboardText();
        else if(m_texture.selected && (time > 1000000))
            m_texture.contents += getClipboardText();

        m_timestamp = ts;

    }

    if(IsKeyPressed(KEY_UP) && m_history.size())
    {
        if(m_history_idx == m_history.size())
            m_current = {m_name.contents,m_model.contents,m_texture.contents};

        if(m_history_idx != 0)
            m_history_idx--;

        m_name.contents = m_history[m_history_idx].name;
        m_model.contents = m_history[m_history_idx].model;
        m_texture.contents = m_history[m_history_idx].texture;

    }

    if(IsKeyPressed(KEY_DOWN) && (m_history_idx < m_history.size()))
    {
        m_history_idx++;

        if(m_history_idx == m_history.size())
        {
            m_name.contents = m_current.name;
            m_model.contents = m_current.model;
            m_texture.contents = m_current.texture;
        }
        else
        {
            m_name.contents = m_history[m_history_idx].name;
            m_model.contents = m_history[m_history_idx].model;
            m_texture.contents = m_history[m_history_idx].texture;
        }
    }

}

void OpenMenu::drawMenu(void)
{
    DrawRectangleLines( m_texture.xpos, m_texture.ypos, m_texture.width, m_texture.height, m_texture.boxColor);
    DrawText(m_texture.name, m_texture.xpos, m_texture.ypos - 15, 10, BLACK );
    DrawText(m_texture.contents.c_str(), m_texture.xpos + 5, m_texture.ypos + 8, 10, BLACK );

    DrawRectangleLines( m_model.xpos, m_model.ypos, m_model.width, m_model.height, m_model.boxColor);
    DrawText(m_model.name, m_model.xpos, m_model.ypos - 15, 10, BLACK );
    DrawText(m_model.contents.c_str(), m_model.xpos + 5, m_model.ypos + 8, 10, BLACK );

    DrawRectangleLines( m_name.xpos, m_name.ypos, m_name.width, m_name.height, m_name.boxColor);
    DrawText(m_name.name, m_name.xpos, m_name.ypos - 15, 10, BLACK );
    DrawText(m_name.contents.c_str(), m_name.xpos + 5, m_name.ypos + 8, 10, BLACK );

    DrawRectangle(m_load.xpos, m_load.ypos, m_load.width, m_load.height, GRAY);
    DrawText(m_load.name, (m_load.xpos + (m_load.width/2) - 20), m_load.ypos + 8, 10, WHITE);

    DrawRectangle(m_close.xpos, m_close.ypos, m_close.width, m_close.height, GRAY);
    DrawText(m_close.name, (m_close.xpos + (m_close.width/2) - 20), m_close.ypos + 8, 10, WHITE);

    DrawRectangle(m_clear.xpos, m_clear.ypos, m_clear.width, m_clear.height, GRAY);
    DrawText(m_clear.name, (m_clear.xpos + (m_clear.width/2) - 20), m_clear.ypos + 8, 10, WHITE);

}
