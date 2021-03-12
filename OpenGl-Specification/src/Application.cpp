#include "OpenGLApp.h"

int main(void)
{ 
	OpenGL::OpenGLApp app("OpenGL Demo");

	app.InitWindow();
	app.IntiOpenGL();
	app.Run();

	return 0;
}