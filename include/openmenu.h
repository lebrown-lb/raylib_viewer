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
        void handleInput(STATE *currentState,std::vector<MODELX> & models, std::vector<BoundingBox> & bboxs);
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
        TEXTBOX m_name;
        TEXTBOX m_model;
        TEXTBOX m_texture;
        PUSHBUTTON m_load;
        PUSHBUTTON m_close;
        PUSHBUTTON m_clear;

    protected:

    private:
        std::chrono::steady_clock::time_point m_timestamp;
        OPENFIELDS m_current;
        std::vector<OPENFIELDS> m_history;
        size_t m_history_idx = 0;
        bool m_visible = false;
        int m_screenWidth;
        int m_screenHeight;
};

#endif // OPENMENU_H
