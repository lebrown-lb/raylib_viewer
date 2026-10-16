#include "openmenu.h"
#include <fstream>    // For std::ifstream
#include <sstream>    // For std::stringstream

OpenMenu::OpenMenu()
{
    //ctor
}

OpenMenu::OpenMenu(int width, int height)
    : m_screenWidth(width), m_screenHeight(height)
{
    //ctor
    m_name = {"Name:", 10, m_screenHeight-170, m_screenWidth-20, 25, BLACK, "", false};
    m_model = {"Model Location:", 10, m_screenHeight-125, m_screenWidth-20, 25, BLACK, "", false};
    m_texture = {"Texture Location:", 10, m_screenHeight-80, m_screenWidth-20, 25, BLACK, "", false};
    int space = m_screenWidth / 3;
    int x = space * 2 + 10;
    int w = space - 20;

    m_load = {"LOAD", x, m_screenHeight - 215, w, 25};
    m_close = {"CLOSE", space + 10, m_screenHeight - 215, w, 25};
    m_clear = {"CLEAR", 10, m_screenHeight - 215, w, 25};

    m_scene = {"Scene Location:", 10, m_screenHeight - 260, 2*w+20, 25, BLACK, "", false};
    m_loadScene = {"LOAD SCENE", x, m_screenHeight - 260, w, 25};

    m_xstart = {"X START POSITION:", 10, m_screenHeight - 35, w, 25, BLACK, "", false};
    m_ystart = {"Y START POSITION:", space + 10, m_screenHeight - 35, w, 25, BLACK, "", false};
    m_zstart = {"Z START POSITION:", x, m_screenHeight - 35, w, 25, BLACK, "", false};

    m_timestamp = std::chrono::steady_clock::now();
}

OpenMenu::~OpenMenu()
{
    //dtor
}


