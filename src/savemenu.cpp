#include "savemenu.h"
#include <fstream>

SaveMenu::SaveMenu()
{
    //ctor
}

SaveMenu::SaveMenu(int width, int height)
    : m_screenWidth(width), m_screenHeight(height)
{
    int space = m_screenWidth / 4;
    int w = space - 20;
    m_file = {"File Location", 10, m_screenHeight-35, w * 3 + 20, 25, BLACK, "", false};
    m_save = {"SAVE",(space * 3) + 10, m_screenHeight-35, w, 25 };
    m_close = {"CLOSE", 10, m_screenHeight - 80, m_screenWidth - 20,25};

    m_timestamp = std::chrono::steady_clock::now();
}

SaveMenu::~SaveMenu()
{
    //dtor
}


void SaveMenu::handleInput(STATE * currentState, std::vector<MODELX> & models)
{
   if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Vector2 mpos = GetMousePosition();


        if(CheckCollisionPointRec(mpos, buttonRectangle(m_close)))
        {
            *currentState = MAIN_STATE;
            setVisible(false);
        }

        if(CheckCollisionPointRec(mpos, buttonRectangle(m_save)))
        {
            saveSceneFile(m_file.contents,models);
        }

        if(CheckCollisionPointRec(mpos, textBoxRectangle(m_file)))
        {
            setTextBoxSelection(m_file, true);
        }
        else
        {
            setTextBoxSelection(m_file, false);
        }

    }

    int key = GetCharPressed();
    if ((key >= 32) && (key <= 125))
    {
        if(m_file.selected)
            m_file.contents += (char)key;
    }

    if(IsKeyPressed(KEY_BACKSPACE))
    {
        if(m_file.selected && (m_file.contents.size() > 0))
            m_file.contents.pop_back();

    }

    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_V))
    {
        std::chrono::steady_clock::time_point ts = std::chrono::steady_clock::now();
        long time = std::chrono::duration_cast<std::chrono::microseconds>(ts - m_timestamp).count();

        if(m_file.selected && (time > 1000000))
            m_file.contents += getClipboardText();

        m_timestamp = ts;

    }
}

void SaveMenu::drawMenu(void)
{
    DrawRectangleLines( m_file.xpos, m_file.ypos, m_file.width, m_file.height, m_file.boxColor);
    DrawText(m_file.name, m_file.xpos, m_file.ypos - 15, 10, BLACK );
    DrawText(m_file.contents.c_str(), m_file.xpos + 5, m_file.ypos + 8, 10, BLACK );

    DrawRectangle(m_save.xpos, m_save.ypos, m_save.width, m_save.height, GRAY);
    DrawText(m_save.name, (m_save.xpos + (m_save.width/2) - 20), m_save.ypos + 8, 10, WHITE);

    DrawRectangle(m_close.xpos, m_close.ypos, m_close.width, m_close.height, GRAY);
    DrawText(m_close.name, (m_close.xpos + (m_close.width/2) - 20), m_close.ypos + 8, 10, WHITE);
}


std::string SaveMenu::vector3_to_string(Vector3 v)
{
    std::string tmp;

    tmp += "{ ";
    tmp += std::to_string(v.x);
    tmp += ", ";
    tmp += std::to_string(v.y);
    tmp += ", ";
    tmp += std::to_string(v.z);
    tmp += "}";

    return tmp;
}

void SaveMenu::saveSceneFile(std::string fpath, std::vector<MODELX> & models)
{
    if(fpath == "")
        return;

    std::ofstream outfile(fpath);

    for(size_t i = 0; i < models.size();i++)
    {
        outfile << "{" << models[i].name << "," << std::endl;
        outfile << models[i].mFile << "," << std::endl;
        outfile << models[i].tFile << "," << std::endl;
        outfile << vector3_to_string(models[i].location) << "," << std::endl;
        outfile << vector3_to_string(models[i].rotation) << "," << std::endl;
        outfile << vector3_to_string(models[i].scale) << std::endl;
        if(i == (models.size()-1))
            outfile << "}";
        else
            outfile << "}," << std::endl;
    }

    outfile.close();

}
