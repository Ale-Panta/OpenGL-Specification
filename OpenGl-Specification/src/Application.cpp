#include "Utils.h"
#include "Math.h"
#include "Texture.h"
#include <GLFW/glfw3.h>	// Must be the last one to be included.
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stack>

using namespace std;
using namespace glm;
using namespace OpenGl;

static const int s_NumVAOs = 1;
static const int s_NumVBOs = 2;

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

void SetupVerticies()
{
	float pyramidPositions[54] =
	{ 
		-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// front face
		1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,	// right face
		 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // back face
		-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// left face
		-1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, // base – left front
		1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f	// base – right back
	};

	float pyrTexCoords[36] = 
	{ 
		0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, // top and right faces
		0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, // back and left faces
		0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f  // base triangles
	};

	glGenVertexArrays(s_NumVAOs, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(s_NumVBOs, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPositions), pyramidPositions, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyrTexCoords), pyrTexCoords, GL_STATIC_DRAW);
}

void Init(GLFWwindow* window) 
{
	renderingProgram = Utils::CreateShaderProgram("assets/shaders/vertShader.glsl", "assets/shaders/fragShader.glsl");
	
	cameraX = 0.0f;
	cameraY = 0.0f;
	cameraZ = 10.0f;

	SetupVerticies();

	// build perspective matrix.
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degree.

	exampleTexture = OpenGL::Texture::LoadTexture("assets/textures/test.JPEG");
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

	// Set up the projection matrix for all geometry.
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	
	// build view matrix, model matrix, and model-view matrix
	mat4 vMat = translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));

	// Push view matrix onto the stack
	mvStack.push(vMat);

	// Pyramid == sun
	mvStack.push(mvStack.top());
	mvStack.top() *= translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	mvStack.push(mvStack.top());
	mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f));

	// Copy perspective and MV matrices to corresponding uniforms variables
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	// Associate VBO with the corresponding vertex attribute in the vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);

	// Activate the texture
	glBindTexture(GL_TEXTURE_2D, exampleTexture);

	// Wrapping and tiling
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);	// Use GL_MIRRORED_REPEAT or GL_CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);	// Use GL_MIRRORED_REPEAT or GL_CLAMP_TO_EDGE

	// Mipmapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	// anisotropic filtering
	if (glewIsSupported("GL_EXT_texture_filter_anisotropic"))
	{
		GLfloat anisoSetting = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting);
	}

	// Adjust OpenGL settings and draw model
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 18);	// Draw pyramid
	mvStack.pop();	// Remove pyramid's rotation from the stack.

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