///////////////////////////////////////////////////
//
//  Hamish Carr
//  September, 2020
//
//  ------------------------
//  AttributedObject.cpp
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

// include the header file
#include "AttributedObject.h"

// include the C++ standard libraries we want
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

// include the Cartesian 3- vector class
#include "Cartesian3.h"

#define MAXIMUM_LINE_LENGTH 1024
#define REMAP_TO_UNIT_INTERVAL(x) (0.5 + (0.5*(x)))
#define REMAP_FROM_UNIT_INTERVAL(x) (-1.0 + (2.0*(x)))

// constructor will initialise to safe values
AttributedObject::AttributedObject()
    : centreOfGravity(0.0,0.0,0.0)
    { // AttributedObject()
    // force arrays to size 0
    vertices.resize(0);
    colours.resize(0);
    normals.resize(0);
    textureCoords.resize(0);
    faceVertices.resize(0);
    faceColours.resize(0);
    faceNormals.resize(0);
    faceTexCoords.resize(0);
    } // AttributedObject()

// read routine returns true on success, failure otherwise
bool AttributedObject::ReadObjectStream(std::istream &geometryStream)
    { // ReadObjectStream()
    
    // create a read buffer
    char readBuffer[MAXIMUM_LINE_LENGTH];
    
    // the rest of this is a loop reading lines & adding them in appropriate places
    while (true)
        { // not eof
        // character to read
        char firstChar = geometryStream.get();
        
//         std::cout << "Read: " << firstChar << std::endl;
        
        // check for eof() in case we've run out
        if (geometryStream.eof())
            break;

        // otherwise, switch on the character we read
        switch (firstChar)
            { // switch on first character
            case '#':       // comment line
                // read and discard the line
                geometryStream.getline(readBuffer, MAXIMUM_LINE_LENGTH);
                break;
                
            case 'v':       // vertex data of some type
                { // some sort of vertex data
                // retrieve another character
                char secondChar = geometryStream.get();
                
                // bail if we ran out of file
                if (geometryStream.eof())
                    break;

                // now use the second character to choose branch
                switch (secondChar)
                    { // switch on second character
                    case ' ':       // space - indicates a vertex
                          { // vertex read
                        Cartesian3 vertex;
                        geometryStream >> vertex;
                        vertices.push_back(vertex);
//                         std::cout << "Vertex " << vertex << std::endl;
                        break;
                        } // vertex read
                    case 'c':       // c indicates colour
                        { // normal read
                        Cartesian3 colour;
                        geometryStream >> colour;
                        colours.push_back(colour);
//                         std::cout << "Colour " << colour << std::endl;
                        break;
                        } // normal read
                    case 'n':       // n indicates normal vector
                        { // normal read
                        Cartesian3 normal;
                        geometryStream >> normal;
                        normals.push_back(normal);
//                         std::cout << "Normal " << normal << std::endl;
                        break;
                        } // normal read
                    case 't':       // t indicates texture coords
                        { // tex coord
                        Cartesian3 texCoord;
                        geometryStream >> texCoord;
                        textureCoords.push_back(texCoord);
//                         std::cout << "Tex Coords " << texCoord << std::endl;
                        break;                  
                        } // tex coord
                    default:
                        break;
                    } // switch on second character 
                break;
                } // some sort of vertex data
                
            case 'f':       // face data
                { // face
				// make a hard assumption that we have a single triangle per line
                unsigned int vertexID;
				unsigned int colourID;
                unsigned int normalID;
				unsigned int texCoordID;				
                
                // read in three vertices
				for (unsigned int vertex = 0; vertex < 3; vertex++)
					{ // per vertex
					// read a vertex ID
					geometryStream >> vertexID;
					// read and discard the slash
					geometryStream.get();
					// read a colour ID
					geometryStream >> colourID;
					// read and discard the slash
					geometryStream.get();
					// read a vertex ID
					geometryStream >> texCoordID;
					// read and discard the slash
					geometryStream.get();
					// read a vertex ID
					geometryStream >> normalID;

// 					std::cout << "Face " << vertexID << "/" << colourID << "/" << texCoordID << "/" << normalID << std::endl;

					// subtract one and store them (OBJ uses 1-based numbering)
					faceVertices.push_back(vertexID-1);
					faceColours.push_back(colourID-1);
					faceNormals.push_back(normalID-1);
					faceTexCoords.push_back(texCoordID-1);
					} // per vertex
				break;
                } // face
                
            // default processing: do nothing
            default:
                break;

            } // switch on first character

        } // not eof

    // compute centre of gravity
    // note that very large files may have numerical problems with this
    centreOfGravity = Cartesian3(0.0, 0.0, 0.0);

    // if there are any vertices at all
    if (vertices.size() != 0)
        { // non-empty vertex set
        // sum up all of the vertex positions
        for (unsigned int vertex = 0; vertex < vertices.size(); vertex++)
            centreOfGravity = centreOfGravity + vertices[vertex];
        
        // and divide through by the number to get the average position
        // also known as the barycentre
        centreOfGravity = centreOfGravity / vertices.size();

        // start with 0 radius
        objectSize = 0.0;

        // now compute the largest distance from the origin to a vertex
        for (unsigned int vertex = 0; vertex < vertices.size(); vertex++)
            { // per vertex
            // compute the distance from the barycentre
            float distance = (vertices[vertex] - centreOfGravity).length();         
            
            // now test for maximality
            if (distance > objectSize)
                objectSize = distance;
                
            } // per vertex
        } // non-empty vertex set

    // return a success code
    return true;
	} // ReadObjectStream()

