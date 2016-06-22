#ifndef FILEUTILS_H_
#define FILEUTILS_H_

#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Material.h"

std::string  read_file( const GLchar* filePath )
{
    // Old school C way to read file is much faster than c++ ifstream
    FILE *file = NULL;
    file = fopen( filePath, "rt" ); // rt means translated mode?(for text on different platforms?)
    if( file == NULL )
    {
	std::cout << "Failed read file " << filePath << std::endl;
	return NULL;
    }
	
    fseek( file, 0, SEEK_END );
    unsigned long length = ftell( file );

    // Allocate memory
    GLchar* data = new GLchar[length + 1]; // plus 1 for termination GLcharacter('\0')
    memset( data, 0, length + 1 );
    fseek( file, 0, SEEK_SET );

    // Read file to data
    fread( data, // pointer to memory
	   1,   // size in bytes of each element
	   length, // number of elements
	   file ); // stream
    
    fclose( file );

    data[ length + 1 ] = '\0';

    std::string result( data );
    delete[] data;

    return result;
}

Material* load_material( const char* filePath )
{
    // Old school C way to read file is much faster than c++ ifstream
    FILE *file = NULL;
    file = fopen( filePath, "rt" ); // rt means translated mode?(for text on different platforms?)
    if( file == NULL )
    {
	std::cout << "Failed read file " << filePath << std::endl;
	return NULL;
    }


    float a,b,c,d,e,f,g,h,i,j;
    
    fscanf( file, "%f %f %f %f %f %f %f %f %f %f\n", &a, &b, &c, &d, &e, &f, &g, &h, &i, &j );
    glm::vec3 ambient = glm::vec3( a, b, c );
    glm::vec3 diffuse = glm::vec3( d, e, f );
    glm::vec3 specular = glm::vec3( g, h, i );
    float shininess = j;

    std::cout << "ambient: " << ambient.x << "," << ambient.y << "," << ambient.z << std::endl;
    std::cout << "diffuse: " << diffuse.x << "," << diffuse.y << "," << diffuse.z << std::endl;
    std::cout << "specular: " << specular.x << "," << specular.y << "," << specular.z << std::endl;
    std::cout << "shininess: " << shininess;

    Material* material = new Material( ambient, diffuse, specular, j );

    
    return material;
}
#endif
