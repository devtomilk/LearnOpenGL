#pragma once
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

// OpenGL Includes
#include <GL/glew.h> // Contains all the necessary OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL/SOIL.h>
#include <assimp/importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
GLint loadTextureFromFile( const char* path, string directory );

class Model
{
 public:
    //====================================================================================================
    // Constructor
    //====================================================================================================
    Model( GLchar* path )
    {
	this->loadModel( path );
    }
    //====================================================================================================
    // Draw
    //====================================================================================================
    void draw( Shader* shader )
    {
	for( GLuint i = 0; i < this->meshes.size(); i++ )
	{
	    this->meshes[i].draw( shader );
	}
    }

    void setScale( GLfloat scale )
    {
	for( GLuint i = 0; i < this->meshes.size(); i++ )
	{
	    this->meshes[i].setScale( scale );
	}
    }

 private:
    vector<Mesh> meshes;
    string directory;
    vector<Texture> textures_loaded;


    //====================================================================================================
    // Load Model
    //====================================================================================================
    void loadModel( string path )
    {
	// Read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile( path, aiProcess_Triangulate | aiProcess_FlipUVs );
	// Check for errors
	if( !scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode )
	{
	    cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
	    return;
	}

	// Retrieve the directory path part of the filepath
	this->directory = path.substr( 0, path.find_last_of('/') );

	// Process Nodes recursively
	this->processNode( scene->mRootNode, scene );

    }

    //----------------------------------------------------------------------------------------------------
    // Process Node
    //----------------------------------------------------------------------------------------------------
    void processNode( aiNode* node, const aiScene* scene )
    {
	// Process meshes of this node
	for( GLuint i = 0; i < node->mNumMeshes; i++ )
	{
	    // node only contains indices to index the actual mesh in aiScene object
	    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
	    this->meshes.push_back( this->processMesh( mesh, scene ));


	}

	// Recursive
	for( GLuint i = 0; i < node->mNumChildren; i++ )
	{
	    this->processNode( node->mChildren[i], scene );
	}
    }

    //----------------------------------------------------------------------------------------------------
    // Process Mesh
    //----------------------------------------------------------------------------------------------------
    Mesh processMesh( aiMesh* mesh, const aiScene* scene )
    {
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	// Walk through each of the mesh's vertices
	//----------------------------------------------------------------------------------------------------
	// Retrieve vertices
	//----------------------------------------------------------------------------------------------------
	for( GLuint i = 0; i < mesh->mNumVertices; i++ )
	{
	    Vertex vertex;
	    glm::vec3 vector;

	    // Positions
	    vector.x = mesh->mVertices[i].x;
	    vector.y = mesh->mVertices[i].y;
	    vector.z = mesh->mVertices[i].z;

	    vertex.position = vector;

	    // Normals
	    vector.x = mesh->mNormals[i].x;
	    vector.y = mesh->mNormals[i].y;
	    vector.z = mesh->mNormals[i].z;

	    vertex.normal = vector;

	    // Texture coordinates
	    if( mesh->mTextureCoords[0] )
	    {
		glm::vec2 vec;

		// A vertex can contain up to 8 different texture coordinates
		vec.x = mesh->mTextureCoords[0][i].x;
		vec.y = mesh->mTextureCoords[0][i].y;

		vertex.texCoords = vec;
	    }
	    else
		vertex.texCoords = glm::vec2( 0.0f, 0.0f );

	    vertices.push_back( vertex );
	}
	//----------------------------------------------------------------------------------------------------
	// Retrieve indices
	//----------------------------------------------------------------------------------------------------
	for( GLuint i = 0; i < mesh->mNumFaces; i++ )
	{
	    aiFace face = mesh->mFaces[i];

	    for( GLuint j = 0; j < face.mNumIndices; j++ )
		indices.push_back( face.mIndices[j] );
	}

	//----------------------------------------------------------------------------------------------------
	// Retrieve material textures
	//----------------------------------------------------------------------------------------------------
	if( mesh->mMaterialIndex >= 0 )
	{
	    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	    // Diffuse
	    vector<Texture> diffuseMaps = this->loadMaterialTextures( material, aiTextureType_DIFFUSE,
								      "texture_diffuse" );
	    textures.insert( textures.end(), diffuseMaps.begin(), diffuseMaps.end() );
	    // Specular
	    vector<Texture> specularMaps = this->loadMaterialTextures( material, aiTextureType_SPECULAR,
								       "texture_specular" );
	    textures.insert( textures.end(), specularMaps.begin(), specularMaps.end() );
	}

	return Mesh( vertices, indices, textures );

    }


    //****************************************************************************************************
    // Load Material Textues
    //****************************************************************************************************
    vector<Texture> loadMaterialTextures( aiMaterial* mat, aiTextureType type, string typeName )
    {
	vector<Texture> textures;
	for( GLuint i = 0; i < mat->GetTextureCount( type ); i++ )
	{
	    aiString texFilePath;
	    mat->GetTexture( type, i, &texFilePath );

	    // Check if texture was loaded before and if so, skip loading it
	    GLboolean skip = false;
	    for( GLuint j = 0; j < textures_loaded.size(); j++ )
	    {
		if( textures_loaded[j].path == texFilePath )
		{
		    textures.push_back( textures_loaded[j] );
		    skip = true;
		    break;
		}
		
	    }
	    // If the texture hasn't already been loaded, then load it
	    if( !skip )
	    {
		Texture texture;
		texture.id = loadTextureFromFile( texFilePath.C_Str(), this->directory );
		texture.type = typeName;
		texture.path = texFilePath;

		textures.push_back( texture );

		this->textures_loaded.push_back( texture );

	    }
	}

	return textures;
    }


    
};


//****************************************************************************************************
// Load Texture From File
//****************************************************************************************************
GLint loadTextureFromFile( const char* path, string directory )
{
    string filename = string( path );
    filename = directory + '/' + filename;
    GLuint textureID;
    glGenTextures( 1, &textureID );
    int width, height;

    unsigned char* image = SOIL_load_image( filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB );

    glBindTexture( GL_TEXTURE_2D, textureID );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );

    // Texture Parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glBindTexture( GL_TEXTURE_2D, 0 );

    SOIL_free_image_data( image );

    return textureID;
}


