#ifndef LIGHTING_H
#define LIGHTING_H

#include"camera.h"
#include"shader.h"
#include"mesh.h"


class Lighting
{
   //Vertex attributes for drawing light mesh
	// position
	std::vector<glm::vec3> position;
	// normal
	std::vector<glm::vec3> normal;
	// texCoords
	std::vector<glm::vec2> texCoords;
    //holds lightbox vertex data
	std::vector<Vertex> lightVertex;

public:

	void Direct(Shader shader)
	{
        //setting directional light uniforms
		shader.setVec3("directLight.direction", 0.0f, 0.0f, 0.0f);
		shader.setVec3("directLight.ambient", 0.3f, 0.3f, 0.3f);
		shader.setVec3("directLight.diffuse", 0.972f, 0.941f, 0.866f); //color value for warmer directional light
		shader.setVec3("directLight.specular", 0.3f, 0.3f, 0.3f);
	}

	void Point(Shader shader)
	{
        //setting point light uniforms
        shader.setVec3("pointLight.position", -10.0f, 5.0f, 5.0f);
        shader.setVec3("pointLight.ambient", 0.5f, 0.5f, 0.5f);
        shader.setVec3("pointLight.diffuse", 1.0f, 1.0f, 1.0f);
        shader.setVec3("pointLight.specular", 0.9f, 0.9f, 0.9f);
        shader.setFloat("pointLight.constant", 1.0f);
        shader.setFloat("pointLight.linear", 0.04f);
        shader.setFloat("pointLight.quadratic", 0.007f);
        shader.setFloat("pointLight.ambientStr", 1.0f); //scalar for ambient strength
	}
    
	void Spot(Shader shader)
	{
        //setting spotlight uniforms
        shader.setVec3("spotLight.position", 3.0f, 1.0f, 3.0f);
        shader.setVec3("spotLight.direction", -0.1f, -0.1f, -0.6f);
        shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
        shader.setFloat("spotLight.constant", 1.0f);
        shader.setFloat("spotLight.linear", 0.09f);
        shader.setFloat("spotLight.quadratic", 0.032f);
	}
    //creates a lightbox mesh
	std::vector<Vertex> Create()
	{
        float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
        };

        Vertex newVert;
        //transforming array values to vec3, vec3, vec2 for Vertex
        for (int i = 0, j = 0; i < 36; ++i, j+=8)
        {
            newVert = Vertex{ glm::vec3(vertices[j], vertices[j + 1], vertices[j + 2]), 
                              glm::vec3(vertices[j + 3], vertices[j + 4], vertices[j + 5]), 
                              glm::vec2(vertices[j + 6], vertices[j + 7]) };
            
            lightVertex.push_back(newVert);
        }

        return lightVertex;
	}

};


#endif // !LIGHTING_H

