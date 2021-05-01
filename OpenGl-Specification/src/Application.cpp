#include "OpenGLApp.h"
#include "../assets/scenes/LitScene.h"

int main(void)
{ 
	OpenGL::OpenGLApp app("OpenGL Demo", 800, 800, new OpenGL::LitScene(1.0472f, 800 / 800, 0.1f, 100.0f));

	app.InitWindow();
	app.IntiOpenGL();
	app.Run();

	return 0;
}