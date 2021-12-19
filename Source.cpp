#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include"Cylinder.h"
#include"Rectangle.h"
#include"Sphere.h"
#include"lighting.h"
#include"Torus.h"

#include <iostream>


//class methods
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
bool getTextures();
Texture loadTexture(const char *path, const char* type);
bool Initialize();

// settings
const char* WIN_TITLE = "CS330 Final Project GC";
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
GLFWwindow* window;

// camera
Camera camera(glm::vec3(0.0f, 2.0f, 5.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
Lighting sceneLights;
glm::vec3 lightPos(-1.2f, 1.0f, 2.0f);

//view toggle
bool changeView;

//MVP matrices
glm::mat4 projection;
glm::mat4 model;
glm::mat4 view;

//placeholder vector for cylinder
std::vector<unsigned int> defaultInd = {
		1
};

//vector to store textures
std::vector<Texture> textureList;


int main()
{
	//initialize environment
	Initialize();

	// build and compile our shader zprogram
	// ------------------------------------
	Shader defaultProgram("shaderfiles/core.vs", "shaderfiles/core.frag");            //main shader
	Shader lightShader("shaderfiles/6.light_cube.vs", "shaderfiles/6.light_cube.fs"); //lighting shader

	//initializes texture list
	getTextures();

	//set up vertex data for shapes using classes
	Cylinder cyl1 = Cylinder(1, 30, 2, true, true, true);    //body of earbud case
	Cylinder cyl2 = Cylinder(1, 30, 2, true, true, true);    //mug body
	Cylinder cyl3 = Cylinder(1, 30, 0.25, true, true, true); //coaster
	Cylinder cyl4 = Cylinder(1, 30, 2, true, true, true);
	Sphere sp1 = Sphere(1.0f, 30, 30);                       //right left sides of earbud case
	Rectangle book1 = Rectangle(4.0f, 3.0f, 1.0f);           //book
	Rectangle plane1 = Rectangle(15.0f, 15.0f, 0.1f);        //countertop
	Torus handle = Torus(0.25, 2.0, 360, 360);               //mug handle
	
	//default vertex vector for torus code
	Vertex defaultVert = Vertex{ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f) };
	std::vector<Vertex> defaultVertVec;
	defaultVertVec.push_back(defaultVert);
	


	//exporting vertex data, indices, textures to mesh objects
	Mesh centerEar = Mesh(cyl1.getVertex(), defaultInd, textureList, "CYLINDER", cyl1.getVertSide(), cyl1.getVertTB(), cyl1.getVertTotal(), 0);
	Mesh mug = Mesh(cyl2.getVertex(), defaultInd, textureList, "CYLINDER", cyl2.getVertSide(), cyl2.getVertTB(), cyl2.getVertTotal(), 0);
	Mesh coast = Mesh(cyl3.getVertex(), defaultInd, textureList, "CYLINDER", cyl3.getVertSide(), cyl3.getVertTB(), cyl3.getVertTotal(), 0);
	Mesh candle = Mesh(cyl4.getVertex(), defaultInd, textureList, "CYLINDER", cyl4.getVertSide(), cyl4.getVertTB(), cyl4.getVertTotal(), 0);
	Mesh ear = Mesh(sp1.getSphereVerts(), sp1.getSphereInd(), textureList, "SPHERE", 0, 0, 0, 0);
	Mesh book = Mesh(book1.getRectangleVerts(), book1.getRectangleInd(), textureList, "RECTANGLE", 0, 0, 0, 0);
	Mesh surfaceBase = Mesh(plane1.getRectangleVerts(), plane1.getRectangleInd(), textureList, "RECTANGLE", 0, 0, 0, 0);
	Mesh handle1 = Mesh(defaultVertVec, defaultInd, textureList, "TORUS", handle.getTorusRadius(), handle.getTorusTubeRad(), handle.getTorusRSegment(), handle.getTorusCSegment());
	Mesh skyLight = Mesh(sceneLights.Create(), defaultInd, textureList, "LIGHT", 0, 0, 0, 0); //lightbox for scene lighting

	// shader configuration
	// --------------------
	defaultProgram.use();
	
	//setting default material attribute locations
	defaultProgram.setInt("material.diffuse", 0);
	defaultProgram.setInt("material.specular", 1);
	

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);

		// clear buffers & background
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// activates shader
		defaultProgram.use();

		//enabling directional and point lights
		sceneLights.Direct(defaultProgram);
		sceneLights.Point(defaultProgram);
		//sceneLights.Spot(defaultProgram);

		//mesh scaling vectors
		glm::vec3 mugScale(0.9f, 1.0f, 0.9f);
		glm::vec3 innerMugScale(0.87f, 0.9f, 0.87f);
		glm::vec3 scale(1.5f, 1.0f, 1.5f);
		glm::vec3 bookScale(1.0f, 0.75f, 0.9f);
		glm::vec3 bookScale2(1.0f, 0.73f, 0.9f);
		glm::vec3 bookScale3(1.005f, 0.70f, 0.9f);
		glm::vec3 earScale(0.25f, 0.25f, 0.25f);
		glm::vec3 coasterScale(1.1f, 0.9f, 1.1f);
		glm::vec3 candleScale(1.0f, 1.2f, 1.0f);
		glm::vec3 lidScale(1.1f, 0.1f, 1.1f);
		glm::vec3 handleScale(0.15f, 0.15f, 0.15f);

		//mesh transform vectors
		glm::vec3 newPosition(-10.0f, 3.0f, 5.0f);
		glm::vec3 rightEarPos(0.77f, 1.1f, 3.70f);
		glm::vec3 leftEarPos(1.05f, 1.1f, 3.30f);
		glm::vec3 earbudCenter(0.9f, 1.1f, 3.5f);
		glm::vec3 bookPos(2.0f, 0.5f, 4.0f);
		glm::vec3 bookPos2(2.0f, 0.5f, 3.995f);
		glm::vec3 bookPos3(2.0f, 0.5f, 4.005f);
		glm::vec3 mugPos(3.0f, 1.0f, -3.0f);
		glm::vec3 innerMugPos(3.0f, 1.11f, -3.0f);
		glm::vec3 coasterPos(3.0f, 0.1f, -3.0f);
		glm::vec3 candlePos(3.5f, 1.25f, 0.0f);
		glm::vec3 lidPos(3.5f, 2.50f, 0.0f);
		glm::vec3 handlePos(3.6f, 1.0f, -2.2f);
		glm::vec3 planePos(4.0f, 0.0f, 0.0f);

		//object rotations degrees and vectors
		glm::vec3 xyzRot(1.0f, 1.0f, 1.0f);
		glm::vec3 xyzCandleRot(1.0f, 0.0f, 0.7f);
		glm::vec3 xyzHandleRot(0.0f, 1.0f, 0.0f);
		glm::vec3 xyzEarRot(1.0f, -0.4f, 0.2f);
		glm::vec3 xyzMugRot(1.0f, 0.0f, 0.5f);
		glm::vec3 xyzBookRot(0.0f, 1.0f, 0.0f);
		float objRotate = 0.0f;
		float bookRotate = -30.0f;
		float earRotate = 100.0f;
		float handleRotate = -45.0f;
		float flipObj = 180.0f;
		
		//mesh texture scaling
		glm::vec2 texUVScale(10.0f, 10.0f);
		glm::vec2 defaultUV(1.0f, 1.0f);
		glm::vec2 coasterUV(2.0f, 2.0f);

		//shininess presets
		float dull = 16.0f;
		float avg = 32.0f;
		float shiny = 64.0f;
		float vShiny = 128.0f;

		//-------------------------------------------------------------------MESH ATTRIBUTE MANIPULATION DEFINITION----------------------------------------------------------------------------------------------------------------------------
		// (shader, camera, changeview, obj scale, UV scale, screen w + h, obj position transform[x,y,z], obj rotation degree, obj rotation axis, diffuse texture index, specular texture index, extra texture index[0 if none], material shine)
		//=======================================================================================================================================================================================================================================
		ear.Draw(defaultProgram, camera, changeView, earScale, defaultUV, SCR_WIDTH, SCR_HEIGHT, rightEarPos, objRotate, xyzRot, 0, 1, 0, avg);
		ear.Draw(defaultProgram, camera, changeView, earScale, defaultUV, SCR_WIDTH, SCR_HEIGHT, leftEarPos, objRotate, xyzRot, 0, 1, 0, avg);
		centerEar.Draw(defaultProgram, camera, changeView, earScale, defaultUV, SCR_WIDTH, SCR_HEIGHT, earbudCenter, earRotate, xyzEarRot, 0, 1, 0, avg);
		book.Draw(defaultProgram, camera, changeView, bookScale, defaultUV, SCR_WIDTH, SCR_HEIGHT, bookPos, bookRotate, xyzBookRot, 7, 7, 8, dull);
		book.Draw(defaultProgram, camera, changeView, bookScale2, defaultUV, SCR_WIDTH, SCR_HEIGHT, bookPos2, bookRotate, xyzBookRot, 7, 7, 0, dull); 
		book.Draw(defaultProgram, camera, changeView, bookScale3, defaultUV, SCR_WIDTH, SCR_HEIGHT, bookPos3, bookRotate, xyzBookRot, 11, 11, 0, dull);
		mug.Draw(defaultProgram, camera, changeView, mugScale, defaultUV, SCR_WIDTH, SCR_HEIGHT, mugPos, flipObj, xyzMugRot, 5, 15, 0, dull);
		mug.Draw(defaultProgram, camera, changeView, innerMugScale, defaultUV, SCR_WIDTH, SCR_HEIGHT, innerMugPos, objRotate, xyzRot, 6, 14, 0, shiny);        //scaled down mug for inner part
		coast.Draw(defaultProgram, camera, changeView, coasterScale, coasterUV, SCR_WIDTH, SCR_HEIGHT, coasterPos, objRotate, xyzRot, 9, 13, 0, dull);
		candle.Draw(defaultProgram, camera, changeView, candleScale, defaultUV, SCR_WIDTH, SCR_HEIGHT, candlePos, flipObj, xyzCandleRot, 10, 12, 0, avg);
		candle.Draw(defaultProgram, camera, changeView, lidScale, defaultUV, SCR_WIDTH, SCR_HEIGHT, lidPos, objRotate, xyzRot, 16, 17, 0, shiny);              //reuse candle mesh scaled to lid
		handle1.Draw(defaultProgram, camera, changeView, handleScale, defaultUV, SCR_WIDTH, SCR_HEIGHT, handlePos, handleRotate, xyzHandleRot, 5, 15, 0, avg);
		surfaceBase.Draw(defaultProgram, camera, changeView, scale, texUVScale, SCR_WIDTH, SCR_HEIGHT, planePos, objRotate, xyzRot, 2, 3, 0, vShiny);
		skyLight.Draw(lightShader, camera, changeView, scale, defaultUV, SCR_WIDTH, SCR_HEIGHT, newPosition, objRotate, xyzRot, 0, 1, 0, avg);                 //draws light source object

		
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}



