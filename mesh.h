#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> // holds all OpenGL type declarations
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include"camera.h"

#include <string>
#include <vector>
using namespace std;

struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
};
//stores path, id, and path of textures
struct Texture {
	unsigned int id;
	string type;
	string path;
};

class Mesh {
public:
	// mesh Data
	vector<Vertex>       vertices;
	vector<unsigned int> indices;
	vector<Texture>      textures;
	const char* meshType;
	unsigned int VAO;
	double side, top, total, auxSlot; //reserved for cylinder drawing or auxillary use for shape requirements
	int numVert; //stores torus verts
	

	// constructor
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, const char* meshType, double side, double top, double total, double auxSlot)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		this->meshType = meshType;
		this->side = side;
		this->top = top;
		this->total = total;
		this->auxSlot = auxSlot;
		
		//torus is generated via different buffer and data
		if (meshType == "TORUS")
		{
			setUpTorus();
		}
		else
		{
			// set the vertex buffers and its attribute pointers.
			setupMesh();
		}
	}

	// render the mesh
	void Draw(Shader &shader, Camera &camera, bool isOrtho, glm::vec3 scale, glm::vec2 UVScale, int widthW, int heightW, glm::vec3 newPosition, float objRotation, glm::vec3 xyzRot, short texNumD, short texNumS, short texNumA, float shiny)
	{
		
		glBindVertexArray(VAO);
		
		//model transformations
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, newPosition); //model translation
		model = glm::rotate(model, glm::radians(objRotation), xyzRot); //model rotation
		model = glm::scale(model, scale); //model scale

		// camera/view mat4 + projection mat4
		glm::mat4 view;
		glm::mat4 projection;
		
		//ortho projection transformation
		if (isOrtho == true)
		{
			projection = glm::ortho(0.0f, 5.0f, 0.0f, 5.0f, 0.1f, 100.0f);
			view = camera.GetViewMatrix();
			shader.setMat4("projection", projection);
			shader.setMat4("view", view);
		}
		//perspective projection transformation
		else
		{
			projection = glm::perspective(glm::radians(camera.Zoom), (float)widthW / (float)heightW, 0.1f, 100.0f);
			view = camera.GetViewMatrix();
			shader.setMat4("projection", projection);
			shader.setMat4("view", view);
		}

		//lighting object must be drawn using lighting shader
		if (meshType == "LIGHT")
		{
			shader.use();
			shader.setMat4("projection", projection);
			shader.setMat4("view", view);

			model = glm::mat4(1.0f);
			model = glm::translate(model, newPosition);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			shader.setMat4("model", model);
			
			glUniform3f(glGetUniformLocation(shader.ID, "color"), 1.0f, 1.0f, 1.0f); //modifies objects color SHOULD MATCH LIGHTING DIFFUSE COLOR
			glDrawArrays(GL_TRIANGLES, 0, 36);
			return;
		}
		

		// Retrieves and passes transform matrices to the Shader program
		GLint modelLoc = glGetUniformLocation(shader.ID, "model");
		GLint viewLoc = glGetUniformLocation(shader.ID, "view");
		GLint projLoc = glGetUniformLocation(shader.ID, "projection");

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//sets view position uniform
		GLint viewPositionLoc = glGetUniformLocation(shader.ID, "viewPosition");
		
		//camera position uniform
		const glm::vec3 cameraPosition = camera.Position;
		glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

		//passes texture uv scale to shader
		GLint UVScaleLoc = glGetUniformLocation(shader.ID, "uvScale");
		glUniform2fv(UVScaleLoc, 1, glm::value_ptr(UVScale));

		//assigns material shine
		shader.setFloat("material.shininess", shiny);

		//binds diffuse material map for mesh
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[texNumD].id);
		glUniform1i(glGetUniformLocation(shader.ID, (textures[texNumD].type).c_str()), texNumD);
		
		//binds specular material map for mesh
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[texNumS].id);
		glUniform1i(glGetUniformLocation(shader.ID, (textures[texNumS].type).c_str()), texNumS);
		
		//if mesh has an augment texture binds material to overlay where it is mixed with diffuse color in frag shader
		if (texNumA != 0)
		{
			shader.setInt("texAugment", 2);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, textures[texNumA].id);
			glUniform1i(glGetUniformLocation(shader.ID, (textures[texNumA].type).c_str()), texNumA);
		}
		//if mesh doesn't have extra texture then sets the shader uniform equal to diffuse map
		if (texNumA == 0)
		{
			shader.setInt("texAugment", 0);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, textures[texNumA].id);
			glUniform1i(glGetUniformLocation(shader.ID, (textures[texNumA].type).c_str()), texNumA);
		}
		//drawing different mesh types
		if (meshType == "CYLINDER")
		{
			//render sides
			glDrawArrays(GL_TRIANGLE_STRIP, 0, side);

			// Render top cover
			glDrawArrays(GL_TRIANGLE_FAN, side, top);

			// Render bottom cover
			glDrawArrays(GL_TRIANGLE_FAN, side + top, top);
		}
		else if (meshType == "SPHERE")
		{
			
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		}
		else if (meshType == "RECTANGLE")
		{
			
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		}
		else if (meshType == "TORUS")
		{
			glDrawArrays(GL_TRIANGLES, 0, numVert * 3); // 12*3 indices starting at 0 -> 12 triangles
		}
		
		glBindVertexArray(0);

		//clear back to default
		glActiveTexture(GL_TEXTURE0);
	}

	//function to create vertex data for torus
	void setCoords(double r, double c, int rSeg, int cSeg, int i, int j,
		GLfloat* vertices, GLfloat* uv) {
		const double PI = 3.1415926535897932384626433832795;
		const double TAU = 2 * PI;

		double x = (c + r * cos(i * TAU / rSeg)) * cos(j * TAU / cSeg);
		double y = (c + r * cos(i * TAU / rSeg)) * sin(j * TAU / cSeg);
		double z = r * sin(i * TAU / rSeg);

		vertices[0] = 2 * x;
		vertices[1] = 2 * y;
		vertices[2] = 2 * z;

		uv[0] = i / (double)rSeg;
		uv[1] = j / (double)cSeg;
	}
	//function setting number of iterations for torus
	int createObject(double r, double c, int rSeg, int cSeg, GLfloat** vertices,
		GLfloat** uv) {
		int count = rSeg * cSeg * 6;
		*vertices = (GLfloat*)malloc(count * 3 * sizeof(GLfloat));
		*uv = (GLfloat*)malloc(count * 2 * sizeof(GLfloat));

		for (int x = 0; x < cSeg; x++) { // through stripes
			for (int y = 0; y < rSeg; y++) { // through squares on stripe
				GLfloat* vertexPtr = *vertices + ((x * rSeg) + y) * 18;
				GLfloat* uvPtr = *uv + ((x * rSeg) + y) * 12;
				setCoords(r, c, rSeg, cSeg, x, y, vertexPtr + 0, uvPtr + 0);
				setCoords(r, c, rSeg, cSeg, x + 1, y, vertexPtr + 3, uvPtr + 2);
				setCoords(r, c, rSeg, cSeg, x, y + 1, vertexPtr + 6, uvPtr + 4);

				setCoords(r, c, rSeg, cSeg, x, y + 1, vertexPtr + 9, uvPtr + 6);
				setCoords(r, c, rSeg, cSeg, x + 1, y, vertexPtr + 12, uvPtr + 8);
				setCoords(r, c, rSeg, cSeg, x + 1, y + 1, vertexPtr + 15,
					uvPtr + 10);
			}
		}

		return count;
	}
	


