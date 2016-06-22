// language
#include <iostream>

// glew
#define GLEW_STATIC
#include <GL/glew.h>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// glfw
#include <GLFW/glfw3.h>

// soil
#include <SOIL/SOIL.h>


// user defined
#include "../Shader.h"
//====================================================================================================
// Variables
//====================================================================================================
#define WIN_WIDTH 1024
#define WIN_HEIGHT 768
#define TITLE "02HelloTriangle"
GLFWwindow* window;
GLuint VAO;
GLuint VBO;
Shader* basicShader = NULL;

GLfloat vertices[] = {
    // left triangle
    -0.5, 0.0, 0.0, //
    0.0, 0.0, 0.0, // 
    0.0, 0.5, 0.0, //
    // right triangle
    0.2, 0.0, 0.0, // 
    0.7, 0.0, 0.0,
    0.7, 0.5, 0.0,
};


bool initFrameworks();
void initScene();
void render();
bool initGLFW();
bool initGLEW();
void key_callback( GLFWwindow* window, int key, int scancode, int action, int mode );
void clean();



int main()
{
    if( !initFrameworks() )
    {
	std::cout << "Failed to init frameworkd!" << std::endl;
    }

    initScene();

    while( !glfwWindowShouldClose(window) )
    {
	glfwPollEvents();

	render();

	glfwSwapBuffers( window );
    }
	
    clean();
    return 0;
}

bool initFrameworks()
{
    if( !initGLFW() )
    {
	std::cout << "Failed to init GLFW!" << std::endl;
	return false;
    }

    // init GLEW
    glewExperimental = GL_TRUE;
    if( glewInit() != GLEW_OK )
    {
	std::cout << "Failed to init GLEW!" << std::endl;
	return false;
    }

    // set viewport
    glViewport( 0, 0, WIN_WIDTH, WIN_HEIGHT );

    return true;
}
bool initGLFW()
{
    if( !glfwInit() )
    {
	std::cout << "Failed to init glfw3!" << std::endl;
	return false;
    }

    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE ); // mac specific

    window =  glfwCreateWindow( WIN_WIDTH, WIN_HEIGHT, TITLE, nullptr, nullptr );
    if( window == NULL )
    {
	std::cout << "Failed to create GLFW window!" << std::endl;
	return false;
    }

    glfwMakeContextCurrent( window );

    glfwSetKeyCallback( window, key_callback );
    

    return true;

}


void render()
{
    glClear( GL_COLOR_BUFFER_BIT );

    glBindVertexArray( VAO );
    basicShader->use();
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glDrawArrays( GL_TRIANGLES, 0, 6 );

}

void initScene()
{
    basicShader = new Shader( "basic.vert", "basic.frag" );

    glClearColor( 0.2f, 0.2f, 0.3f, 1.0f );

    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );

    glBindVertexArray( VAO );
    {
	// vbo
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );

	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0 );

    }
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    
}


void key_callback( GLFWwindow* window, int key, int scancode, int action, int mode )
{
    if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
	glfwSetWindowShouldClose( window, GL_TRUE );
}


void clean()
{
    delete basicShader;
    glfwTerminate();
}


