// baisc 
#include <iostream>

// glew
#include <GL/glew.h>

// glfw
#include <GLFW/glfw3.h>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// user defined
#include "../Shader.h"
#include "../Camera.h"


// variables
GLFWwindow* window = NULL;
Shader* basicShader;
Shader* lightShader;
#define WIDTH 800
#define HEIGHT 600

GLuint cubeVAO;
GLuint lightVAO;
GLuint VBO;
glm::vec3 lightPos( 0.0f, 1.2f, 0.0f );

GLfloat deltaTime = 0.0f; // Time between current frame and last frame
GLfloat lastFrame = 0.0f; // Time of last frame
//====================================================================================================
// Uniforms
//====================================================================================================
GLuint uniformLocModel;
GLuint uniformLocView;
GLuint uniformLocProjection;
GLuint uniformLocObjectColor;
GLuint uniformLocLightColor;
GLuint uniformLocLightPos;
GLuint uniformLocCameraPos;


//====================================================================================================
// Camera Stuff
//====================================================================================================
Camera* camera;
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;
glm::vec3 cameraPos;
glm::vec3 cameraFront;
glm::vec3 cameraUp;

GLfloat yaw = -90.0f;
GLfloat pitch= 0.0f;
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
GLfloat fov = 45.0f;
bool keys[1024];



// methods
bool initFrameworks();
void setupScene();
void render();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void doMovement();


GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

int main()
{
    if( !initFrameworks() )
    {
	return -1;
    }

    setupScene();

    while( !glfwWindowShouldClose(window) )
    {
	glfwPollEvents();

	doMovement();

	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	render();

	glfwSwapBuffers(window);
	
    }

    return 0;
}
//====================================================================================================
// Mouse callback
//====================================================================================================
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if( firstMouse )
    {
	lastX = xpos;
	lastY = ypos;
	firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    GLfloat sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // bounds
    if( pitch > 89.0f )
	pitch = 89.0f;
    if( pitch < -89.0f )
	pitch = -89.0f;

    glm::vec3 front;
    front.x = cos( glm::radians(yaw) ) * cos( glm::radians(pitch) );
    front.y = sin( glm::radians(pitch) );
    front.z = sin( glm::radians(yaw) ) * cos( glm::radians(pitch) );

    camera->setFront( front );
}

//====================================================================================================
// Keyboard callback
//====================================================================================================
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
	glfwSetWindowShouldClose( window, GL_TRUE );


    if( key >= 0 && key < 1024 )
    {
	if( action == GLFW_PRESS )
	    keys[key] = true;
	else if( action == GLFW_RELEASE )
	    keys[key] = false;
    }
}

bool initFrameworks()
{
    if( !glfwInit() )
    {
	std::cout << "Failed to init GLFW3!" << std::endl;
	return false;
    }


    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    // OSX specific
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    

    window = glfwCreateWindow( WIDTH, HEIGHT, "09Materials", NULL, NULL );

    if( window == NULL )
    {
	std::cout << "Failed to create window!" << std::endl;
	return false;
    }

    // set key callback
    glfwSetKeyCallback( window, key_callback );
    // set mouse callback

    glfwSetCursorPosCallback( window, mouse_callback );
    glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

    // make context current
    glfwMakeContextCurrent( window );

    // Init GLEW
    glewExperimental = GL_TRUE;
    if( glewInit() != GLEW_OK )
    {
	std::cout << "Failed to init GLEW!" << std::endl;
	return false;
    }

    glViewport( 0, 0, WIDTH, HEIGHT );

    return true;
}

