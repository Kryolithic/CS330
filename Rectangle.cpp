#include"Rectangle.h"

//constructor
Rectangle::Rectangle(float length, float width, float height)
{
	Rectangle::length = length;
	Rectangle::width = width;
	Rectangle::height = height;
}
//returns vector of Vertex type
std::vector<Vertex> Rectangle::getRectangleVerts()
{
	//holds x,y,z from l*w*h in respect to origin at center of prism to reduce computational complexity
	float x, y, z;
	Vertex newVert;
	float s = 0;
	float t = 0;
	float a = 0;

	x = length / 2.0f;
	y = height / 2.0f;
	z = width / 2.0f;

	std::vector<glm::vec3> rectVerts =
	{
		//front face verts (0) 
		glm::vec3(-x, -y, z), //1
		glm::vec3(x, -y, z),  //2
		glm::vec3(x, y, z),   //3
		glm::vec3(-x, y, z),  //4

		//top face verts (1)
		glm::vec3(-x, y, z),  //5
		glm::vec3(x, y, z),   //6
		glm::vec3(x, y, -z),  //7
		glm::vec3(-x, y, -z), //8

		//back face verts (2) 
		glm::vec3(x, -y, -z), //9
		glm::vec3(-x, -y, -z),//10
		glm::vec3(-x, y, -z), //11
		glm::vec3(x, y, -z),  //12

		//bot face verts (3)
		glm::vec3(-x, -y, -z),//13
		glm::vec3(x, -y, -z), //14
		glm::vec3(x, -y, z),  //15
		glm::vec3(-x, -y, z), //16

		//left face verts (4) 
		glm::vec3(-x, -y,-z), //17
		glm::vec3(-x, -y, z), //18
		glm::vec3(-x, y, z),  //19
		glm::vec3(-x, y, -z), //20

		//right face verts (5)
		glm::vec3(x, -y, z),  //21
		glm::vec3(x, -y, -z), //22
		glm::vec3(x, y, -z),  //23
		glm::vec3(x, y, z)    //24
	};

	std::vector<glm::vec3> rectNormals =
	{
		//front face verts (0) 
		glm::normalize(glm::vec3(-x, -y, z)), //1
		glm::normalize(glm::vec3(x, -y, z)),  //2
		glm::normalize(glm::vec3(x, y, z)),   //3
		glm::normalize(glm::vec3(-x, y, z)),  //4

		//top face verts (1)
		glm::normalize(glm::vec3(-x, y, z)),  //5
		glm::normalize(glm::vec3(x, y, z)),   //6
		glm::normalize(glm::vec3(x, y, -z)),  //7
		glm::normalize(glm::vec3(-x, y, -z)), //8

		//back face verts (2) 
		glm::normalize(glm::vec3(x, -y, -z)), //9
		glm::normalize(glm::vec3(-x, -y, -z)),//10
		glm::normalize(glm::vec3(-x, y, -z)), //11
		glm::normalize(glm::vec3(x, y, -z)),  //12

		//bot face verts (3)
		glm::normalize(glm::vec3(-x, -y, -z)),//13
		glm::normalize(glm::vec3(x, -y, -z)), //14
		glm::normalize(glm::vec3(x, -y, z)),  //15
		glm::normalize(glm::vec3(-x, -y, z)), //16

		//left face verts (4) 
		glm::normalize(glm::vec3(-x, -y,-z)), //17
		glm::normalize(glm::vec3(-x, -y, z)), //18
		glm::normalize(glm::vec3(-x, y, z)),  //19
		glm::normalize(glm::vec3(-x, y, -z)), //20

		//right face normals (5)
		glm::normalize(glm::vec3(x, -y, z)),  //21
		glm::normalize(glm::vec3(x, -y, -z)), //22
		glm::normalize(glm::vec3(x, y, -z)),  //23
		glm::normalize(glm::vec3(x, y, z))    //24
	};

	std::vector<glm::vec2> rectUVs =
	{
		//front
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),
		//top
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),
		//back
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),
		//bot
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),
		//left
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),
		//right
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f)
	};
	//loop to add vertex data to vector
	for (int i = 0; i < rectVerts.size(); ++i)
	{
		newVert = Vertex{ rectVerts[i], rectNormals[i], rectUVs[i]};
		rectVertData.push_back(newVert);
	}

	return rectVertData;
}
//returns vector of index coords
std::vector<unsigned int> Rectangle::getRectangleInd()
{
	//index positions
	std::vector<unsigned int> rectIndices = {
		
		//front face
		0, 1, 2,
		2, 3, 0,
		// top
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,
		// bottom
		12, 13, 14,
		14, 15, 12,
		// left
		16, 17, 18,
		18, 19, 16,
		// right
		20, 21, 22,
		22, 23, 20
	};
	return rectIndices;
}