void OpenMenu::handleInput(STATE * currentState,RayCollision collision,COLLISION & mesh, std::vector<MODELX> & models)
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
        else if(CheckCollisionPointRec(mpos, buttonRectangle(m_load)))
        {
            Vector3 tmp = stringsToVector3(m_xstart.contents,m_ystart.contents,m_zstart.contents);
            loadModelx(m_name.contents, m_model.contents,m_texture.contents,tmp,models);
            m_history.push_back({m_name.contents,m_model.contents,m_texture.contents});
            m_history_idx = m_history.size();
            mesh.hitInfo.hit = false;
        }
        else if(CheckCollisionPointRec(mpos, buttonRectangle(m_clear)))
        {
            if(m_name.selected)
                m_name.contents = "";
            else if(m_model.selected)
                m_model.contents = "";
            else if(m_texture.selected)
                m_texture.contents = "";
            else if(m_scene.selected)
                m_scene.contents = "";
            else if(m_xstart.selected)
                m_xstart.contents = "";
            else if(m_ystart.selected)
                m_ystart.contents = "";
            else if(m_zstart.selected)
                m_zstart.contents = "";

            mesh.hitInfo.hit = false;
        }
        else if(CheckCollisionPointRec(mpos, buttonRectangle(m_loadScene)))
        {
            loadSceneFile(m_scene.contents, models);
            mesh.hitInfo.hit = false;
        }

        if(CheckCollisionPointRec(mpos, textBoxRectangle(m_name)))
        {
            setTextBoxSelection(m_name, true);
            setTextBoxSelection(m_model, false);
            setTextBoxSelection(m_texture, false);
            setTextBoxSelection(m_scene, false);
            setTextBoxSelection(m_xstart, false);
            setTextBoxSelection(m_ystart, false);
            setTextBoxSelection(m_zstart, false);

            mesh.hitInfo.hit = false;


        }
        else if(CheckCollisionPointRec(mpos, textBoxRectangle(m_model)))
        {
            setTextBoxSelection(m_name, false);
            setTextBoxSelection(m_model, true);
            setTextBoxSelection(m_texture, false);
            setTextBoxSelection(m_scene, false);
            setTextBoxSelection(m_xstart, false);
            setTextBoxSelection(m_ystart, false);
            setTextBoxSelection(m_zstart, false);

            mesh.hitInfo.hit = false;
        }
        else if(CheckCollisionPointRec(mpos, textBoxRectangle(m_texture)))
        {
            setTextBoxSelection(m_name, false);
            setTextBoxSelection(m_model, false);
            setTextBoxSelection(m_texture, true);
            setTextBoxSelection(m_scene, false);
            setTextBoxSelection(m_xstart, false);
            setTextBoxSelection(m_ystart, false);
            setTextBoxSelection(m_zstart, false);

            mesh.hitInfo.hit = false;
        }
        else if(CheckCollisionPointRec(mpos, textBoxRectangle(m_scene)))
        {
            setTextBoxSelection(m_name, false);
            setTextBoxSelection(m_model, false);
            setTextBoxSelection(m_texture, false);
            setTextBoxSelection(m_scene, true);
            setTextBoxSelection(m_xstart, false);
            setTextBoxSelection(m_ystart, false);
            setTextBoxSelection(m_zstart, false);

            mesh.hitInfo.hit = false;
        }
        else if(CheckCollisionPointRec(mpos, textBoxRectangle(m_xstart)))
        {
            setTextBoxSelection(m_name, false);
            setTextBoxSelection(m_model, false);
            setTextBoxSelection(m_texture, false);
            setTextBoxSelection(m_scene, false);
            setTextBoxSelection(m_xstart, true);
            setTextBoxSelection(m_ystart, false);
            setTextBoxSelection(m_zstart, false);

            mesh.hitInfo.hit = false;
        }
        else if(CheckCollisionPointRec(mpos, textBoxRectangle(m_ystart)))
        {
            setTextBoxSelection(m_name, false);
            setTextBoxSelection(m_model, false);
            setTextBoxSelection(m_texture, false);
            setTextBoxSelection(m_scene, false);
            setTextBoxSelection(m_xstart, false);
            setTextBoxSelection(m_ystart, true);
            setTextBoxSelection(m_zstart, false);

            mesh.hitInfo.hit = false;
        }
        else if(CheckCollisionPointRec(mpos, textBoxRectangle(m_zstart)))
        {
            setTextBoxSelection(m_name, false);
            setTextBoxSelection(m_model, false);
            setTextBoxSelection(m_texture, false);
            setTextBoxSelection(m_scene, false);
            setTextBoxSelection(m_xstart, false);
            setTextBoxSelection(m_ystart, false);
            setTextBoxSelection(m_zstart, true);

            mesh.hitInfo.hit = false;
        }
        else
        {
            setTextBoxSelection(m_name, false);
            setTextBoxSelection(m_model, false);
            setTextBoxSelection(m_texture, false);
            setTextBoxSelection(m_scene, false);
            setTextBoxSelection(m_xstart, false);
            setTextBoxSelection(m_ystart, false);
            setTextBoxSelection(m_zstart, false);
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
        else if(m_scene.selected)
            m_scene.contents += (char)key;
        else if(m_xstart.selected)
            m_xstart.contents += (char)key;
        else if(m_ystart.selected)
            m_ystart.contents += (char)key;
        else if(m_zstart.selected)
            m_zstart.contents += (char)key;
    }

    if(IsKeyPressed(KEY_BACKSPACE))
    {
        if(m_name.selected && (m_name.contents.size() > 0))
            m_name.contents.pop_back();
        else if(m_model.selected && (m_model.contents.size() > 0))
            m_model.contents.pop_back();
        else if(m_texture.selected && (m_texture.contents.size() > 0))
            m_texture.contents.pop_back();
        else if(m_scene.selected && (m_scene.contents.size() > 0))
            m_scene.contents.pop_back();
        else if(m_xstart.selected && (m_xstart.contents.size() > 0))
            m_xstart.contents.pop_back();
        else if(m_ystart.selected && (m_ystart.contents.size() > 0))
            m_ystart.contents.pop_back();
        else if(m_zstart.selected && (m_zstart.contents.size() > 0))
            m_zstart.contents.pop_back();

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
        else if(m_scene.selected && (time > 1000000))
            m_scene.contents += getClipboardText();

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

    if(IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE) && collision.hit)
    {
        m_xstart.contents = std::to_string(collision.point.x);
        m_ystart.contents = std::to_string(collision.point.y);
        m_zstart.contents = std::to_string(collision.point.z);
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

    DrawRectangleLines( m_scene.xpos, m_scene.ypos, m_scene.width, m_scene.height, m_scene.boxColor);
    DrawText(m_scene.name, m_scene.xpos, m_scene.ypos - 15, 10, BLACK );
    DrawText(m_scene.contents.c_str(), m_scene.xpos + 5, m_scene.ypos + 8, 10, BLACK );

    DrawRectangle(m_loadScene.xpos, m_loadScene.ypos, m_loadScene.width, m_loadScene.height, GRAY);
    DrawText(m_loadScene.name, (m_loadScene.xpos + (m_loadScene.width/2) - 40), m_loadScene.ypos + 8, 10, WHITE);

    DrawRectangleLines( m_xstart.xpos, m_xstart.ypos, m_xstart.width, m_xstart.height, m_xstart.boxColor);
    DrawText(m_xstart.name, m_xstart.xpos, m_xstart.ypos - 15, 10, BLACK );
    DrawText(m_xstart.contents.c_str(), m_xstart.xpos + 5, m_xstart.ypos + 8, 10, BLACK );

    DrawRectangleLines( m_ystart.xpos, m_ystart.ypos, m_ystart.width, m_ystart.height, m_ystart.boxColor);
    DrawText(m_ystart.name, m_ystart.xpos, m_ystart.ypos - 15, 10, BLACK );
    DrawText(m_ystart.contents.c_str(), m_ystart.xpos + 5, m_ystart.ypos + 8, 10, BLACK );

    DrawRectangleLines( m_zstart.xpos, m_zstart.ypos, m_zstart.width, m_zstart.height, m_zstart.boxColor);
    DrawText(m_zstart.name, m_zstart.xpos, m_zstart.ypos - 15, 10, BLACK );
    DrawText(m_zstart.contents.c_str(), m_zstart.xpos + 5, m_zstart.ypos + 8, 10, BLACK );
}


