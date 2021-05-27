#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include <string>

namespace OpenGL
{
	/**
	 * Application superclass.
	 * All specialized applications must inherit from this one.
	 * 
	 * @author	Alessandro Pantano
	 */
	class OpenGLApp
	{
	public:
		/**
		 * OpenGLApp constructor.
		 * 
		 * @param name		Application name
		 * @param width		Client window width
		 * @param height	Client window height
		 * @param samples	Is recommended to use 4, 8 or 16, more than that can cause performance issue
		 * @warning			More the 16 samples can cause performance issue
		 */
		OpenGLApp(const char* name, int width, int height, int samples);

		/**
		 * OpenGLApp copy constructor.
		 * Deleted.
		 */
		OpenGLApp(const OpenGLApp& rhs) = delete;

		/**
		 * OpenGLApp assign operator.
		 * Deleted.
		 */
		OpenGLApp operator = (const OpenGLApp& rhs) = delete;

		/**
		 * OpenGLApp destructor.
		 * Delete manually all resources allocated in the heap.
		 */
		virtual ~OpenGLApp();

	public:
		/**
		 * Get the current application.
		 * 
		 * @return The application instance pointer
		 */
		static OpenGLApp* GetApp();

		/**
		 * Get the current app's aspect ratio.
		 * 
		 * @return the width / height.
		 */
		float GetAspectRatio();

		/**
		 * Intialize window, opengl, resources in this order.
		 * Call the base before initialize resources.
		 * 
		 * @return <code>true</code> if no stage fails else <code>false</code>
		 */
		virtual bool Initialize();

		/**
		 * Run the application loop.
		 * Calculate the application time and frame, 
		 * process events and call Update and Draw.
		 * 
		 * @see Update
		 * @see Draw
		 */
		void Run();

	protected:
		/**
		 * Resize window event.
		 */
		virtual void OnResize();
		
		/**
		 * Update generic application stuff.
		 * Reset buffers or update position based on time for examples.
		 * 
		 * @param gt	Application time elapsed since start
		 */
		virtual void Update(double gt) = 0;

		/**
		 * Draw the scene.
		 * It is divided into multiple stages that are the follow:
		 * render sky box, shadow, opaque, transparent.
		 * This order must not be broke.
		 * 
		 * @param gt	Application time elapsed since start
		 */
		virtual void Draw(double gt) = 0;

		/** 
		 * Initialize the context window.
		 */
		bool InitMainWindow();

		/**
		 * Initialize glew. It retrieve the function to communicate with GPU.
		 * 
		 * @warning		Must be called after InitWindow()
		 */
		bool InitiOpenGL();

		/**
		 * Create depth buffer object and texture attach to it.
		 */
		void CreateDepthBuffer();

		/**
		 * Create all OIT stuff.
		 */
		void CreateOITBuffers();

		/**
		 * Refresh OIT stuff.
		 *
		 * @warning		make sure to have initialize m_data first
		 * @throw		null reference exception if m_data is not initialized first
		 */
		void RefresOITBuffers();

	protected:
		// --------------------------------------------------------------------------
		// OIT stuff
		// --------------------------------------------------------------------------

		GLuint m_HeadPtrTexture			= 0;
		GLuint m_HeadPtrClearBuffer		= 0;
		GLuint m_AtomicCounterBuffer	= 0;
		GLuint m_LinkedListBuffer		= 0;
		GLuint m_LinkedListTexture		= 0;
		GLuint* m_data = nullptr;

		// --------------------------------------------------------------------------
		// Shadow Mapping stuff
		// --------------------------------------------------------------------------

		const int m_ShadowTexWidth = 2048;
		const int m_ShadowTexHeight = 2048;
		GLuint m_ShadowFBO = 0;
		GLuint m_ShadowTexture = 0;

		/** Client window width. */
		int m_ClientWidth = 600;

		/** Client window height. */
		int m_ClientHeight = 600;

	private:
		/** Current running application ptr */
		static OpenGLApp* m_App;

		/** Window ptr of the application. */
		GLFWwindow* m_Context = nullptr;

		/** Window's name. */
		std::string m_WindowName = "OpenGL Default Demo Window";

		/** 
		 * Number of samples taken for pixels. 
		 * Usually you want 4 or 8 samples but you can have more like 16.
		 * More than 16 is not recommended.
		 */
		int m_NumOfSamples = 4;
	};
}