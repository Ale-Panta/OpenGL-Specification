#include "OpenGLApp.h"
#include "ThesisApp.h"

int main(void)
{ 
	OpenGL::ThesisApp app("OpenGL Demo", 1280, 720, 4);
	app.Initialize();
	app.Run();

	return 0;
}