#ifndef SPHERE_H
#define SPHERE_H

#include"mesh.h"

using namespace std;

class Sphere
{
	const float PI = 3.1415926f; //pi constant
	float radius;
	int nStacks;
	int nSlices;                 //number of slices in sphere (higher number = more round)
	

	std::vector<glm::vec3> position;   //stores vector of x[i], y[i], z[i]
	std::vector<GLuint> sphereIndices; //stores index positions
	std::vector<Vertex> sphereVerts;   //stores vertex data


public:
	Sphere(float radius, int nStacks, int nSlices);
	std::vector<Vertex> getSphereVerts();
	std::vector<unsigned int> getSphereInd();
};


#endif