#ifndef SCALEMENU_H
#define SCALEMENU_H
#include "common.h"

class ScaleMenu
{
    public:
        ScaleMenu();
        ScaleMenu(int width, int height);
        virtual ~ScaleMenu();
        void handleInput(STATE *currentState,COLLISION & mesh,std::vector<MODELX> & models);
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
        TEXTBOX m_xscale;
        TEXTBOX m_yscale;
        TEXTBOX m_zscale;
        PUSHBUTTON m_update;
        PUSHBUTTON m_close;

    protected:

    private:
        bool m_visible = false;
        int m_screenWidth;
        int m_screenHeight;
};



#endif // SCALEMENU_H
