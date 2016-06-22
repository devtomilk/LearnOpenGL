#ifndef SHADER_H_
#define SHADER_H_

#include <iostream>

#include <GL/glew.h>
// user defined
#include "fileutils.h"

class Shader {
 public:
    GLuint program; // shader program ID

    Shader( const GLchar* vertexPath, const GLchar* fragmentPath )
    {
	std::string vShaderSourceString = read_file( vertexPath );
	std::string fShaderSourceString = read_file( fragmentPath );

	const GLchar* vShaderSource = vShaderSourceString.c_str();
	const GLchar* fShaderSource = fShaderSourceString.c_str();

	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];


	// Vertex Shader
	vertex = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( vertex, 1, &vShaderSource, NULL );
	glCompileShader( vertex );

	// errors
	glGetShaderiv( vertex, GL_COMPILE_STATUS, &success );
	if( !success )
	{
	    glGetShaderInfoLog( vertex, 512, NULL, infoLog );
	    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	// Fragment Shader
	fragment = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fragment, 1, &fShaderSource, NULL );
	glCompileShader( fragment );

	// errors
	glGetShaderiv( fragment, GL_COMPILE_STATUS, &success );
	if( !success )
	{
	    glGetShaderInfoLog( fragment, 512, NULL, infoLog );
	    std::cout << "ERROR::SHADER::FRAGMENT:COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Shader program
	this->program = glCreateProgram();
	glAttachShader( this->program, vertex );
	glAttachShader( this->program, fragment );
	glLinkProgram( this->program );

	// errors
	glGetProgramiv( this->program, GL_LINK_STATUS, &success );
	if( !success )
	{
	    glGetProgramInfoLog( this->program, 512, NULL, infoLog );
	    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Delete the shaders as they're linked into our program now and are no longer needed
	glDeleteShader( vertex );
	glDeleteShader( fragment );

    };

    void use()
    {
	glUseProgram( this->program );
    };
 private:
};

#endif
