#include "Fiber.h"

std::unordered_map<PVOID, Fiber*> Fiber::_fibers;

std::mutex Fiber::mutex;
