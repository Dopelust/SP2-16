/******************************************************************************/
/*!
\file	MeshBuilder.h
\author Ricsson Toh
\brief 
Class to generate meshes
*/
/******************************************************************************/

#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "Mesh.h"
#include "vertex.h"
#include "LoadOBJ.h"

/******************************************************************************/
/*!
		Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{
public:
	static Mesh* GenerateAxes(const std::string &meshName);
	static Mesh* GenerateLine(const std::string &meshName, Color color, float length);
	static Mesh* GenerateQuad(const std::string &meshName, Color color, float lengthX, float lengthZ, int repeat);
	static Mesh* GenerateXYQuad(const std::string &meshName, Color color, float lengthX, float lengthY, int repeat);
	static Mesh* GenerateCubeOnPlane(const std::string &meshName, Color color, float lengthX, float lengthY, float lengthZ, int repeat);
	static Mesh* GenerateCube(const std::string &meshName, Color color, float lengthX, float lengthY, float lengthZ, int repeat);
	static Mesh* GenerateCircle(const std::string &meshName, Color color, float radius);
	static Mesh* GenerateRing(const std::string &meshName, Color color, float radius);
	static Mesh* GenerateSphere(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius);
	static Mesh* GenerateHemi(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius);
	static Mesh* GenerateCone(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius);
	static Mesh* GenerateCylinder(const std::string &meshName, Color color, unsigned numSlice, float height, float radius);
	static Mesh* GenerateCylinder(const std::string &meshName, Color colorBot, Color colorTop, unsigned numSlice, float height, float radius);
	static Mesh* GenerateCapsule(const std::string &meshName, Color color,unsigned numStack, unsigned numSlice, float height, float radius);
	static Mesh* GenerateTorus(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float outerR, float innerR);

	static Mesh* GenerateText(const std::string &meshName, unsigned numRow, unsigned numCol);
	static Mesh* GenerateOBJ(const std::string &meshName, const std::string &file_path);
};

#endif