void setupScene()
{
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f);


    // setup cube
    glGenVertexArrays( 1, &cubeVAO );
    glGenBuffers( 1, &VBO );

    glBindVertexArray( cubeVAO );
    {
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );

	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0 );

	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)) );
    }
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );


    // setup mvp matrix
    basicShader = new Shader( "basic.vert", "basic.frag" );
    camera = new Camera( glm::vec3(1.0,1.0,4.0), glm::vec3(0.0,1.0,0.0),
			 glm::vec3(0.0,0.0,-1.0), 45.0f, WIDTH / HEIGHT, 0.1f, 100.0f  );
    model = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    projection = glm::mat4(1.0f);

    view = glm::lookAt( camera->getPos() , camera->getPos() + camera->getFront(), camera->getUp() );
    projection = glm::perspective( camera->getFOV(), camera->getRatio(), camera->getNear(), camera->getFar() );

    basicShader->use();
    uniformLocModel = glGetUniformLocation( basicShader->ProgramID, "M" );
    uniformLocView = glGetUniformLocation( basicShader->ProgramID, "V" );
    uniformLocProjection = glGetUniformLocation( basicShader->ProgramID, "P" );
    uniformLocObjectColor = glGetUniformLocation( basicShader->ProgramID, "objectColor" );
    uniformLocLightColor = glGetUniformLocation( basicShader->ProgramID, "lightColor" );
    uniformLocLightPos= glGetUniformLocation( basicShader->ProgramID, "lightPos" );
    uniformLocCameraPos = glGetUniformLocation( basicShader->ProgramID, "cameraPos" );
    

    glUniformMatrix4fv( uniformLocModel, 1, GL_FALSE, glm::value_ptr(model) );
    glUniformMatrix4fv( uniformLocView, 1, GL_FALSE, glm::value_ptr(view) );
    glUniformMatrix4fv( uniformLocProjection, 1, GL_FALSE, glm::value_ptr(projection) );
    glUniform3f( uniformLocObjectColor, 0.5f, 0.2f, 0.0f );
    glUniform3f( uniformLocLightColor, 1.0f, 1.0f, 1.0f );
    glUniform3f( uniformLocLightPos, lightPos.x, lightPos.y, lightPos.z );
    glUniform3f( uniformLocCameraPos, camera->getPos().x,
		                      camera->getPos().y,
		                      camera->getPos().z );


    // setup light

    glGenVertexArrays( 1, &lightVAO );

    glBindVertexArray( lightVAO );
    {
	glBindBuffer( GL_ARRAY_BUFFER, VBO );

	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0 );
    }
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );


    
    lightShader = new Shader( "light.vert", "light.frag" );
    lightShader->use();

    model = glm::translate( model, lightPos );
    model = glm::scale( model, glm::vec3( 0.1, 0.1, 0.1 ) );
    uniformLocModel = glGetUniformLocation( lightShader->ProgramID, "M" );
    uniformLocView = glGetUniformLocation( lightShader->ProgramID, "V" );
    uniformLocProjection = glGetUniformLocation( lightShader->ProgramID, "P" );

    glUniformMatrix4fv( uniformLocModel, 1, GL_FALSE, glm::value_ptr(model) );
    glUniformMatrix4fv( uniformLocView, 1, GL_FALSE, glm::value_ptr(view) );
    glUniformMatrix4fv( uniformLocProjection, 1, GL_FALSE, glm::value_ptr(projection) );

}

void render()
{
    glEnable( GL_DEPTH_TEST );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


    // draw cube
    basicShader->use();
    glBindVertexArray( cubeVAO );
    view = glm::lookAt( camera->getPos() , camera->getPos() + camera->getFront(), camera->getUp() );
    uniformLocView = glGetUniformLocation( basicShader->ProgramID, "V" );
    uniformLocCameraPos = glGetUniformLocation( basicShader->ProgramID, "cameraPos" );
    glUniformMatrix4fv( uniformLocView, 1, GL_FALSE, glm::value_ptr(view) );
    glUniform3f( uniformLocCameraPos, camera->getPos().x,
		                      camera->getPos().y,
		                      camera->getPos().z );

    glDrawArrays( GL_TRIANGLES, 0, 36 );
    glBindVertexArray( 0 );

    // draw light
    lightShader->use();
    uniformLocView = glGetUniformLocation( lightShader->ProgramID, "V" );
    glUniformMatrix4fv( uniformLocView, 1, GL_FALSE, glm::value_ptr(view) );

    glBindVertexArray( lightVAO );
    glDrawArrays( GL_TRIANGLES, 0, 36 );
    glBindVertexArray( 0 );
}
//====================================================================================================
// Camera Movement
//====================================================================================================
void doMovement()
{
    GLfloat cameraSpeed = 0.01f;
    if( keys[GLFW_KEY_W] )
    {
	camera->setPos( camera->getPos() + cameraSpeed * camera->getFront() );
    }
    if( keys[GLFW_KEY_S] )
    {
	camera->setPos( camera->getPos() - cameraSpeed * camera->getFront() );
    }
    if( keys[GLFW_KEY_A] )
    {
	camera->setPos( camera->getPos() -
			cameraSpeed * glm::normalize(glm::cross(camera->getFront(), camera->getUp())) );
    }
    if( keys[GLFW_KEY_D] )
    {
	camera->setPos( camera->getPos() +
			cameraSpeed * glm::normalize(glm::cross(camera->getFront(), camera->getUp())) );
    }
	
}

void clean()
{
    delete basicShader;
    delete camera;
}
