// Project
#include "cylinder.h"

//constructor
	Cylinder::Cylinder(float radius, int numSlices, float height, bool withPositions, bool withTextureCoordinates, bool withNormals)
	{
		this->_radius = radius;
		this->_numSlices = numSlices;
		this->_height = height;
		this->withPositions = withPositions;
		this->withTextures = withTextureCoordinates;
		this->withNormals = withNormals;

		initializeData();
	}

	//calculates cylinder vertex data
	void Cylinder::initializeData()
	{

		// Calculate and cache numbers of vertices
		_numVerticesSide = (_numSlices + 1) * 2;
		_numVerticesTopBottom = _numSlices + 2;
		_numVerticesTotal = _numVerticesSide + _numVerticesTopBottom * 2;

		// Pre-calculate sines / cosines for given number of slices
		const auto sliceAngleStep = 2.0f * glm::pi<float>() / float(_numSlices);
		auto currentSliceAngle = 0.0f;
		std::vector<float> sines, cosines;
		for (auto i = 0; i <= _numSlices; i++)
		{
			sines.push_back(sin(currentSliceAngle));
			cosines.push_back(cos(currentSliceAngle));

			// Update slice angle
			currentSliceAngle += sliceAngleStep;
		}
		//creates glm::vec3 data for x,y,z positons of vertices
		if (withPositions)
		{
			// Pre-calculate X and Z coordinates (generates unit circle on x and z plane)
			std::vector<float> x;
			std::vector<float> z;
			for (auto i = 0; i <= _numSlices; i++)
			{
				x.push_back(cosines[i] * _radius);
				z.push_back(sines[i] * _radius);
			}

			// Add cylinder side vertices (creates top and bottom vertices to draw tube for sides)
			for (auto i = 0; i <= _numSlices; i++)
			{
				const auto topPosition = glm::vec3(x[i], _height / 2.0f, z[i]); //glm::vec3 for top positions (takes x and z positions and adds height/2 y position)
				const auto bottomPosition = glm::vec3(x[i], -_height / 2.0f, z[i]); //glm::vec3 for bot positions
				position.push_back(topPosition);
				position.push_back(bottomPosition);
				
			}

			// Add top cylinder cover (creates vertex data for circle at height/2)
			glm::vec3 topCenterPosition(0.0f, _height / 2.0f, 0.0f); //glm::vec3 for top center vertex
			position.push_back(topCenterPosition);
			
			for (auto i = 0; i <= _numSlices; i++)
			{
				const auto topPosition = glm::vec3(x[i], _height / 2.0f, z[i]);
				position.push_back(topPosition);
				
			}

			// Add bottom cylinder cover (creates vertex data to draw circle at -height/2
			glm::vec3 bottomCenterPosition(0.0f, -_height / 2.0f, 0.0f); //glm::vec3 for bottom center vertex
			position.push_back(bottomCenterPosition);
			
			for (auto i = 0; i <= _numSlices; i++)
			{
				const auto bottomPosition = glm::vec3(x[i], -_height / 2.0f, -z[i]);
				position.push_back(bottomPosition);
				
			}
		}
		//creates glm::vec2 for tex coords
		if (withTextures)
		{
			// Pre-calculate step size in texture coordinate U
			const auto sliceTextureStepU = 2.0f / float(_numSlices); //wraps texture around cyl twice
			//TEXTURE UV FOR SIDE OF CYLINDER
			auto currentSliceTexCoordU = 0.0f;
			for (auto i = 0; i <= _numSlices; i++)
			{
				texCoords.push_back(glm::vec2(currentSliceTexCoordU, 1.0f)); //generates s texture coordinate
				texCoords.push_back(glm::vec2(currentSliceTexCoordU, 0.0f)); //generates t texture coordinate
				

				// Update texture coordinate of current slice 
				currentSliceTexCoordU += sliceTextureStepU;
			}

			// Generate circle texture coordinates for cylinder top cover
			glm::vec2 topBottomCenterTexCoord(0.5f, 0.5f);
			texCoords.push_back(topBottomCenterTexCoord);
			
			for (auto i = 0; i <= _numSlices; i++) {
				texCoords.push_back(glm::vec2(topBottomCenterTexCoord.x + sines[i] * 0.5f, topBottomCenterTexCoord.y + cosines[i] * 0.5f));
				
			}

			// Generate circle texture coordinates for cylinder bottom cover
			texCoords.push_back(topBottomCenterTexCoord);
			
			for (auto i = 0; i <= _numSlices; i++) {
				texCoords.push_back(glm::vec2(topBottomCenterTexCoord.x + sines[i] * 0.5f, topBottomCenterTexCoord.y - cosines[i] * 0.5f));
				
			}
		}
		//creates glm::vec3 for normals
		if (withNormals)
		{
			//creates normals for side of cylinder
			for (auto i = 0; i < _numSlices; i++) {
				normal.push_back(glm::vec3(cosines[i], 0.0f, sines[i]));
				
			}

			//creates normals for side of cylinder
			for (auto i = 0; i < _numSlices; i++) {
				normal.push_back(glm::vec3(cosines[i], 0.0f, sines[i]));
				
			}

			// Add normal for every vertex of cylinder top cover
			for (int i = 0; i <= _numVerticesTopBottom; ++i)
			{
				normal.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
			}
			
			

			// Add normal for every vertex of cylinder bottom cover
			for (int i = 0; i <= _numVerticesTopBottom; ++i)
			{
				normal.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
			}
			
			
		}
		//adds vertex data to vector
		Vertex newVert;
		for (int i = 0; i < _numVerticesTotal; ++i)
		{
			newVert = Vertex{ position[i], normal[i], texCoords[i] };
			cylinderVertex.push_back(newVert);
		}
		return;
	}

	//getters for cylinder vertex totals
	int Cylinder::getVertSide()
	{
		return _numVerticesSide;
	}
	int Cylinder::getVertTB()
	{
		return _numVerticesTopBottom;
	}
	int Cylinder::getVertTotal()
	{
		return _numVerticesTotal;
	}
	std::vector<Vertex> Cylinder::getVertex()
	{
		return cylinderVertex;
	}

