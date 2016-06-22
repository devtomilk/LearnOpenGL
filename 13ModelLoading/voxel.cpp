// It took me 100 minutes to write this little program 2016/05/27 (shader.h, fileutils.h)
// language
#include <iostream>
#include <math.h>

// glew
#define GLEW_STATIC
#include <GL/glew.h>

// glm
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// glfw
#include <GLFW/glfw3.h>

// soil
#include <SOIL/SOIL.h>


// user defined
#include "../Model.h"
#include "../Shader.h"
#include "../Camera.h"
//====================================================================================================
// Variables
//====================================================================================================
#define WIN_WIDTH 1024
#define WIN_HEIGHT 768
#define TITLE "Voxelj"
GLFWwindow* window;
bool keys[1024];
// buffers
Model* objModel;
Model* oldGuy;
Model* ephtracy;
// shaders
Shader* voxelShader = NULL;


Camera* camera;
// mouse
bool firstMouse = true;
GLfloat lastX;
GLfloat lastY;
GLfloat cameraYaw = -90.0f;
GLfloat cameraPitch = 0.0f;
void scroll_callback( GLFWwindow* window, double xoffset, double yoffset);

// mvp
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

GLuint uniformLocView;
GLuint uniformLocLightColor;
GLuint uniformLocObjectColor;
GLuint uniformLocCameraPos;
GLuint uniformLocLightPos;


//====================================================================================================
// methods
//====================================================================================================
bool initFrameworks();
void initScene();
void updateViewMatrix();
void render();
bool initGLFW();
bool initGLEW();
void key_callback( GLFWwindow* window, int key, int scancode, int action, int mode );
void mouse_callback( GLFWwindow* window, double xpos, double ypos);
void doMovement();
void clean();




//====================================================================================================
// Render
//====================================================================================================

void render()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


    // update view matrix
    updateViewMatrix();

    // draw obj
    oldGuy->draw( voxelShader );

}

void updateViewMatrix()
{
    // update view matrix
    view = glm::lookAt( camera->getPos(), camera->getPos() + camera->getFront(), camera->getUp() );
    uniformLocView = glGetUniformLocation( voxelShader->program, "V" );
    glUniformMatrix4fv( uniformLocView, 1, GL_FALSE, glm::value_ptr(view) );

}

//====================================================================================================
// Init scene
//====================================================================================================

void initScene()
{
    voxelShader = new Shader( "basic.vert", "voxel_basic.frag" );

    glClearColor( 0.2f, 0.2f, 0.3f, 1.0f );
    glEnable( GL_DEPTH_TEST );

    //--------------------------------------------------
    // MVP Matrix
    //--------------------------------------------------
    // setup camera
    camera = new Camera( glm::vec3(1.0f,0.0f,3.0f), // position
			 glm::vec3(0.0f,0.0f,-1.0f), // front
			 glm::vec3(0.0f,1.0f,0.0f), // up
			 45.0f, // fov
			 0.1f, // near
			 100.0f ); // far
    model = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    view = glm::lookAt(camera->getPos(), camera->getPos() + camera->getFront(), camera->getUp() );
    projection = glm::mat4(1.0f);
    projection = glm::perspective( camera->getFov(), (GLfloat)WIN_WIDTH/WIN_HEIGHT, camera->getNear(),
				   camera->getFar() );

    voxelShader->use();
    uniformLocView = glGetUniformLocation( voxelShader->program, "V" );
    glUniformMatrix4fv( glGetUniformLocation(voxelShader->program, "M"), 1, GL_FALSE, glm::value_ptr(model) );
    glUniformMatrix4fv( uniformLocView, 1, GL_FALSE, glm::value_ptr(view) );
    glUniformMatrix4fv( glGetUniformLocation(voxelShader->program, "P"), 1, GL_FALSE, glm::value_ptr(projection) );

    // setup lighting parameters
    // directional light
    glUniform3f( glGetUniformLocation(voxelShader->program, "dirLight.direction"), -5.0f, -1.0f, -2.0f );
    glUniform3f( glGetUniformLocation(voxelShader->program, "dirLight.ambient"), 0.2f, 0.2f, 0.2f );
    glUniform3f( glGetUniformLocation(voxelShader->program, "dirLight.diffuse"), 0.7f, 0.7f, 0.7f );
    glUniform3f( glGetUniformLocation(voxelShader->program, "dirLight.specular"), 1.0f, 1.0f, 1.0f );
    // point lights
    /*
    glUniform3f( glGetUniformLocation(voxelShader->program, "pointLights[0].position"), lampsPositions[0].x,
		    lampsPositions[0].y,
		    lampsPositions[0].z );
    glUniform3f( glGetUniformLocation(voxelShader->program, "pointLights[0].ambient"), 0.2f, 0.2f, 0.2f );
    glUniform3f( glGetUniformLocation(voxelShader->program, "pointLights[0].diffuse"), 0.7f, 0.7f, 0.7f );
    glUniform3f( glGetUniformLocation(voxelShader->program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f );
    glUniform1f( glGetUniformLocation(voxelShader->program, "pointLights[0].kConstant"), 1.0f );
    glUniform1f( glGetUniformLocation(voxelShader->program, "pointLights[0].kLinear"),  0.09f );
    glUniform1f( glGetUniformLocation(voxelShader->program, "pointLights[0].kQuadratic"), 0.032f );
    */

    // flashlight
    glUniform3f( glGetUniformLocation(voxelShader->program, "spotLight.ambient"), 0.2f, 0.2f, 0.2f );
    glUniform3f( glGetUniformLocation(voxelShader->program, "spotLight.diffuse"), 0.7f, 0.7f, 0.7f );
    glUniform3f( glGetUniformLocation(voxelShader->program, "spotLight.specular"), 1.0f, 1.0f, 1.0f );
    glUniform1f( glGetUniformLocation(voxelShader->program, "spotLight.kConstant"), 1.0f );
    glUniform1f( glGetUniformLocation(voxelShader->program, "spotLight.kLinear"),  0.09f );
    glUniform1f( glGetUniformLocation(voxelShader->program, "spotLight.kQuadratic"), 0.032f );
    glUniform1f( glGetUniformLocation(voxelShader->program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)) );
    glUniform1f( glGetUniformLocation(voxelShader->program, "spotLight.outerCutOff"), glm::cos(glm::radians(25.0f)) );



    // Model
    //objModel = new Model( "models/nanosuit/nanosuit.obj" );
    //    objModel->setScale( 0.5f );
    oldGuy = new Model( "models/oldGuy/chr_old.obj" );
    oldGuy->setScale( 0.5f );

