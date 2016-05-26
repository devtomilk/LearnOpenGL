#ifndef CAMERA_H_
#define CAMERA_H_

#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Camera
{
 public:
    Camera( glm::vec3 position, glm::vec3 up, glm::vec3 front, GLfloat fov, GLfloat ratio, GLfloat near, GLfloat far)
    {
	_position = position;
	_up = up;
	_front = front;
	_ratio = ratio;
	_fov = fov;
	_near = near;
	_far = far;
    }

    glm::vec3 getPos()
    {
	return _position;
    }

    glm::vec3 getUp()
    {
	return _up;
    }

    
    float getRatio()
    {
	return _ratio;
    }

    
    float getNear()
    {
	return _near;
    }

    
    float getFar()
    {
	return _far;
    }

    float getFOV()
    {
	return _fov;
    }

    void setPos( glm::vec3 position )
    {
	_position = position;
    }

    void setFront( glm::vec3 front )
    {
	_front = front;
    }

    glm::vec3 getFront()
    {
	return _front;
    }

 private:
    glm::vec3 _position;
    glm::vec3 _up;
    glm::vec3 _front;
    GLfloat _fov;
    GLfloat _far;
    GLfloat _near;
    GLfloat _ratio;
};

#endif