bool Initialize()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, WIN_TITLE, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	return 0;
}



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) //closes window
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) //moves forwards
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) //moves backwards
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) //moves left
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) //moves right
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) //moves up vertically
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) //moves down vertically
		camera.ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) //toggles ortho and perspective views
		if (changeView == true)
		{
			changeView = false;
		}
		else
		{
			changeView = true;
		}
}



// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}



// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}



// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

//loads textures from project data
bool getTextures()
{
	// load textures (we now use a utility function to keep the code more organized)
	Texture earbudBase = loadTexture("earbuds.png", "earbudDiff");
	Texture earbudSpec = loadTexture("earbudSpecular.png", "earbudSpec");
	Texture counterTop = loadTexture("stone1.png", "countertopDiff");
	Texture counterTopSpec = loadTexture("counterSpec.jpg", "countertopSpec");
	Texture mugBase = loadTexture("redMetal.png", "mugDiff");
	Texture mugInterior = loadTexture("interiorMug.png", "mugInteriorDiff");
	Texture mugLogo = loadTexture("nuka.png", "mugLogo");
	Texture bookBase = loadTexture("bookBase.png", "bookDiff");
	Texture bookCover = loadTexture("bookImage.png", "bookLogo");
	Texture corkBase = loadTexture("cork.png", "corkDiff");
	Texture candleBase = loadTexture("candle.png", "candleDiff");
	Texture bookPage = loadTexture("bookPage.png", "bookPageTexture");
	Texture candleSpec = loadTexture("candlespecular.jpg", "candleSpec");
	Texture corkSpec = loadTexture("corkspec.jpg", "corkSpec");
	Texture intMugSpec = loadTexture("interiorMugSpec.jpg", "intMugSpec");
	Texture mugSpec = loadTexture("redMetalSpec.jpg", "mugSpec");
	Texture candleLid = loadTexture("candleLid.png", "candleLid");
	Texture lidSpec = loadTexture("candleLidSpec.png", "candleSpec");

	//add texture to vector
	textureList.push_back(earbudBase);     //0 - earbuds diffuse
	textureList.push_back(earbudSpec);     //1 - earbuds spec
	textureList.push_back(counterTop);     //2 - countertop diffuse
	textureList.push_back(counterTopSpec); //3 - countertop spec
	textureList.push_back(mugBase);        //4 - mugBase diffuse
	textureList.push_back(mugLogo);        //5 - mug logo
	textureList.push_back(mugInterior);    //6 - mug interior diffuse
	textureList.push_back(bookBase);       //7 - book diffuse
	textureList.push_back(bookCover);      //8 - book logo
	textureList.push_back(corkBase);       //9 - cork diffuse
	textureList.push_back(candleBase);     //10 - candle diffuse
	textureList.push_back(bookPage);       //11 - book side pages
	textureList.push_back(candleSpec);     //12 - specular map candle
	textureList.push_back(corkSpec);       //13 - specular map cork
	textureList.push_back(intMugSpec);     //14 - specular map inside mug
	textureList.push_back(mugSpec);        //15 - specular map outside mug
	textureList.push_back(candleLid);      //16 - candle lid diffuse
	textureList.push_back(lidSpec);        //17 - candle lid specular

	return true;
}

// utility function for loading a 2D texture from file CHANGE TO TEXTURE TYPE RETURNS TEXTURE OBJECT(ID, PATH, TYPE) THAT CAN ADD TO VECTOR
// ---------------------------------------------------
Texture loadTexture(char const * path, const char* type)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
	Texture newTexture = Texture{ textureID, path, type };

	return newTexture;
}