// write routine
void AttributedObject::WriteObjectStream(std::ostream &geometryStream)
    { // WriteObjectStream()
    geometryStream << "# " << (faceVertices.size()/3) << " triangles" << std::endl;
    geometryStream << std::endl;

    // output the vertex coordinates
    geometryStream << "# " << vertices.size() << " vertices" << std::endl;
    for (unsigned int vertex = 0; vertex < vertices.size(); vertex++)
        geometryStream << "v  " << std::fixed << vertices[vertex] << std::endl;

    // output the vertex colours
    geometryStream << "# " << colours.size() << " vertex colours" << std::endl;
    for (unsigned int vertex = 0; vertex < colours.size(); vertex++)
        geometryStream << "vc " << std::fixed << colours[vertex] << std::endl;

    // output the vertex normals
    geometryStream << "# " << normals.size() << " vertex normals" << std::endl;
    for (unsigned int vertex = 0; vertex < normals.size(); vertex++)
        geometryStream << "vn " << std::fixed << normals[vertex] << std::endl;

    // output the vertex colours
    geometryStream << "# " << textureCoords.size() << " vertex tex coords" << std::endl;
    for (unsigned int vertex = 0; vertex < textureCoords.size(); vertex++)
        geometryStream << "vt " << std::fixed << textureCoords[vertex] << std::endl;

    // and the faces
    for (unsigned int face = 0; face < faceVertices.size(); face+=3)
        { // per face
        geometryStream << "f";
        
        // loop through # of vertices
        for (unsigned int vertex = 0; vertex < 3; vertex++)
			{ // per vertex
            geometryStream << " ";
            geometryStream << faceVertices[face+vertex] + 1 << "/";
            geometryStream << faceColours[face+vertex] + 1 << "/";
            geometryStream << faceTexCoords[face+vertex] + 1 << "/";
            geometryStream << faceNormals[face+vertex] + 1;
			} // per vertex
		// end the line
        geometryStream << std::endl;
        } // per face
    
    } // WriteObjectStream()

// routine to render
void AttributedObject::Render(RenderParameters *renderParameters)
    { // Render()
	// make sure that textures are disabled
	glDisable(GL_TEXTURE_2D);

    // Scale defaults to the zoom setting
    float scale = renderParameters->zoomScale;
    scale /= objectSize;
    glTranslatef(-centreOfGravity.x * scale, -centreOfGravity.y * scale, -centreOfGravity.z * scale);

    // start rendering
    glBegin(GL_TRIANGLES);

    // loop through the faces: note that they may not be triangles, which complicates life
    for (unsigned int face = 0; face < faceVertices.size(); face+=3)
        { // per face
		// now do a loop over three vertices
		for (unsigned int vertex = 0; vertex < 3; vertex++)
			{ // per vertex
			glColor3f
				(
				colours[faceVertices[face+vertex]].x,
				colours[faceVertices[face+vertex]].y,
				colours[faceVertices[face+vertex]].z
				);

			glVertex3f
				(
				scale * vertices[faceVertices[face+vertex]].x,
				scale * vertices[faceVertices[face+vertex]].y,
				scale * vertices[faceVertices[face+vertex]].z
				);
			} // per vertex
        } // per face

    // close off the triangles
    glEnd();
    } // Render()

