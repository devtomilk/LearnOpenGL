#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <iostream>
#include <glm/glm.hpp>


using namespace glm;
class Material {
 public:
    // Constructor
    Material( vec3 ambient, vec3 diffuse, vec3 specular, float shininess )
    {
	_ambient = ambient;
	_diffuse = diffuse;
	_specular = specular;
	_shininess = shininess;

    }

    glm::vec3 getAmbient()
    {
	return _ambient;
    }

    glm::vec3 getDiffuse()
    {
	return _diffuse;
    }

    glm::vec3 getSpecular()
    {
	return _specular;
    }

    float getShininess()
    {
	return _shininess;
    }

    
 private:
    glm::vec3 _ambient;
    glm::vec3 _diffuse;
    glm::vec3 _specular;
    float _shininess;
};


#endif
