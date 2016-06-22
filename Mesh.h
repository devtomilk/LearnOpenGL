#pragma once

// Std includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;
// GL includes
#include <GL/glew.h> // Contains all the necessary OpenGL inlcudes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

struct Vertex {
    // Position
    glm::vec3 position;
    // Normal
    glm::vec3 normal; 
    // TexCoords
    glm::vec2 texCoords;
};

struct Texture {
    GLuint id;
    string type;
    aiString path;
};

class Mesh {
 public:
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;

    //====================================================================================================
    // Constructor
    //====================================================================================================
    Mesh( vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures )
    {
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	this->setupMesh(); // Upload data to GPU
    }

    //====================================================================================================
    // Render
    //====================================================================================================
    void setScale( GLfloat scale )
    {
	this->scale = scale;
    }
    void draw( Shader* shader )
    {
	//----------------------------------------------------------------------------------------------------
	// Bind appropriate textures
	//----------------------------------------------------------------------------------------------------
	GLuint diffuseNumSuffix = 1;
	GLuint specularNumSuffix = 1;
	for( GLuint i = 0; i < this->textures.size(); i++ )
	{
	    glActiveTexture( GL_TEXTURE0 + i ); // Activate new texture uint before binding


	    // FIXME: C way is better? This is too messy a way to get the number suffix
	    stringstream ss;
	    string number;
	    string name = this->textures[i].type;
	    if( name == "texture_diffuse" )
		ss << diffuseNumSuffix++;
	    else if( name == "texture_specular" )
		ss << specularNumSuffix++;
	    number = ss.str();

	    // Now set the sampler to the corresponding new texture unit
	    glUniform1i( glGetUniformLocation( shader->program, (name + number).c_str()), i );

	    // And finally bind the texture
	    glBindTexture( GL_TEXTURE_2D, this->textures[i].id );
	    
	}

	// Set mesh's default shininess property to a default value
	glUniform1f( glGetUniformLocation(shader->program, "material.shininess"), 16.0f );


	//----------------------------------------------------------------------------------------------------
	// Draw call
	//----------------------------------------------------------------------------------------------------
	glm::mat4 modelMatrix = glm::mat4( 1.0f );
	modelMatrix = glm::scale( modelMatrix, glm::vec3(this->scale, this->scale, this->scale) );
	glUniformMatrix4fv( glGetUniformLocation( shader->program, "M" ), 1, GL_FALSE, glm::value_ptr(modelMatrix) );
	glBindVertexArray( this->VAO );
	glDrawElements( GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0 );
	glBindVertexArray( 0 );

	//----------------------------------------------------------------------------------------------------
	// Cleanup
	//----------------------------------------------------------------------------------------------------
	for( GLuint i = 0; i < this->textures.size(); i++ )
	{
	    glActiveTexture( GL_TEXTURE0 + i );
	    glBindTexture( GL_TEXTURE_2D, 0 );
	}
	
    }
 private:
    GLuint VAO, VBO, EBO;
    GLfloat scale;

    void setupMesh()
    {
	//
	glGenVertexArrays( 1, &this->VAO );
	glGenBuffers( 1, &this->VBO );
	glGenBuffers( 1, &this->EBO );

	glBindVertexArray( this->VAO );
	{
	    glBindBuffer( GL_ARRAY_BUFFER, this->VBO );
	    glBufferData( GL_ARRAY_BUFFER, sizeof(Vertex) * this->vertices.size(), &this->vertices[0],
			  GL_STATIC_DRAW );

	    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, this->EBO );
	    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * this->indices.size(), &this->indices[0],
			  GL_STATIC_DRAW );

	    // Vertex Buffer Attribute Pointers
	    // position
	    glEnableVertexAttribArray( 0 );
	    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0 );
	    // normal
	    glEnableVertexAttribArray( 1 );
	    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal) );
	    // texture coordinates
	    glEnableVertexAttribArray( 2 );
	    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
				   (GLvoid*)offsetof(Vertex, texCoords) );
	}
	glBindVertexArray( 0 );

    }

};