private:
	// render data 
	unsigned int VBO, EBO;

	// initializes all the buffer objects/arrays
	void setupMesh()
	{
		// create buffers/arrays
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		// load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// A great thing about structs is that their memory layout is sequential for all its items.
		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		// again translates to 3/2 floats which translates to a byte array.
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		//all meshes besides cylinder and light type use indices
		if (meshType != "CYLINDER" || meshType != "LIGHT")
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		}

		// set the vertex attribute pointers
		if (meshType != "LIGHT")
		{

			// vertex Positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
			// vertex normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
			// vertex texture coords
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));


		}
		else //for light mesh type we only want 1st three values of vertex
		{
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			// updating stride to skip to 1st position of each vertex data packet
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
		}

		glBindVertexArray(0);
	}
	//method generates VAO for torus
	void setUpTorus()
	{
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		//create buffer pointers for torus functions
		GLfloat* g_vertex_buffer_data;
		GLfloat* g_uv_buffer_data;
		//builds torus with data from main
		numVert = createObject(side, top, total, auxSlot, &g_vertex_buffer_data,
			&g_uv_buffer_data);

		GLuint vertexbuffer;
		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, numVert * 3 * sizeof(GLfloat),
			g_vertex_buffer_data, GL_STATIC_DRAW);
		
		GLuint uvbuffer;
		glGenBuffers(1, &uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, numVert * 2 * sizeof(GLfloat),
			g_uv_buffer_data, GL_STATIC_DRAW);

		// 1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(1, // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size : U+V => 2
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);


	}
};


#endif