#pragma once

#include"mesh.h"

	class Cylinder
	{
		// position
		std::vector<glm::vec3> position;
		// normal
		std::vector<glm::vec3> normal;
		// texCoords
		std::vector<glm::vec2> texCoords;
		//holds cylinder vertex data
		std::vector<Vertex> cylinderVertex;

	public:
		Cylinder(float radius, int numSlices, float height, bool withPositions, bool withTextureCoordinates, bool withNormals);

		std::vector<Vertex> getVertex();

		int getVertSide();
		int getVertTB();
		int getVertTotal();
		bool withPositions, withTextures, withNormals;

	private:
		float _radius;      // Cylinder radius (distance from the center of cylinder to surface)
		int _numSlices;     // Number of cylinder slices
		float _height;      // Height of the cylinder

		int _numVerticesSide;      // number of side vertices to render
		int _numVerticesTopBottom; // number of vertices to render on top and bottom
		int _numVerticesTotal;     // total number of vertices in mesh

		void initializeData();
	};

