
#ifndef COMMON_H
#define COMMON_H
#include <string>
#include <vector>
#include <cstdio>
#include <mutex>
#include <raylib.h>
#include <raymath.h>


struct MODELX{
    std::string name;
    std::string mFile;
    std::string tFile;
    Model model;
    Texture2D texture;
    Vector3 center = { 0.0f, 0.0f, 0.0f};
    Vector3 location = { 0.0f, 0.0f, 0.0f};
    Vector3 rotation = { 0.0f, 0.0f, 0.0f};
    Vector3 scale = { 1.0f, 1.0f, 1.0f};
    bool selected;
    Color color;
    BoundingBox bbox;
};

struct TEXTBOX{
    const char * name = nullptr;
    int xpos;
    int ypos;
    int width;
    int height;
    Color boxColor;
    std::string contents;
    bool selected;
};

struct PUSHBUTTON{
    const char * name = nullptr;
    int xpos;
    int ypos;
    int width;
    int height;
};

enum STATE{
    MAIN_STATE,
    OPEN_MENU_STATE,
    MOVE_MENU_STATE,
    ROTATE_MENU_STATE,
    SCALE_MENU_STATE,
    SAVE_MENU_STATE

};



BoundingBox calculateBoundingBox(Model & x);

BoundingBox calculateAndTransformBoundingBox(MODELX & x);

Vector3 calculateModelCenter(Model & x);

Vector3 stringsToVector3(std::string xstr, std::string ystr, std::string zstr);

void loadModelx(std::string name, std::string model, std::string texture, Vector3 start, std::vector<MODELX> & m);

void changeSelectedModelLocation(Vector3 location, std::vector<MODELX> & m);

void changeSelectedModelRotation(Vector3 rotation, std::vector<MODELX> & m);

void changeSelectedModelScale(Vector3 scale, std::vector<MODELX> & m);

Rectangle textBoxRectangle(TEXTBOX &tb);

Rectangle buttonRectangle(PUSHBUTTON &pb);

void setTextBoxSelection(TEXTBOX &tb, bool x);

std::string getClipboardText();



#endif // COMMON_H
