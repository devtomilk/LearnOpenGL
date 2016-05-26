#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <iostream>

std::string read_file( const char* filepath )
{
    // Old school C way  to read files is much faster than c++ way(ifstream)

    // Open file
    FILE* file = fopen( filepath, "r" );
    fseek( file, 0, SEEK_END );
    unsigned long length = ftell( file );

    // Allocate memory
    char* data = new char[length + 1]; // plus 1 for termination character(\0) when we want to use it as a normal string
    memset( data, 0, length + 1 );
    fseek( file, 0, SEEK_SET );

    // Read file to data
    fread( data, 1, length, file );

    // Close file
    fclose( file );

    data[length + 1] = '\0';
    std::string result( data );
    delete[] data;
    return result;
}

#endif