Model* ephtracy;
}


void key_callback( GLFWwindow* window, int key, int scancode, int action, int mode )
{
    if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
	glfwSetWindowShouldClose( window, GL_TRUE );

    if( action == GLFW_PRESS )
    {
	keys[key] = true;
    }

    if( action == GLFW_RELEASE )
    {
	keys[key] = false;
    }
}

void doMovement()
{
    GLfloat speed = 0.05f;
    if( keys[GLFW_KEY_W] )
    {
	camera->setPos( camera->getPos() + camera->getFront() * speed );
    }

    if( keys[GLFW_KEY_S] )
    {
	camera->setPos( camera->getPos() - camera->getFront() * speed );
    }

    if( keys[GLFW_KEY_A] )
    {
	camera->setPos( camera->getPos() - glm::cross(camera->getFront(), camera->getUp()) * speed );
    }

    if( keys[GLFW_KEY_D] )
    {
	camera->setPos( camera->getPos() + glm::cross(camera->getFront(), camera->getUp()) * speed );
    }
}

//====================================================================================================
// Mouse Movement
//====================================================================================================
void mouse_callback( GLFWwindow* window, double xpos, double ypos)
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

    cameraYaw += xoffset;
    cameraPitch += yoffset;

    if( cameraPitch > 89.0f )
    {
	cameraPitch = 89.0f;
    }
    if( cameraPitch < -89.0f )
    {
	cameraPitch = -89.0f;
    }

    glm::vec3 front;
    front.x = cos(glm::radians(cameraPitch)) * cos(glm::radians(cameraYaw));
    front.y = sin(glm::radians(cameraPitch));
    front.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));

    camera->setFront( front );
}

//====================================================================================================
// Mouse Scroll
//====================================================================================================

void scroll_callback( GLFWwindow* window, double xoffset, double yoffset)
{
    float scrollSensitivity = 0.02f;
    if( camera->getFov() >= 1.0f && camera->getFov() <= 45.0f )
	camera->setFov( camera->getFov() - yoffset * scrollSensitivity )  ;
    if( camera->getFov() <= 1.0f )
    {
	camera->setFov( 1.0f );
    }
    if( camera->getFov() >= 45.0f )
	camera->setFov( 45.0f );
}

void clean()
{
    delete voxelShader;
    delete camera;
    glfwTerminate();
}



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

	doMovement();
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
    
    glfwSetCursorPosCallback( window, mouse_callback );

    glfwSetScrollCallback( window, scroll_callback );

    glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );


    return true;

}
