// It took me 100 minutes to write this little program 2016/05/27 (shader.h, fileutils.h)
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
GLuint IBO;
Shader* basicShader = NULL;
GLuint texture1;
GLuint texture2;

GLfloat vertices[] = {
    // position          // color         // texture coords
    -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f,    // bottom left
    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,    // bottom right
    -0.5f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,    // top left
    0.5f, 0.5f, 0.0f,    0.5f, 0.5f, 0.0f,   1.0f, 1.0f,    // top right
};

GLuint indices[] = {
    0, 1, 2,
    2, 3, 1,
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

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, texture1 );
    glUniform1i( glGetUniformLocation( basicShader->program, "myTexture1" ), 0 );

    glActiveTexture( GL_TEXTURE1 );
    glBindTexture( GL_TEXTURE_2D, texture2 );
    glUniform1i( glGetUniformLocation( basicShader->program, "myTexture2" ), 1 );

    glBindVertexArray( VAO );
    basicShader->use();
    //glDrawArrays( GL_TRIANGLES, 0, 36 );
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );

}

void initScene()
{
    basicShader = new Shader( "basic.vert", "basic.frag" );

    glClearColor( 0.2f, 0.2f, 0.3f, 1.0f );

    //--------------------------------------------------
    // Geometry
    //--------------------------------------------------
    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );
    glGenBuffers( 1, &IBO );

    glBindVertexArray( VAO );
    {
	// vbo
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );

	// position
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0 );

	// color
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)) );

	// texture coordinates
	glEnableVertexAttribArray( 2 );
	glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)) );

	// ibo
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW );
    }
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    //--------------------------------------------------
    // Textures
    //--------------------------------------------------
    glGenTextures( 1, &texture1 );
    glBindTexture( GL_TEXTURE_2D, texture1 );

    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT );
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    int width, height;
    unsigned char* image = SOIL_load_image( "../../container.jpg", &width, &height, 0, SOIL_LOAD_RGB );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );
    glBindTexture( GL_TEXTURE_2D, 0 );

    // second texture
    glGenTextures( 1, &texture2 );
    glBindTexture( GL_TEXTURE_2D, texture2 );

    image = SOIL_load_image( "../../awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );
    glBindTexture( GL_TEXTURE_2D, 0 );
    
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


