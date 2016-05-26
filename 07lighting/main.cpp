// language
#include <iostream>

// frameworks
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// user defined
#include "../Shader.h"
#include "../fileutils.h"

#define WIN_WIDTH  800
#define WIN_HEIGHT  600

GLuint lampVAO;
GLuint cubeVAO;
GLuint VBO;

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

GLuint uniformLocModel;
GLuint uniformLocView;
GLuint uniformLocProjection;
GLuint uniformLocModelColor;
GLuint uniformLocLightColor;

Shader* basicShader;
Shader* lampShader;


GLfloat vertices[] = {
    -0.5f, -0.5f, -0.5f,  
     0.5f, -0.5f, -0.5f,  
     0.5f,  0.5f, -0.5f,  
     0.5f,  0.5f, -0.5f,  
    -0.5f,  0.5f, -0.5f,  
    -0.5f, -0.5f, -0.5f,  

    -0.5f, -0.5f,  0.5f,  
     0.5f, -0.5f,  0.5f,  
     0.5f,  0.5f,  0.5f,  
     0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f,  0.5f,  
    -0.5f, -0.5f,  0.5f,  

    -0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f, -0.5f,  
    -0.5f, -0.5f, -0.5f,  
    -0.5f, -0.5f, -0.5f,  
    -0.5f, -0.5f,  0.5f,  
    -0.5f,  0.5f,  0.5f,  

     0.5f,  0.5f,  0.5f,  
     0.5f,  0.5f, -0.5f,  
     0.5f, -0.5f, -0.5f,  
     0.5f, -0.5f, -0.5f,  
     0.5f, -0.5f,  0.5f,  
     0.5f,  0.5f,  0.5f,  

    -0.5f, -0.5f, -0.5f,  
     0.5f, -0.5f, -0.5f,  
     0.5f, -0.5f,  0.5f,  
     0.5f, -0.5f,  0.5f,  
    -0.5f, -0.5f,  0.5f,  
    -0.5f, -0.5f, -0.5f,  

    -0.5f,  0.5f, -0.5f,  
     0.5f,  0.5f, -0.5f,  
     0.5f,  0.5f,  0.5f,  
     0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f, -0.5f, 
};

GLFWwindow* window;
bool initFrameworks();
bool loadAssets();
void render();
void key_callback( GLFWwindow* window, int key, int scancode, int action, int mode );
void clean();

int main()
{
    if( !initFrameworks() )
    {
	return -1;
    }

    // create shaders
    basicShader = new Shader( "basic.vert", "basic.frag" );
    lampShader = new Shader( "basic.vert", "lamp.frag" );
    // load assets
    if( !loadAssets() )
    {
	std::cout << "Failed to load assets!" << std::endl;
    }

    while( !glfwWindowShouldClose( window ) )
    {
	glfwPollEvents();

	render();
    }

    

    clean();
    return 0;
}

void render()
{
    glEnable( GL_DEPTH_TEST );
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // render cube
    basicShader->use();
    glBindVertexArray( cubeVAO );
    glDrawArrays( GL_TRIANGLES, 0 , 36 );

    // render lamp
    lampShader->use();
    glBindVertexArray( lampVAO );
    glDrawArrays( GL_TRIANGLES, 0, 36 );

    glfwSwapBuffers( window );

}

bool initFrameworks()
{
    // GLFW
    if( !glfwInit() )
    {
	std::cout << "Failed to init glfw!" << std::endl;
	return false;
    }

    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    // mac specific
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );

    // create a glfw window
    window = glfwCreateWindow( WIN_WIDTH, WIN_HEIGHT, "color", nullptr, nullptr );
    if( window == nullptr )
    {
	std::cout << "Faield to create glfw window!" << std::endl;
	glfwTerminate();
	return false;
    }
    glfwMakeContextCurrent( window );
    // GLEW
    glewExperimental = GL_TRUE;
    if( glewInit() != GLEW_OK )
    {
	std::cout << "Failed to inti glew!" << std::endl;
	return false;
    }

    // set opengl viewport size
    glViewport( 0, 0, WIN_WIDTH, WIN_HEIGHT );


    // key callback
    glfwSetKeyCallback( window, key_callback );
    return true;
}




void key_callback( GLFWwindow* window, int key, int scancode, int action, int mode )
{
    if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
    {
	glfwSetWindowShouldClose( window, GL_TRUE );
    }
}

bool loadAssets()
{
    // cube buffers
    glGenVertexArrays( 1, &cubeVAO );
    glGenBuffers( 1, &VBO );

    glBindVertexArray( cubeVAO );
    {
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );

	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0 );
    }
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    // lamp buffers
    glGenVertexArrays( 1, &lampVAO );
    glBindVertexArray( lampVAO );
    {
	glBindBuffer( GL_ARRAY_BUFFER, VBO );

	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0 );
    }
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    // set up cube
    basicShader->use();
    model = glm::mat4();
    view = glm::lookAt( glm::vec3(1.0, 1.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0) );
    projection = glm::perspective( 45.0f, (GLfloat)(WIN_WIDTH / WIN_HEIGHT), 0.1f, 100.0f );

    uniformLocModel = glGetUniformLocation( basicShader->ProgramID, "M" );
    uniformLocView = glGetUniformLocation( basicShader->ProgramID, "V" );
    uniformLocProjection = glGetUniformLocation( basicShader->ProgramID, "P" );

    glUniformMatrix4fv( uniformLocModel, 1, GL_FALSE, glm::value_ptr(model) );
    glUniformMatrix4fv( uniformLocView, 1, GL_FALSE, glm::value_ptr(view) );
    glUniformMatrix4fv( uniformLocProjection, 1, GL_FALSE, glm::value_ptr(projection) );

    uniformLocModelColor = glGetUniformLocation( basicShader->ProgramID, "modelColor" );
    uniformLocLightColor = glGetUniformLocation( basicShader->ProgramID, "lightColor" );

    glUniform3f( uniformLocModelColor, 1.0f, 0.5f, 0.31f );
    glUniform3f( uniformLocLightColor, 1.0f, 1.0f, 1.0f );
    // set up lamp
    lampShader->use();
    glm::vec3 lampPos( 4.2f, 1.0f, 2.0f );
    model = glm::mat4();
    model = glm::scale( model, glm::vec3(0.2,0.2,0.2) );
    model = glm::translate( model, lampPos );

    uniformLocModel = glGetUniformLocation( lampShader->ProgramID, "M" );
    uniformLocView = glGetUniformLocation( lampShader->ProgramID, "V" );
    uniformLocProjection = glGetUniformLocation( lampShader->ProgramID, "P" );

    glUniformMatrix4fv( uniformLocModel, 1, GL_FALSE, glm::value_ptr(model) );
    glUniformMatrix4fv( uniformLocView, 1, GL_FALSE, glm::value_ptr(view) );
    glUniformMatrix4fv( uniformLocProjection, 1, GL_FALSE, glm::value_ptr(projection) );



    return true;
}

void clean()
{
    delete basicShader;
    delete lampShader;
}
