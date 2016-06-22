#ifndef CAMERA_H_
#define CAMERA_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

using namespace glm;
class Camera {
 public:
    Camera( vec3 position, vec3 front, vec3 up, GLfloat fov, GLfloat near, GLfloat far )
    {
	_position = position;
	_front = front;
	_up = up;
	_fov = fov;
	_near = near;
	_far = far;
    }
    GLfloat getFov()
    {
	return _fov;
    }
    void setFov(GLfloat fov)
    {
	_fov = fov;
    }

    GLfloat getNear()
    {
	return _near;
    }

    GLfloat getFar()
    {
	return _far;
    }

    vec3 getPos()
    {
	return _position;
    }
    void setPos(vec3 position)
    {
	_position = position;
    }

    vec3 getFront()
    {
	return _front;
    }
    void setFront(vec3 front)
    {
	_front = front;
    }

    vec3 getUp()
    {
	return _up;
    }
    void setUp(vec3 up)
    {
	_up = up;
    }

 private:
    vec3 _position;
    vec3 _front;
    vec3 _up;
    GLfloat _fov;
    GLfloat _far;
    GLfloat _near;
};
#endif
