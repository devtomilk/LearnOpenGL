// c/c++ headers
#include <iostream>
#include <cmath> 
// glew header
#define glew_static
#include <gl/glew.h>
// glfw header
#include <glfw/glfw3.h>
// user headers
#include "fileutils.h"

void key_callback( GLFWwindow* window, int key, int scancode, int action, int mode);
void render();
bool init();
bool loadShaders();
bool loadAssets();

const std::string vertSourceString = read_file( "basic.vert" );
const std::string fragSourceString = read_file( "basic.frag" );
const GLchar* vertexShaderSource = vertSourceString.c_str();
const GLchar* fragmentShaderSource = fragSourceString.c_str();

GLFWwindow* window;
GLuint vertexShader;
GLuint fragmentShader;
GLuint shaderProgram;

GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f, // bottom left
    0.5f, -0.5f, 0.0f, // bottom right
    0.5f, 0.5f, 0.0f, // top right
    -0.5f, 0.5f, 0.0f, // top left
};

GLuint indices[] = {
    0, 1, 2, // first triangle
    2, 3, 0, // second triangle
};
GLuint VBO, VAO, IBO;

int main()
{

    // init glfw & glew
    if( !init() )
    {
	return -1;
    }


    // load shaders
    if( !loadShaders() )
    {
	return -1;
    }
    // load assets
    if( !loadAssets() )
    {
	std::cout << "failed to load assets!" << std::endl;
	return -1;
    }
    

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
    glUseProgram( shaderProgram );

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


bool loadShaders()
{
    // compile vertex shader
    vertexShader = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertexShader, 1, &vertexShaderSource, NULL );
    glCompileShader( vertexShader );

    // check compilation errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &success );
    if( !success )
    {
	glGetShaderInfoLog( vertexShader, 512, NULL, infoLog );
	std::cout << "error: failed to compile vertex shader.\n" << infoLog << std::endl;

	return false;
    }


    
    // compile fragment shader
    fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragmentShader, 1, &fragmentShaderSource, NULL );
    glCompileShader( fragmentShader );
    
    // check compilation errors
    glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &success );
    if( !success )
    {
	glGetShaderInfoLog( fragmentShader, 512, NULL, infoLog );
	std::cout << "error: failed to compile fragment shader.\n" << infoLog << std::endl;

	return false;
    }

    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader( shaderProgram, vertexShader );
    glAttachShader( shaderProgram, fragmentShader );
    glLinkProgram( shaderProgram );

    // check linking errors
    glGetProgramiv( shaderProgram, GL_LINK_STATUS, &success );
    if ( !success )
    {
	glGetProgramInfoLog( shaderProgram, 512, NULL, infoLog );
	std::cout << "error: shader linking failed" << infoLog << std::endl;
    }

    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );

    return true;
}

bool loadAssets()
{

    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );
    glGenBuffers( 1, &IBO );
    glBindVertexArray( VAO );
    {
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

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
