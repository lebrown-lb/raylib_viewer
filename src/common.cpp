#include "common.h"
#include <iostream>



BoundingBox calculateBoundingBox(Model & x)
{
    BoundingBox result;
    BoundingBox tmp;

    for(int m = 0; m < x.meshCount;m++)
    {
        tmp = GetMeshBoundingBox(x.meshes[m]);

        if(m == 0)
            result = tmp;
        else
        {
            if(tmp.max.x > result.max.x)
                result.max.x = tmp.max.x;
            if(tmp.max.y > result.max.y)
                result.max.y = tmp.max.y;
            if(tmp.max.z > result.max.z)
                result.max.z = tmp.max.z;

            if(tmp.min.x < result.min.x)
                result.min.x = tmp.min.x;
            if(tmp.min.y < result.min.y)
                result.min.y = tmp.min.y;
            if(tmp.min.z < result.min.z)
                result.min.z = tmp.min.z;
        }

    }

    return result;
}

BoundingBox calculateAndTransformBoundingBox(MODELX & x)
{
    BoundingBox result;
    BoundingBox tmp;

    for(int m = 0; m < x.model.meshCount;m++)
    {
        tmp = GetMeshBoundingBox(x.model.meshes[m]);

        if(m == 0)
            result = tmp;
        else
        {
            if(tmp.max.x > result.max.x)
                result.max.x = tmp.max.x;
            if(tmp.max.y > result.max.y)
                result.max.y = tmp.max.y;
            if(tmp.max.z > result.max.z)
                result.max.z = tmp.max.z;

            if(tmp.min.x < result.min.x)
                result.min.x = tmp.min.x;
            if(tmp.min.y < result.min.y)
                result.min.y = tmp.min.y;
            if(tmp.min.z < result.min.z)
                result.min.z = tmp.min.z;
        }

    }

    Vector3 corners[8];
    corners[0] = (Vector3){result.min.x, result.min.y, result.min.z};
    corners[1] = (Vector3){result.max.x, result.min.y, result.min.z};
    corners[2] = (Vector3){result.min.x, result.max.y, result.min.z};
    corners[3] = (Vector3){result.max.x, result.max.y, result.min.z};
    corners[4] = (Vector3){result.min.x, result.min.y, result.max.z};
    corners[5] = (Vector3){result.max.x, result.min.y, result.max.z};
    corners[6] = (Vector3){result.min.x, result.max.y, result.max.z};
    corners[7] = (Vector3){result.max.x, result.max.y, result.max.z};


    Matrix t = MatrixIdentity();
    t = MatrixMultiply(t,MatrixTranslate(-x.center.x,-x.center.y,-x.center.z));
    t = MatrixMultiply(t, MatrixScale(x.scale.x, x.scale.y, x.scale.z));
    t = MatrixMultiply(t, MatrixRotateXYZ(x.rotation)); // Or MatrixRotate if single axis
    t = MatrixMultiply(t, MatrixTranslate(x.location.x, x.location.y, x.location.z));
    t = MatrixMultiply(t,MatrixTranslate(x.center.x,x.center.y,x.center.z));

    for (int i = 0; i < 8; i++)
    {
        corners[i] = Vector3Transform(corners[i], t);
    }

    Vector3 newMin = corners[0];
    Vector3 newMax = corners[0];

    for (int i = 1; i < 8; i++)
    {
        newMin.x = fminf(newMin.x, corners[i].x);
        newMin.y = fminf(newMin.y, corners[i].y);
        newMin.z = fminf(newMin.z, corners[i].z);

        newMax.x = fmaxf(newMax.x, corners[i].x);
        newMax.y = fmaxf(newMax.y, corners[i].y);
        newMax.z = fmaxf(newMax.z, corners[i].z);
    }

    BoundingBox bbox = (BoundingBox){newMin, newMax};



    return bbox;

}

Vector3 calculateModelCenter(Model & x)
{
    Vector3 result;
    BoundingBox tmp = calculateBoundingBox(x);

    result.x = (tmp.min.x + tmp.max.x)/2;
    result.y = (tmp.min.y + tmp.max.y)/2;
    result.z = (tmp.min.z + tmp.max.z)/2;

    return result;

}

void loadModelx(std::string name, std::string model, std::string texture, std::vector<MODELX> & m)
{
    m.push_back({name,
                model,
                texture,
                LoadModel(model.c_str()),
                LoadTexture(texture.c_str()),
                (Vector3){0.0f, 0.0f, 0.0f},
                (Vector3){0.0f, 0.0f, 0.0f},
                (Vector3){0.0f, 0.0f, 0.0f},
                (Vector3){1.0f, 1.0f, 1.0f},
                false,
                WHITE,
                {(Vector3){0.0f, 0.0f, 0.0f},(Vector3){0.0f, 0.0f, 0.0f}}});
    size_t idx = m.size() - 1;
    if(texture != "")
        m[idx].model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = m[idx].texture;            // Set model diffuse texture
    m[idx].center = calculateModelCenter(m[idx].model);
    m[idx].bbox = calculateAndTransformBoundingBox(m[idx]); // Get mesh bounding box
    m[idx].model.transform = MatrixTranslate(m[idx].location.x, m[idx].location.y, m[idx].location.z);
}

