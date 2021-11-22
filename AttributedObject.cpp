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
#include "string.h"
#include <deque>
#include <fstream>
#include <algorithm>
#include "Cartesian3.h"
#include <unistd.h>
#include <sys/stat.h>
#include <sstream>
#include <stdlib.h>


#define MAXIMUM_LINE_LENGTH 1024
#define REMAP_TO_UNIT_INTERVAL(x) (0.5 + (0.5*(x)))
#define REMAP_FROM_UNIT_INTERVAL(x) (-1.0 + (2.0*(x)))
using namespace std;
const int MAP_SIZE = 1024;




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
/**!
 * generate texture and normal map, all codes are in the function
 *
 */
void AttributedObject::generateTextureAndNormalMap() {
    //MAP_SIZE is 1024, store it in a variable is good, as we can change it without modifying other codes if we need to.
    unsigned int numFaces = faceVertices.size(); //get the number of triangle faces
    Cartesian3 (*textureMap)[MAP_SIZE] = new Cartesian3[MAP_SIZE][MAP_SIZE]; //create a 2 D array on heap memory, storing texture color data. As 1024 * 1024 is two big.
    Cartesian3 (*normalMap)[MAP_SIZE] = new Cartesian3[MAP_SIZE][MAP_SIZE]; //create a 2 D array on heap memory, storing normal color data. As 1024 * 1024 is two big.

    for (unsigned int face = 0; face < numFaces; face+=3){
        std::deque<Cartesian3> triangleTexture = std::deque<Cartesian3>(); //store texture u v for every triangle's vertexes
        std::deque<Cartesian3> triangleNormal = std::deque<Cartesian3>(); //store normal coordinates for every triangle's vertexes
        std::deque<Cartesian3> triangleColour = std::deque<Cartesian3>(); //store colours RGB for every triangle's vertexes
        for (unsigned int vertex = 0; vertex < 3; vertex++){
            int indexTextureCoordinate = faceTexCoords[face + vertex]; //get the index of texture coordinates of a vertex
            //as uv in a texture map is inverse to uv in a model, so we exchange u v here, then we can use them later directly.
            Cartesian3 currentTexCoords = textureCoords[indexTextureCoordinate];
            Cartesian3 trueTexCoords = Cartesian3(currentTexCoords.y, currentTexCoords.x, currentTexCoords.z);
            triangleTexture.push_back(trueTexCoords);
            int indexNormal = faceNormals[face + vertex];
            triangleNormal.push_back(normals[indexNormal]);
            int indexColour = faceColours[face + vertex];
            triangleColour.push_back(colours[indexColour]);
        }
        //project uv to 1024 * 1024 coordinate space.
        Cartesian3 textureCoordTransformed0 = triangleTexture.front();
        textureCoordTransformed0.x = textureCoordTransformed0.x * MAP_SIZE;
        textureCoordTransformed0.y = textureCoordTransformed0.y * MAP_SIZE;
        triangleTexture.pop_front();
        Cartesian3 textureCoordTransformed1 = triangleTexture.front();
        textureCoordTransformed1.x = textureCoordTransformed1.x * MAP_SIZE;
        textureCoordTransformed1.y = textureCoordTransformed1.y * MAP_SIZE;
        triangleTexture.pop_front();
        Cartesian3 textureCoordTransformed2 = triangleTexture.front();
        textureCoordTransformed2.x = textureCoordTransformed2.x * MAP_SIZE;
        textureCoordTransformed2.y = textureCoordTransformed2.y * MAP_SIZE;
        triangleTexture.pop_front();
        // calculate the distance for a vertex to the opposing side, just use the code from 5812 cw1
        float minX = MAP_SIZE, maxX = 0.0;
        float minY = MAP_SIZE, maxY = 0.0;

        // test against all vertices
        if (textureCoordTransformed0.x < minX) minX = textureCoordTransformed0.x;
        if (textureCoordTransformed0.x > maxX) maxX = textureCoordTransformed0.x;
        if (textureCoordTransformed0.y < minY) minY = textureCoordTransformed0.y;
        if (textureCoordTransformed0.y > maxY) maxY = textureCoordTransformed0.y;

        if (textureCoordTransformed1.x < minX) minX = textureCoordTransformed1.x;
        if (textureCoordTransformed1.x > maxX) maxX = textureCoordTransformed1.x;
        if (textureCoordTransformed1.y < minY) minY = textureCoordTransformed1.y;
        if (textureCoordTransformed1.y > maxY) maxY = textureCoordTransformed1.y;

        if (textureCoordTransformed2.x < minX) minX = textureCoordTransformed2.x;
        if (textureCoordTransformed2.x > maxX) maxX = textureCoordTransformed2.x;
        if (textureCoordTransformed2.y < minY) minY = textureCoordTransformed2.y;
        if (textureCoordTransformed2.y > maxY) maxY = textureCoordTransformed2.y;

        // now for each side of the triangle, compute the line vectors
        Cartesian3 vector01 = textureCoordTransformed1 - textureCoordTransformed0;
        Cartesian3 vector12 = textureCoordTransformed2 - textureCoordTransformed1;
        Cartesian3 vector20 = textureCoordTransformed0 - textureCoordTransformed2;

        // now compute the line normal vectors
        Cartesian3 normal01(-vector01.y, vector01.x, 0.0);
        Cartesian3 normal12(-vector12.y, vector12.x, 0.0);
        Cartesian3 normal20(-vector20.y, vector20.x, 0.0);

        // we don't need to normalise them, because the square roots will cancel out in the barycentric coordinates
        float lineConstant01 = normal01.dot(textureCoordTransformed0);
        float lineConstant12 = normal12.dot(textureCoordTransformed1);
        float lineConstant20 = normal20.dot(textureCoordTransformed2);

        // and compute the distance of each vertex from the opposing side
        float distance0 = normal12.dot(textureCoordTransformed0) - lineConstant12;
        float distance1 = normal20.dot(textureCoordTransformed1) - lineConstant20;
        float distance2 = normal01.dot(textureCoordTransformed2) - lineConstant01;
        if ((distance0 == 0) || (distance1 == 0) || (distance2 == 0))
            continue;

        Cartesian3 vertexNormal0 = triangleNormal.front();
        triangleNormal.pop_front();
        Cartesian3 vertexNormal1 = triangleNormal.front();
        triangleNormal.pop_front();
        Cartesian3 vertexNormal2 = triangleNormal.front();
        triangleNormal.pop_front();

        Cartesian3 normalProject = Cartesian3(0.5,0.5,0.5);
        //project normal vector to 0-1
        Cartesian3 normal0 = vertexNormal0.unit() * 0.5 + normalProject;
        Cartesian3 normal1 = vertexNormal1.unit() * 0.5 + normalProject;
        Cartesian3 normal2 = vertexNormal2.unit() * 0.5 + normalProject;

        Cartesian3 colour0 = triangleColour.front();
        triangleColour.pop_front();
        Cartesian3 colour1 = triangleColour.front();
        triangleColour.pop_front();
        Cartesian3 colour2 = triangleColour.front();
        triangleColour.pop_front();
        //do barycentric coordinate interpolation for texture map and normal map
        for (int i = minY; i <= maxY; ++i) {
            if (i >= MAP_SIZE || i<0) continue;

            for (int j = minX; j <= maxX; ++j) {
                if (j >= MAP_SIZE || j<0) continue;

                Cartesian3 pixel(j, i, 0.0);
                //calculate α β γ
                float alpha = (normal12.dot(pixel) - lineConstant12) / distance0;
                float beta = (normal20.dot(pixel) - lineConstant20) / distance1;
                float gamma = (normal01.dot(pixel) - lineConstant01) / distance2;

                // now perform the half-plane test
                if ((alpha < 0.0) || (beta < 0.0) || (gamma < 0.0))
                    continue;

                float red = alpha * colour0.x + beta * colour1.x + gamma * colour2.x;
                float green = alpha * colour0.y + beta * colour1.y + gamma * colour2.y;
                float blue = alpha * colour0.z + beta * colour1.z + gamma * colour2.z;
                textureMap[j][i] = Cartesian3(red, green, blue); //store the texture colour in textureMap

                float redNormal = alpha * normal0.x + beta * normal1.x + gamma * normal2.x;
                float greenNormal = alpha * normal0.y + beta * normal1.y + gamma * normal2.y;
                float blueNormal = alpha * normal0.z + beta * normal1.z + gamma * normal2.z;
                normalMap[j][i] = Cartesian3(redNormal, greenNormal, blueNormal); //store the texture colour in normalMap
            }
        }
    }
    //get the new file name
    ResultFileNames resultFileNames = this->generateNewFileName();

    std::ofstream fileTextureMap;
    fileTextureMap.open(resultFileNames.textureName);
    fileTextureMap << "P3\n " << MAP_SIZE << " " << MAP_SIZE << " " << "\n255" << std::endl;


    std::ofstream fileNormalMap;
    fileNormalMap.open(resultFileNames.normalMapName);
    fileNormalMap << "P3\n " << MAP_SIZE << " " << MAP_SIZE << " " << "\n255" << std::endl;

    //write ppm files
    for (int i = 0; i <MAP_SIZE; ++i) {
        for (int j = 0; j <MAP_SIZE; ++j) {
            unsigned int r = textureMap[i][j].x * 255;
            r = max<unsigned int>(r,0);
            r = min<unsigned int>(r,255);
            unsigned int g = textureMap[i][j].y * 255;
            g = max<unsigned int>(g,0);
            g = min<unsigned int>(g,255);
            unsigned int b = textureMap[i][j].z * 255;
            b = max<unsigned int>(b,0);
            b = min<unsigned int>(b,255);
            fileTextureMap << r << ' ' << g << ' ' << b << '\n';

            unsigned int rN = normalMap[i][j].x * 255;
            rN = max<unsigned int>(rN,0);
            rN = min<unsigned int>(rN,255);
            unsigned int gN = normalMap[i][j].y * 255;
            gN = max<unsigned int>(gN,0);
            gN = min<unsigned int>(gN,255);
            unsigned int bN = normalMap[i][j].z * 255;
            bN = max<unsigned int>(bN,0);
            bN = min<unsigned int>(bN,255);
            fileNormalMap << rN << ' ' << gN << ' ' << bN << '\n';

        }

    }
    fileTextureMap.close();
    fileNormalMap.close();
    //we need delete the memory when we use heap memory
    delete[] textureMap;
    delete[] normalMap;

    if (access(resultFileNames.textureName.c_str(),0) != -1){
        cout<<"successfully generate texture ppm."<<endl;
    }
    if (access(resultFileNames.normalMapName.c_str(),0) != -1){
        cout<<"successfully generate normal map ppm."<<endl;
    }
}