std::vector<std::string> OpenMenu::parse_string_by_curly_braces(std::string str)
{
    std::vector<std::string> sections;
    int brace_count = 0;
    std::string current_section;

    for (char c : str)
    {
        if (c == '{')
        {
            brace_count++;
            if (brace_count == 1) // Start of a new top-level section
            {
                current_section.clear();
            }
        }
        else if (c == '}')
        {
            brace_count--;
            if (brace_count == 0) // End of a top-level section
            {
                current_section += '}';
                sections.push_back(current_section);
                current_section.clear();
            }
        }
        if (brace_count > 0) // Inside a section
            if((c != '\n') && (c != '\r'))
                current_section += c;
    }

    return sections;

}


std::vector<std::string> OpenMenu::parse_variable_string(std::string str)
{
    std::vector<std::string> sections;
    int brace_count = 0;
    std::string current_section;
    size_t first = str.find('{');
    size_t last = str.rfind('}');

    str.erase(first,1);
    str.erase(last,1);

    size_t s = str.size() - 1;
    size_t cnt = 0;

    for (char c : str)
    {
        if (c == '{')
            brace_count++;
        else if (c == '}')
            brace_count--;

        if ((brace_count == 0) && (c == ','))
        {
            sections.push_back(current_section);
            current_section.clear();

        }
        else if(cnt == s)
            sections.push_back(current_section);
        else
        {
            if(c != ' ')
                current_section += c;
        }

        cnt++;
    }

    return sections;


}


Vector3 OpenMenu::parse_vector3_string(std::string str)
{
    Vector3 result = {0.0f, 0.0f, 0.0f};
    std::vector<std::string> vstrings = parse_variable_string(str);
    if(vstrings.size() != 3)
        return result;

    result.x = std::stof(vstrings[0]);
    result.y = std::stof(vstrings[1]);
    result.z = std::stof(vstrings[2]);

    return result;


}


std::string OpenMenu::readFileToString(const std::string& filePath)
{
    std::ifstream inputFile(filePath);
    std::stringstream buffer;
    std::string fileContent;

    if (!inputFile.is_open())
    {
        std::cout << "Error: Could not open file " << filePath << std::endl;
        return ""; // Return an empty string on error
    }

    // Read the entire file content into the stringstream buffer
    buffer << inputFile.rdbuf();

    // Get the string from the stringstream buffer
    fileContent = buffer.str();

    inputFile.close(); // Close the file stream

    return fileContent;
}


void OpenMenu::loadSceneFile(std::string fpath, std::vector<MODELX> & m)
{
    std::string contents = readFileToString(fpath);

    if(contents == "")
        return;

    std::vector<std::string> sections = parse_string_by_curly_braces(contents);

    size_t idx;
    for(std::string x : sections)
    {
        std::vector<std::string> variables = parse_variable_string(x);
        if(variables.size() != 6)
        {
            std::cout << "ERROR PARSING FILE" << std::endl;
            break;
        }

        std::string name = variables[0];
        std::string model = variables[1];
        std::string texture = variables[2];
        std::cout << "-----------------------------------------------------------------------------------" << std::endl;
        std::cout << "name:" << name << std::endl;
        std::cout << "model:" << model << std::endl;
        std::cout << "texture:" << texture << std::endl;

        Vector3 location = parse_vector3_string(variables[3]);
        Vector3 rotation = parse_vector3_string(variables[4]);
        Vector3 scale = parse_vector3_string(variables[5]);

        if((scale.x == 0.0f) || (scale.y == 0.0f) || (scale.z == 0.0f))
        {
            scale.x = 1.0f;
            scale.y = 1.0f;
            scale.z = 1.0f;
        }





        m.push_back({name,
                    model,
                    texture,
                    LoadModel(model.c_str()),
                    LoadTexture(texture.c_str()),
                    (Vector3){0.0f, 0.0f, 0.0f},
                    location,
                    rotation,
                    scale,
                    false,
                    WHITE,
                    {(Vector3){0.0f, 0.0f, 0.0f},(Vector3){0.0f, 0.0f, 0.0f}}});

        idx = m.size() - 1;

        if(texture != "")
            m[idx].model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = m[idx].texture;
        m[idx].center = calculateModelCenter(m[idx].model);
        m[idx].bbox = calculateAndTransformBoundingBox(m[idx]);
        Matrix t = MatrixTranslate(-m[idx].center.x, -m[idx].center.y, -m[idx].center.z);
        t = MatrixMultiply(t,MatrixScale(m[idx].scale.x,m[idx].scale.y,m[idx].scale.z));
        t = MatrixMultiply(t,MatrixRotateXYZ(m[idx].rotation));
        t = MatrixMultiply(t,MatrixTranslate(m[idx].location.x, m[idx].location.y, m[idx].location.z));
        t = MatrixMultiply(t,MatrixTranslate(m[idx].center.x,m[idx].center.y,m[idx].center.z));
        m[idx].model.transform = t;


        m_history.push_back({name,model,texture});


    }

    m_history_idx = m_history.size();

}
