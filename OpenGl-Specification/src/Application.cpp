#include "OpenGLApp.h"
#include "../assets/scenes/OITScene.h"

int main(void)
{ 
	OpenGL::OpenGLApp app("OpenGL Demo", 1080, 1080, new OpenGL::OITScene(1.0472f, 800 / 800, 0.1f, 100.0f));

	app.InitWindow();
	app.IntiOpenGL();
	app.Run();

	return 0;
}