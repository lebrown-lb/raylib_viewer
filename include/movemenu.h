#ifndef MOVEMENU_H
#define MOVEMENU_H
#include "common.h"

class MoveMenu
{
    public:
        MoveMenu();
        MoveMenu(int width, int height);
        virtual ~MoveMenu();
        void handleInput(STATE *currentState, RayCollision collision,COLLISION & mesh,std::vector<MODELX> & models);
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
        TEXTBOX m_xpos;
        TEXTBOX m_ypos;
        TEXTBOX m_zpos;
        RADIOBUTTON m_relative;
        PUSHBUTTON m_update;
        PUSHBUTTON m_close;

    protected:

    private:
        bool m_visible = false;
        int m_screenWidth;
        int m_screenHeight;
};

#endif // MOVEMENU_H
