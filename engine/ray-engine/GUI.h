#pragma once


#include "SDL.h"
#include "Config.h"

#include <vector>
#include <string>

namespace app {


	enum KeyPosition {
		RIGHT,
		LEFT,
		MIDDLE
	};
	struct Inputs {
		struct Mouse {
			bool mouse[3];
			int relPos[2];
			int position[2];
			int wheel[2];
		} mouse;
		struct KeyBoard {
			bool ctrl[2];
			bool alt[2];
			bool shift[2];
			bool space;
			bool escape;
			bool printScreen;
		} keyboard;
		bool resized;
	};

	struct GUISettings {
		float fov;
		int depth;
		std::vector<std::string> scene;
	};

	class GUI
	{
	public:
		GUI(unsigned int width, unsigned int height);
		~GUI();

		void build(SDL_Window *window, SDL_GLContext glContext, const char* glslVersion);
		void draw();
		Inputs &events();
		//const GUISettings &settings(bool &updated);

	private:
		void prepare();
		void onKeyDown(SDL_KeyboardEvent &e);
		void onKeyUp(SDL_KeyboardEvent &e);
		void onMouseButtonDown(SDL_MouseButtonEvent &e);
		void onMouseButtonUp(SDL_MouseButtonEvent &e);
		void onMouseMotion(SDL_MouseMotionEvent &e);
		void onMouseWheel(SDL_MouseWheelEvent &e);
	private:
		SDL_Window *window;
		unsigned int width, height;
		Inputs m_inputs;
		//GUISettings m_settings;
		//bool changed;
	};
}
