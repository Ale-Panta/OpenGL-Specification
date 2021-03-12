#include "Utils.h"
#include "Math.h"
#include "Texture.h"
#include "Sphere.h"
#include <GLFW/glfw3.h>	// Must be the last one to be included.
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stack>
#include <vector>
#include <iostream>

using namespace std;
using namespace glm;
using namespace OpenGl;
using namespace OpenGL;

static const int s_NumVAOs = 1;
static const int s_NumVBOs = 3;

float cameraX;
float cameraY;
float cameraZ;

GLuint renderingProgram;
GLuint vao[s_NumVAOs];
GLuint vbo[s_NumVBOs];

GLuint mvLoc;
GLuint projLoc;

GLuint exampleTexture;

int width;
int height;
float aspect;

mat4 pMat;

// When an object is created relative to a parent object, call .push().
// Apply the new object's desired transforms.
// When an object or sub-object has finished begin drawn, call .pop() to remove its model-view matrix
// from atop the matrix stack.
stack<glm::mat4> mvStack;

Sphere mySphere(64);

void SetupVerticies()
{
	std::vector<int> ind = mySphere.GetIndices();   
	std::vector<glm::vec3> vert = mySphere.GetVertices();  
	std::vector<glm::vec2> tex = mySphere.GetTexCoords();   
	std::vector<glm::vec3> norm = mySphere.GetNormals();   
	std::vector<float> pvalues;  // vertex positions  
	std::vector<float> tvalues;  // texture coordinates  
	std::vector<float> nvalues;  // normal vectors
	int numIndices = mySphere.GetIndicesCount();   
	
	for (int i = 0; i < numIndices; i++) 
	{
		pvalues.push_back((vert[ind[i]]).x); 
		pvalues.push_back((vert[ind[i]]).y); 
		pvalues.push_back((vert[ind[i]]).z); 
		tvalues.push_back((tex[ind[i]]).s); 
		tvalues.push_back((tex[ind[i]]).t); 
		nvalues.push_back((norm[ind[i]]).x); 
		nvalues.push_back((norm[ind[i]]).y); 
		nvalues.push_back((norm[ind[i]]).z); 
	} 
	
	glGenVertexArrays(1, vao);   
	glBindVertexArray(vao[0]);   
	glGenBuffers(3, vbo);
	
	// put the vertices into buffer #0   
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);   
	glBufferData(GL_ARRAY_BUFFER, pvalues.size()*4, &pvalues[0], GL_STATIC_DRAW);
	
	// put the texture coordinates into buffer #1   
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);   
	glBufferData(GL_ARRAY_BUFFER, tvalues.size()*4, &tvalues[0], GL_STATIC_DRAW);
	
	// put the normals into buffer #2   
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);   
	glBufferData(GL_ARRAY_BUFFER, nvalues.size()*4, &nvalues[0], GL_STATIC_DRAW);
}

void Init(GLFWwindow* window) 
{
	renderingProgram = Utils::CreateShaderProgram("assets/shaders/vertShader.glsl", "assets/shaders/fragShader.glsl");
	
	cameraX = 0.0f;
	cameraY = 0.0f;
	cameraZ = 5.0f;

	SetupVerticies();

	// build perspective matrix.
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degree.
}

void WindowReshapeCallback(GLFWwindow* window, int newWidth, int newHeight)
{
	// build perspective matrix.
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat = perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degree.
}

// Called each frame.
void display(GLFWwindow* window, double currentTime)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_DEPTH_BUFFER_BIT);	// Clear the background to black, each time
	glClear(GL_COLOR_BUFFER_BIT);	// Clear the color buffer to black, each time

	glUseProgram(renderingProgram);	// Install GLSL program to the GPU. Now we can access to uniforms and attributes.

	// Get the uniforms variables for the MV and projection matrices.
	//mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	// build view matrix, model matrix, and model-view matrix
	mat4 vMat = translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	mvStack.push(vMat);

	mvStack.push(mvStack.top());
	mvStack.top() *= translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime), cos((float)currentTime), 0.0f));
	mvStack.push(mvStack.top());
	mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));

	// Set up the projection matrix for all geometry.
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	// Copy perspective and MV matrices to corresponding uniforms variables
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glEnable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, mySphere.GetIndicesCount());

	mvStack.pop();
	mvStack.pop();
	mvStack.pop();
}

int main(void)
{ 
	if (!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Create the window context.
	// Note: Creating the window doesn't make it current context by default.
	GLFWwindow* window = glfwCreateWindow(1280, 720, "OpenGl Demo", nullptr, nullptr);

	// Mark the window as current context
	glfwMakeContextCurrent(window);

	// glewInit must be called after the creation of the context.
	if (glewInit() != GLEW_OK)
	{
		return -1;
	}

	glfwSwapInterval(1);

	glfwSetWindowSizeCallback(window, WindowReshapeCallback);

	Init(window);

	while (!glfwWindowShouldClose(window))
	{
		display(window, glfwGetTime());
		glfwSwapBuffers(window);	// GLFW are by default double-buffered.
		glfwPollEvents();	// Handle other window-related events.
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}