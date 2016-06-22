// It took me 100 minutes to write this little program 2016/05/27 (shader.h, fileutils.h)
// language
#include <iostream>
#include <math.h>

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
#include "../Camera.h"
//====================================================================================================
// Variables
//====================================================================================================
#define WIN_WIDTH 1024
#define WIN_HEIGHT 768
#define TITLE "SpotLight"
GLFWwindow* window;
bool keys[1024];
// buffers
GLuint cubeVAO;
GLuint cube2VAO;
GLuint cube3VAO;
GLuint cube4VAO;

GLuint VBO;
// shaders
Shader* cubeShader = NULL;
Material* material;

// Textures
GLuint diffuseMap;
GLuint specularMap;

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

glm::vec3 lightColor;
glm::vec3 objectColor;

GLfloat vertices[] = {
        // Positions          // Normals           // Texture Coords
    // back face
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

	// front face
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

	// left face
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	// right face
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	// bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

	// top face
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};


//====================================================================================================
// methods
//====================================================================================================
bool initFrameworks();
void initScene();
void render();
bool initGLFW();
bool initGLEW();
void key_callback( GLFWwindow* window, int key, int scancode, int action, int mode );
void mouse_callback( GLFWwindow* window, double xpos, double ypos);
void doMovement();
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

//====================================================================================================
// Render
//====================================================================================================

void render()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    cubeShader->use();

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, diffuseMap );
    glUniform1i( glGetUniformLocation( cubeShader->program, "material.diffuse" ), 0 );

    glActiveTexture( GL_TEXTURE1 );
    glBindTexture( GL_TEXTURE_2D, specularMap );
    glUniform1i( glGetUniformLocation( cubeShader->program, "material.specular" ), 1 );

    /*
    glActiveTexture( GL_TEXTURE1 );
    glBindTexture( GL_TEXTURE_2D, texture2 );
    glUniform1i( glGetUniformLocation( cubeShader->program, "myTexture2" ), 1 );
    */

    // transformation
    view = glm::lookAt( camera->getPos(), camera->getPos() + camera->getFront(), camera->getUp() );
    uniformLocView = glGetUniformLocation( cubeShader->program, "V" );
    glUniformMatrix4fv( uniformLocView, 1, GL_FALSE, glm::value_ptr(view) );

    projection = glm::perspective( camera->getFov(), (GLfloat)WIN_WIDTH/WIN_HEIGHT, camera->getNear(),
				   camera->getFar() );
    glUniformMatrix4fv( glGetUniformLocation( cubeShader->program, "P" ), 1, GL_FALSE, glm::value_ptr(projection) );
    // providing camera pos and spot light pos
    glUniform3f( glGetUniformLocation( cubeShader->program, "cameraPos" ),
		 camera->getPos().x,
		 camera->getPos().y,
		 camera->getPos().z );

    glUniform3f( glGetUniformLocation(cubeShader->program, "light.position"),
		 camera->getPos().x,
		 camera->getPos().y,
		 camera->getPos().z );

    glUniform3f( glGetUniformLocation(cubeShader->program, "light.direction"),
		 camera->getFront().x,
		 camera->getFront().y,
		 camera->getFront().z );

    // draw cube
    model = glm::mat4( 1.0f );
    glUniformMatrix4fv( glGetUniformLocation( cubeShader->program, "M" ), 1, GL_FALSE, glm::value_ptr(model) );
    glBindVertexArray( cubeVAO );
    glDrawArrays( GL_TRIANGLES, 0, 36 );
    glBindVertexArray( 0 );

    // draw cube2
    model = glm::translate( model, glm::vec3(4.0f, 0.0f, 0.0f) );
    glUniformMatrix4fv( glGetUniformLocation( cubeShader->program, "M" ), 1, GL_FALSE, glm::value_ptr(model) );
    glBindVertexArray( cube2VAO );
    glDrawArrays( GL_TRIANGLES, 0, 36 );
    glBindVertexArray( 0 );

    // draw cube3
    model = glm::translate( model, glm::vec3(0.0f, 0.0f, -3.0f) );
    glUniformMatrix4fv( glGetUniformLocation( cubeShader->program, "M" ), 1, GL_FALSE, glm::value_ptr(model) );
    glBindVertexArray( cube3VAO );
    glDrawArrays( GL_TRIANGLES, 0, 36 );
    glBindVertexArray( 0 );

    // draw cube4
    model = glm::translate( model, glm::vec3(0.0f, 0.0f, -5.0f) );
    glUniformMatrix4fv( glGetUniformLocation( cubeShader->program, "M" ), 1, GL_FALSE, glm::value_ptr(model) );
    glBindVertexArray( cube4VAO );
    glDrawArrays( GL_TRIANGLES, 0, 36 );
    glBindVertexArray( 0 );
    


}

