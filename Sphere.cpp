#include"Sphere.h"

//constructor
Sphere::Sphere(float radius, int nStacks, int nSlices)
{
	Sphere::radius = radius;
	Sphere::nStacks = nStacks;
	Sphere::nSlices = nSlices;

}
//returns vector of Vertex type
std::vector<Vertex> Sphere::getSphereVerts()
{
	float x, y, z, xy;
	float sectorStep = 2 * PI / nSlices;
	float stackStep = PI / nStacks;
	float sectorAngle, stackAngle;
	float s = 0;
	float t = 0;
	int a = 0;
	Vertex newVert;

	//generates x,y,z vertices coord
	for (int i = 0; i <= nStacks; ++i)
	{
		stackAngle = PI / 2 - i * stackStep;
		xy = radius * cosf(stackAngle);
		z = radius * sinf(stackAngle);

		for (int j = 0; j <= nSlices; ++j)
		{
			sectorAngle = j * sectorStep;

			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);
			position.push_back(glm::vec3(x, y, z));
			
		}
	}
	//creates vector of Vertex data for each vtx
	for (int i = 0; i < position.size(); ++i)
	{
		//calculates UV value from 0,0 -> 1,1
		if (i < position.size() / 2)
		{
			s += (2 * i / position.size());
		}
		else
		{
			t += (2 * a / position.size());
			++a;
		}
		newVert = Vertex{ position[i], glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(s, t) };
		sphereVerts.push_back(newVert);
	}

	return sphereVerts;
}
//generates vector of index coords
std::vector<unsigned int> Sphere::getSphereInd()
{
	std::vector<unsigned int> sphereIndicesVec;
	int k1, k2;
	//GLuint index function
	for (int i = 0; i < nStacks; ++i)
	{
		k1 = i * (nSlices + 1);     // beginning of current stack
		k2 = k1 + nSlices + 1;

		for (int j = 0; j < nSlices; ++j, ++k1, ++k2)
		{
			if (i != 0)
			{
				sphereIndicesVec.push_back(k1);
				sphereIndicesVec.push_back(k2);
				sphereIndicesVec.push_back(k1 + 1);
			}

			if (i != (nStacks - 1))
			{
				sphereIndicesVec.push_back(k1 + 1);
				sphereIndicesVec.push_back(k2);
				sphereIndicesVec.push_back(k2 + 1);
			}
		}
	}
	
	return sphereIndicesVec;
}
