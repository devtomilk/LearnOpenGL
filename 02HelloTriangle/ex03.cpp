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
GLuint leftVAO;
GLuint leftVBO;

GLuint rightVAO;
GLuint rightVBO;

Shader* leftShader = NULL;
Shader* rightShader = NULL;

GLfloat left_vertices[] = {
    // left triangle
    -0.5, 0.0, 0.0, //
    0.0, 0.0, 0.0, // 
    0.0, 0.5, 0.0, //
};

GLfloat right_vertices[] = {
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

    // left triangle
    glBindVertexArray( leftVAO );
    leftShader->use();
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glDrawArrays( GL_TRIANGLES, 0, 3 );
    glBindVertexArray( 0 );


    // right triangle
    glBindVertexArray( rightVAO );
    rightShader->use();
    glDrawArrays( GL_TRIANGLES, 0, 3 );
    glBindVertexArray( 0 );



}

void initScene()
{
    leftShader = new Shader( "basic.vert", "basic.frag" );
    rightShader = new Shader( "basic.vert", "ex03.frag" );

    glClearColor( 0.2f, 0.2f, 0.3f, 1.0f );

    // left triangle
    glGenVertexArrays( 1, &leftVAO );
    glGenBuffers( 1, &leftVBO );

    glBindVertexArray( leftVAO );
    {
	// vbo
	glBindBuffer( GL_ARRAY_BUFFER, leftVBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof(left_vertices), left_vertices, GL_STATIC_DRAW );

	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0 );

    }
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    // right triangle
    glGenVertexArrays( 1, &rightVAO );
    glGenBuffers( 1, &rightVBO );

    glBindVertexArray( rightVAO );
    {
	// vbo
	glBindBuffer( GL_ARRAY_BUFFER, rightVBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof(right_vertices), right_vertices, GL_STATIC_DRAW );

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
    delete leftShader;
    delete rightShader;
    glfwTerminate();
}


