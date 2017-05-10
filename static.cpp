
#include "stdinc.h"
using namespace HfCloud;
//Fiber
#include "Fiber.h"

thread_local std::unordered_map<ProcHandle, Fiber*> Fiber::_fibers;

#ifdef __linux__

thread_local ProcHandle Fiber::nowRunning = nullptr;

#endif

//Graphics
#include "Graphics.h"
SDL_Window *Graphics::window;
SDL_Renderer *Graphics::render;
int Graphics::frame_count = 0;
int Graphics::width, Graphics::height;

//input
#include "Input.h"
std::vector<KeyInfo> Input::keys;
std::vector<ButtonInfo> Input::buttons;

//

