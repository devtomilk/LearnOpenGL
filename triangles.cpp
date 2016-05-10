#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

void key_callback( GLFWwindow* window, int key, int scancode, int action, int mode);
void render();


int main()
{
    if( !glfwInit() )
    {
	std::cout << "Failed to init GLFW!" << std::endl;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    // mac specific
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow( 800, 600, "LearnOpenGL", nullptr, nullptr );
    if( window == nullptr )
    {
	std::cout << "Failed to create GLFW window" << std::endl;
	glfwTerminate();
	return -1;
    }

    glfwMakeContextCurrent(window);

    // Init GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
	std::cout << "Failed to initialize GLEW" << std::endl;
	return -1;
    }

    // Set OpenGL viewport size
    int width, height;
    glfwGetFramebufferSize( window, &width, &height );
    glViewport( 0, 0, width, height );

    glfwSetKeyCallback( window, key_callback );
    
    while( !glfwWindowShouldClose( window ) )
    {
	glfwPollEvents();

	render();

	glfwSwapBuffers( window );
    }


    glfwTerminate();
    return 0;
}
void render()
{
    glClearColor( 1.0f, 0.3f, 0.3f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );
}

void key_callback( GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
    {
	glfwSetWindowShouldClose( window,GL_TRUE );
    }
}
