#ifndef OPENMENU_H
#define OPENMENU_H

#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <string>
#include <chrono>
#include "common.h"

struct OPENFIELDS{
    std::string name;
    std::string model;
    std::string texture;
};


class OpenMenu
{
    public:
        OpenMenu();
        OpenMenu(int width, int height);
        virtual ~OpenMenu();
        void handleInput(STATE *currentState,RayCollision collision, COLLISION & mesh,std::vector<MODELX> & models);
        void drawMenu(void);
        bool isVisible(void)
        {
            return m_visible;
        }
        void setVisible(bool x)
        {
            m_visible = x;
        }
        void setScreenSize(int width, int height)
        {
            m_screenWidth = width;
            m_screenHeight = height;
        }
        TEXTBOX m_scene;
        TEXTBOX m_name;
        TEXTBOX m_model;
        TEXTBOX m_texture;
        TEXTBOX m_xstart;
        TEXTBOX m_ystart;
        TEXTBOX m_zstart;
        PUSHBUTTON m_loadScene;
        PUSHBUTTON m_load;
        PUSHBUTTON m_close;
        PUSHBUTTON m_clear;

    protected:

    private:
        std::vector<std::string> parse_string_by_curly_braces(std::string str);
        std::vector<std::string> parse_variable_string(std::string str);
        Vector3 parse_vector3_string(std::string str);
        std::string readFileToString(const std::string& filePath);
        void loadSceneFile(std::string fpath, std::vector<MODELX> & m);
        std::chrono::steady_clock::time_point m_timestamp;
        OPENFIELDS m_current;
        std::vector<OPENFIELDS> m_history;
        size_t m_history_idx = 0;
        bool m_visible = false;
        int m_screenWidth;
        int m_screenHeight;
};

#endif // OPENMENU_H
