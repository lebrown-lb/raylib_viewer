#ifndef ROTATEMENU_H
#define ROTATEMENU_H
#include "common.h"

class RotateMenu
{
    public:
        RotateMenu();
        RotateMenu(int width, int height);
        virtual ~RotateMenu();
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
        TEXTBOX m_xrot;
        TEXTBOX m_yrot;
        TEXTBOX m_zrot;
        PUSHBUTTON m_update;
        PUSHBUTTON m_close;

    protected:

    private:
        bool m_visible = false;
        int m_screenWidth;
        int m_screenHeight;
};

#endif // ROTATEMENU_H
