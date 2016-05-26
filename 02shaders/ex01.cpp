// c/c++ headers
#include <iostream>
#include <cmath> 
// glew header
#define glew_static
#include <gl/glew.h>
// glfw header
#include <glfw/glfw3.h>
// user headers
#include "../fileutils.h"
#include "Shader.h"

void key_callback( GLFWwindow* window, int key, int scancode, int action, int mode);
void render();
bool init();
bool loadAssets();


GLFWwindow* window;
GLuint vertexShader;
GLuint fragmentShader;
GLuint shaderProgram;

GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f, // bottom left
    0.5f, -0.5f, 0.0f, // bottom right
    0.0f, 0.5f, 0.0f, // top
};

GLuint VBO, VAO;

Shader* ourShader;

int main()
{

    // init glfw & glew
    if( !init() )
    {
	return -1;
    }

    // load assets
    if( !loadAssets() )
    {
	std::cout << "failed to load assets!" << std::endl;
	return -1;
    }

    ourShader = new Shader( "ex01.vert", "ex01.frag" );
    //glpolygonmode( gl_front_and_back, gl_line ); // draw only outlines
    while( !glfwWindowShouldClose( window ) )
    {
	glfwPollEvents();

	render();
    }

    
    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers( 1, &VBO );
    glfwTerminate();
    return 0;
}
void render()
{
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );

    glBindVertexArray( VAO );
    ourShader->use();

    glDrawArrays( GL_TRIANGLES, 0, 3 );
    glBindVertexArray( 0 );

    glfwSwapBuffers( window );
}

void key_callback( GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
    {
	glfwSetWindowShouldClose( window, GL_TRUE );
    }
}


bool loadAssets()
{

    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );
    glBindVertexArray( VAO );
    {
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0 ); 

    }
    glBindVertexArray( 0 );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

    return true;
}

bool init()
{
    if( !glfwInit() )
    {
	std::cout << "failed to init glfw!" << std::endl;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    // mac specific
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // create a glfw window
    window = glfwCreateWindow( 800, 600, "learnopengl", nullptr, nullptr );
    if( window == nullptr )
    {
	std::cout << "failed to create glfw window" << std::endl;
	glfwTerminate();
	return -1;
    }

    glfwMakeContextCurrent(window);

    // init glew
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
	std::cout << "failed to initialize glew" << std::endl;
	return -1;
    }

    // set opengl viewport size
    int width, height;
    glfwGetFramebufferSize( window, &width, &height );
    glViewport( 0, 0, width, height );

    glfwSetKeyCallback( window, key_callback );

    return true;
}
