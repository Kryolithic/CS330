#ifndef RECTANGLE_H
#define RECTANGLE_H

#include"mesh.h"

using namespace std;

class Rectangle
{
	//field attributes
	float length, width, height;
	const int numTriangles = 12;
	

	public:
		std::vector<Vertex> rectVertData; //stores vertex data
		Rectangle(float length, float width, float height);
		std::vector<Vertex> getRectangleVerts();
		std::vector<unsigned int> getRectangleInd();
};


#endif