/*!
 * generate the new file name for texture.ppm and normalMap.ppm file
 * @return ResultFileNames, a struct
 */
ResultFileNames AttributedObject::generateNewFileName()
{
    ResultFileNames result;
    string newTextureFileDir = strdup("./textureMaps/");
    string newNormalMapFileDir = strdup("./normalMaps/");
    string newFileSuffix = strdup(".ppm");
    string newTextureFileName = string();
    string newNormalMapFileName = string();
    string fileFullPath = this->filename;
    string::size_type targetPosition = fileFullPath.find_last_of("/") + 1;
    string file = fileFullPath.substr(targetPosition, fileFullPath.length() - targetPosition);

    string objName = file.substr(0, file.rfind("."));

    if (access(newTextureFileDir.c_str(), 0) == -1)	{
        mkdir(newTextureFileDir.c_str(), 0777);
    }

    if (access(newNormalMapFileDir.c_str(), 0) == -1)	{
        mkdir(newNormalMapFileDir.c_str(), 0777);
    }

    newTextureFileName = newTextureFileDir + objName + "_texture" + newFileSuffix;
    newNormalMapFileName = newNormalMapFileDir + objName + "_normal" + newFileSuffix;
    result.textureName = newTextureFileName;
    result.normalMapName = newNormalMapFileName;

    return result;
}

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

