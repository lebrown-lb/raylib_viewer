#ifndef SAVEMENU_H
#define SAVEMENU_H
#include "common.h"

class SaveMenu
{
    public:
        SaveMenu();
        SaveMenu(int width, int height);
        virtual ~SaveMenu();
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
        TEXTBOX m_file;
        PUSHBUTTON m_save;
        PUSHBUTTON m_close;

    protected:

    private:
        std::string vector3_to_string(Vector3 v);
        void saveSceneFile(std::string fpath, std::vector<MODELX> & models);
        std::chrono::steady_clock::time_point m_timestamp;
        bool m_visible = false;
        int m_screenWidth;
        int m_screenHeight;
};

#endif // SAVEMENU_H
