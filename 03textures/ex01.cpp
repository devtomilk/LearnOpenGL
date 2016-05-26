// c/c++ headers
#include <iostream>
#include <cmath> 
// glew header
#define glew_static
#include <gl/glew.h>
// glfw header
#include <glfw/glfw3.h>
// soil
#include <SOIL/SOIL.h>
// user headers
#include "../fileutils.h"
#include "../Shader.h"

void key_callback( GLFWwindow* window, int key, int scancode, int action, int mode);
void render();
bool init();
bool loadAssets();


GLFWwindow* window;
GLuint vertexShader;
GLuint fragmentShader;
GLuint shaderProgram;
GLuint texture1, texture2;

GLfloat vertices[] = {
    // position         // colors          // texcoords
    -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // bottom left
    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  // bottom right 
    0.5f, 0.5f, 0.0f,    0.0f, 0.0f, 1.0f,  1.0f, 1.0f,  // top right
    -0.5f, 0.5f, 0.0f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f, // top left
};

GLuint indices[] = {
    0, 1, 2,
    2, 3, 0,
};

GLuint VBO, VAO, IBO;

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

    ourShader = new Shader( "basic.vert", "ex01.frag" );
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

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, texture1 );
    glUniform1i( glGetUniformLocation(ourShader->ProgramID, "ourTexture1"), 0 );

    glActiveTexture( GL_TEXTURE1 );
    glBindTexture( GL_TEXTURE_2D, texture2 );
    glUniform1i( glGetUniformLocation(ourShader->ProgramID, "ourTexture2"), 1 );

    glBindVertexArray( VAO );
    ourShader->use();

    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
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
    glGenBuffers( 1, &IBO );
    glGenBuffers( 1, &VBO );
    glBindVertexArray( VAO );
    {
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );
	
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0 ); 

	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	glEnableVertexAttribArray( 2 );
	glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));

    }
    glBindVertexArray( 0 );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );


    // ========================================================================================
    // Texture 1
    // ========================================================================================
    glGenTextures( 1, &texture1 );
    glBindTexture( GL_TEXTURE_2D, texture1 );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image, create texture and generate mipmaps
    int width, height;
    unsigned char* image = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
    // ========================================================================================
    // Texture 2
    // ========================================================================================
    glGenTextures( 1, &texture2 );
    glBindTexture( GL_TEXTURE_2D, texture2 );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    image = SOIL_load_image("awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );
    glBindTexture( GL_TEXTURE_2D, 0 );


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