//====================================================================================================
// Init scene
//====================================================================================================

void initScene()
{
    cubeShader = new Shader( "basic.vert", "spot.frag" );

    glClearColor( 0.2f, 0.2f, 0.3f, 1.0f );
    glEnable( GL_DEPTH_TEST );

    //--------------------------------------------------
    // Geometry
    //--------------------------------------------------
    // cube
    glGenVertexArrays( 1, &cubeVAO );
    glGenBuffers( 1, &VBO );

    glBindVertexArray( cubeVAO );
    {
	// vbo
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );

	// position
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0 );

	// normals
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)) );

	// tex coords
	glEnableVertexAttribArray( 2 );
	glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)) );

    }
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    // cube2
    glGenVertexArrays( 1, &cube2VAO );
    glBindVertexArray( cube2VAO );
    {
	// vbo
	glBindBuffer( GL_ARRAY_BUFFER, VBO );

	// position
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0 );

	// normals
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)) );

	// tex coords
	glEnableVertexAttribArray( 2 );
	glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)) );
    }
    glBindVertexArray( 0 );
    // cube3
    glGenVertexArrays( 1, &cube3VAO );
    glBindVertexArray( cube3VAO );
    {
	// vbo
	glBindBuffer( GL_ARRAY_BUFFER, VBO );

	// position
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0 );

	// normals
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)) );

	// tex coords
	glEnableVertexAttribArray( 2 );
	glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)) );
    }
    glBindVertexArray( 0 );

    // cube4
    glGenVertexArrays( 1, &cube4VAO );
    glBindVertexArray( cube4VAO );
    {
	// vbo
	glBindBuffer( GL_ARRAY_BUFFER, VBO );

	// position
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0 );

	// normals
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)) );

	// tex coords
	glEnableVertexAttribArray( 2 );
	glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)) );
    }
    glBindVertexArray( 0 );


    //--------------------------------------------------
    // Textures
    //--------------------------------------------------
    // specular
    glGenTextures( 1, &diffuseMap );
    glBindTexture( GL_TEXTURE_2D, diffuseMap );

    int width, height;
    unsigned char* image = SOIL_load_image( "../../container2.png", &width, &height, 0, SOIL_LOAD_RGB );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );
    glBindTexture( GL_TEXTURE_2D, 0 );

    // diffuse
    glGenTextures( 1, &specularMap );
    glBindTexture( GL_TEXTURE_2D, specularMap );

    image = SOIL_load_image( "../../container2_specular.png", &width, &height, 0, SOIL_LOAD_RGB );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );
    glBindTexture( GL_TEXTURE_2D, 0 );
    //--------------------------------------------------
    // Textures
    //--------------------------------------------------
    /*
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
    */


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

    cubeShader->use();
    uniformLocView = glGetUniformLocation( cubeShader->program, "V" );
    glUniformMatrix4fv( glGetUniformLocation(cubeShader->program, "M"), 1, GL_FALSE, glm::value_ptr(model) );
    glUniformMatrix4fv( uniformLocView, 1, GL_FALSE, glm::value_ptr(view) );
    glUniformMatrix4fv( glGetUniformLocation(cubeShader->program, "P"), 1, GL_FALSE, glm::value_ptr(projection) );

    // setup material
    glUniform3f( glGetUniformLocation(cubeShader->program, "material.specular"), 0.5f, 0.5f, 0.5f );
    glUniform1f( glGetUniformLocation(cubeShader->program, "material.shininess"), 64.0f );
    // setup lighting parameters
    glUniform3f( glGetUniformLocation(cubeShader->program, "light.ambient"), 0.2f, 0.2f, 0.2f );
    glUniform3f( glGetUniformLocation(cubeShader->program, "light.diffuse"), 0.7f, 0.7f, 0.7f );
    glUniform3f( glGetUniformLocation(cubeShader->program, "light.specular"), 1.0f, 1.0f, 1.0f );
    glUniform1f( glGetUniformLocation(cubeShader->program, "light.kConstant"), 1.0f );
    glUniform1f( glGetUniformLocation(cubeShader->program, "light.kLinear"),  0.09f );
    glUniform1f( glGetUniformLocation(cubeShader->program, "light.kQuadratic"), 0.032f );
    glUniform1f( glGetUniformLocation(cubeShader->program, "light.cutOff"), glm::cos(glm::radians(12.5f)) );
    glUniform1f( glGetUniformLocation(cubeShader->program, "light.outerCutOff"), glm::cos(glm::radians(25.0f)) );


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
    delete cubeShader;
    delete camera;
    delete material;
    glfwTerminate();
}