void changeSelectedModelLocation(std::string xstr, std::string ystr, std::string zstr, std::vector<MODELX> & m)
{
    float x = std::stof(xstr);
    float y = std::stof(ystr);
    float z = std::stof(zstr);
    for(size_t i = 0; i < m.size();i++)
    {
        if(m[i].selected)
        {
            Matrix t;
            m[i].location.x = x;
            m[i].location.y = y;
            m[i].location.z = z;

            t = MatrixTranslate(-m[i].center.x, -m[i].center.y, -m[i].center.z);
            t = MatrixMultiply(t,MatrixScale(m[i].scale.x,m[i].scale.y,m[i].scale.z));
            t = MatrixMultiply(t,MatrixRotateXYZ(m[i].rotation));
            t = MatrixMultiply(t,MatrixTranslate(m[i].location.x, m[i].location.y, m[i].location.z));
            t = MatrixMultiply(t,MatrixTranslate(m[i].center.x,m[i].center.y,m[i].center.z));
            m[i].model.transform = t;
            m[i].bbox = calculateAndTransformBoundingBox(m[i]);


        }

    }
}

void changeSelectedModelRotation(std::string xstr, std::string ystr, std::string zstr, std::vector<MODELX> & m)
{
    float x = std::stof(xstr) * DEG2RAD;
    float y = std::stof(ystr) * DEG2RAD;
    float z = std::stof(zstr) * DEG2RAD;
    for(size_t i = 0; i < m.size();i++)
    {
        if(m[i].selected)
        {
            Matrix t;
            m[i].rotation.x = x;
            m[i].rotation.y = y;
            m[i].rotation.z = z;

            t = MatrixTranslate(-m[i].center.x, -m[i].center.y, -m[i].center.z);
            t = MatrixMultiply(t,MatrixScale(m[i].scale.x,m[i].scale.y,m[i].scale.z));
            t = MatrixMultiply(t,MatrixRotateXYZ(m[i].rotation));
            t = MatrixMultiply(t,MatrixTranslate(m[i].location.x, m[i].location.y, m[i].location.z));
            t = MatrixMultiply(t,MatrixTranslate(m[i].center.x,m[i].center.y,m[i].center.z));
            m[i].model.transform = t;
            m[i].bbox = calculateAndTransformBoundingBox(m[i]);


        }

    }

}

void changeSelectedModelScale(std::string xstr, std::string ystr, std::string zstr, std::vector<MODELX> & m)
{
    float x = std::stof(xstr);
    float y = std::stof(ystr);
    float z = std::stof(zstr);
    for(size_t i = 0; i < m.size();i++)
    {
        if(m[i].selected)
        {
            Matrix t;
            m[i].scale.x = x;
            m[i].scale.y = y;
            m[i].scale.z = z;

            t = MatrixTranslate(-m[i].center.x, -m[i].center.y, -m[i].center.z);
            t = MatrixMultiply(t,MatrixScale(m[i].scale.x,m[i].scale.y,m[i].scale.z));
            t = MatrixMultiply(t,MatrixRotateXYZ(m[i].rotation));
            t = MatrixMultiply(t,MatrixTranslate(m[i].location.x, m[i].location.y, m[i].location.z));
            t = MatrixMultiply(t,MatrixTranslate(m[i].center.x,m[i].center.y,m[i].center.z));
            m[i].model.transform = t;
            m[i].bbox = calculateAndTransformBoundingBox(m[i]);


        }

    }

}


Rectangle textBoxRectangle(TEXTBOX &tb)
{
    Rectangle tmp;

    tmp.x = tb.xpos;
    tmp.y = tb.ypos;
    tmp.width = tb.width;
    tmp.height = tb.height;

    return tmp;
}

Rectangle buttonRectangle(PUSHBUTTON &pb)
{
    Rectangle tmp;

    tmp.x = pb.xpos;
    tmp.y = pb.ypos;
    tmp.width = pb.width;
    tmp.height = pb.height;

    return tmp;
}

void setTextBoxSelection(TEXTBOX &tb, bool x)
{
    if(x)
    {
        tb.selected = true;
        tb.boxColor = RED;
    }
    else
    {
        tb.selected = false;
        tb.boxColor = BLACK;
    }

}

std::string getClipboardText()
{
    FILE *pipe = popen("xclip -o -selection clipboard", "r"); // or "xsel -b"
    if (!pipe) {
        return "Error opening pipe";
    }

    char buffer[128];
    std::string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != nullptr) {
            result += buffer;
        }
    }
    pclose(pipe);
    return result;
}

