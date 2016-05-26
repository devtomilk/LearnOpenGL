#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <string>


#include <GL/glew.h>

#include "../fileutils.h"

class Shader
{
 public:
    // Program ID
    GLuint ProgramID;

    // Constructor
    Shader( const GLchar* vertexPath, const GLchar* fragmentPath )
    {
	// Vertex Shader
	const std::string vertSourceString = read_file( vertexPath );
	const GLchar* vertSourceCString = vertSourceString.c_str();
	GLuint vertexShaderID = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( vertexShaderID, 1, &vertSourceCString, NULL );
	glCompileShader( vertexShaderID );

	// Check for compilation errors
	GLint success;
	GLchar InfoLog[512];
	glGetShaderiv( vertexShaderID, GL_COMPILE_STATUS, &success );
	if( !success )
	{
	    glGetShaderInfoLog( vertexShaderID, 512, NULL, InfoLog );
	    std::cout << "Failed to compile vertex shader: " << InfoLog << std::endl;
	}

	// Fragment Shader
	const std::string fragSourceString = read_file( fragmentPath );
	const GLchar* fragSourceCString = fragSourceString.c_str();
	GLuint fragmentShaderID = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fragmentShaderID, 1, &fragSourceCString, NULL );
	glCompileShader( fragmentShaderID );

	// Check for compilation errors
	glGetShaderiv( fragmentShaderID, GL_COMPILE_STATUS, &success );
	if( !success )
	{
	    glGetShaderInfoLog( fragmentShaderID, 512, NULL, InfoLog );
	    std::cout << "Failed to compile vertex shader: " << InfoLog << std::endl;
	}

	// Linking program
	this->ProgramID = glCreateProgram();
	glAttachShader( this->ProgramID, vertexShaderID );
	glAttachShader( this->ProgramID, fragmentShaderID );
	glLinkProgram( this->ProgramID );
	// Check for linking errors
	glGetProgramiv( this->ProgramID, GL_LINK_STATUS, &success );
	if( !success )
	{
	    glGetProgramInfoLog( this->ProgramID, 512, NULL, InfoLog );
	    std::cout << "Failed to link program: " << InfoLog << std::endl;
	}

	glDeleteShader( vertexShaderID );
	glDeleteShader( fragmentShaderID );

    }

    // Use the program
    void use()
    {
	glUseProgram( this->ProgramID );
    }
};

#endif
