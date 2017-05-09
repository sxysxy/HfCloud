
#include "stdinc.h"
using namespace HfCloud;
//Fiber
#include "Fiber.h"
std::unordered_map<PVOID, Fiber*> Fiber::_fibers;
std::mutex Fiber::mutex;

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
