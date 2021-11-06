#pragma once
#include "Light.h"

class DirectionalLight :
    public Light
{
public:
    DirectionalLight();
    DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, 
                     GLfloat aIntensity, GLfloat dIntensity, //aIntensity for ambient light intensity, dIntensity for Diffuse light intensity
                    GLfloat xDir, GLfloat yDir, GLfloat zDir);          

    void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
        GLfloat diffuseIntensityLocation, GLfloat directionLocation);

    ~DirectionalLight();

private:
    glm::vec3 direction;

};

