///////////////////////////////////////////////////
//
//  Hamish Carr
//  September, 2020
//
//  ------------------------
//  AttributedObject.h
//  ------------------------
//  
//  Base code for rendering assignments.
//
//  Minimalist (non-optimised) code for reading and 
//  rendering an object file
//  
//  We will make some hard assumptions about input file
//  quality. We will not check for manifoldness or 
//  normal direction, &c.  And if it doesn't work on 
//  all object files, that's fine.
//
//	Variant on TexturedObject that stores explicit RGB
//	values for each vertex
//  
///////////////////////////////////////////////////

// include guard for AttributedObject
#ifndef _ATTRIBUTED_OBJECT_H
#define _ATTRIBUTED_OBJECT_H

// include the C++ standard libraries we need for the header
#include <vector>
#include <iostream>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

// include the unit with Cartesian 3-vectors
#include "Cartesian3.h"
// the render parameters
#include "RenderParameters.h"
#include <string>

//store two file names for returning two values in a function
typedef struct ResultFileNames{
    std::string textureName;
    std::string normalMapName;
}ResultFileNames;

// define a macro for "not used" flag
//#define NO_SUCH_ELEMENT -1

// use macros for the "previous" and "next" IDs
//#define PREVIOUS_EDGE(x) ((x) % 3) ? ((x) - 1) : ((x) + 2)
//#define NEXT_EDGE(x) (((x) % 3) == 2) ? ((x) - 2) : ((x) + 1)

class AttributedObject
    { // class AttributedObject
    public:
    char *filename; // store the .face filename,
    // vector of vertices
    std::vector<Cartesian3> vertices;

    // vector of colours stored as cartesian triples in float
    std::vector<Cartesian3> colours;
    
    // vector of normals
    std::vector<Cartesian3> normals;
    
    // vector of texture coordinates (stored as triple to simplify code)
    std::vector<Cartesian3> textureCoords;

    // vector of faces
    std::vector<unsigned int> faceVertices;

    // vector of colour IDs
    std::vector<unsigned int> faceColours;

    // corresponding vector of normals
    std::vector<unsigned int> faceNormals;
    
    // corresponding vector of texture coordinates
    std::vector<unsigned int> faceTexCoords;

    // centre of gravity - computed after reading
    Cartesian3 centreOfGravity;

    // size of object - i.e. radius of circumscribing sphere centred at centre of gravity
    float objectSize;

    // constructor will initialise to safe values
    AttributedObject();
   
    // read routine returns true on success, failure otherwise
    bool ReadObjectStream(std::istream &geometryStream);

    // write routine
    void WriteObjectStream(std::ostream &geometryStream);
    // generate texture and normal map
    void generateTextureAndNormalMap();
    // generate texture file name and normal map file name
    ResultFileNames generateNewFileName();
    // routine to render
    void Render(RenderParameters *renderParameters);
    }; // class AttributedObject

// end of include guard for AttributedObject
#endif
