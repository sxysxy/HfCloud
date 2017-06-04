/***************************
	class Fiber
	author: Rio Shiina
	E-mail: 390855044@qq.com
****************************/

#pragma once

#ifdef _WIN32

#include <Windows.h>

using ProcHandle = PVOID;

#elif defined(__linux__)

#include <ucontext.h>

using ProcHandle = ucontext_t*;

#endif

#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <memory>
#include <stack>
#include "stdinc.h"
#if defined(__LINUX__) && !defined(__linux__)
#define __linux__ __LINUX__
#endif

#define FIBER_STACK_SIZE 0x100000

enum class FiberStatus {

	Running,
	HungUp,
	Dead,

};

class Fiber {

    bool exit;

    struct Fiber_Exit {};

	struct Context {

		Fiber *self;

		ProcHandle proc;

		std::function<void()> functor;

#ifdef __linux__

		ucontext_t ucontext;

		void *pstack; /**< ptr to stack. */

#endif

	};

#ifdef _WIN32

	inline static VOID WINAPI FiberProc(PVOID pvParam) {

		Context &context = *reinterpret_cast<Context*>(pvParam);

        if (context.functor)
            try {

            context.functor();

            context.self->_dead.insert(context.proc);

        }
        catch (Fiber_Exit) {}

		context.self->yield();

	}

#elif defined(__linux__)

	static thread_local ProcHandle nowRunning;

	static void FiberProc(Context *context) {

		if (context->functor)
			context->functor();

		context->self->_dead.insert(context->proc);

		context->self->yield();

	}

#endif

	static void CreateFiber(Context *context) {

#ifdef _WIN32

		context->proc = ::CreateFiberEx(
			0,
			0,
			FIBER_FLAG_FLOAT_SWITCH,
			FiberProc,
			context
		);

#elif defined(__linux__)

		ProcHandle handle = context->proc = &context->ucontext;

		::getcontext(handle);

        handle->uc_stack.ss_sp = context->pstack = malloc(FIBER_STACK_SIZE);
        HFASSERT(context->pstack != nullptr, "Create fiber failed when allocating stack.")

		handle->uc_stack.ss_size = FIBER_STACK_SIZE;

		::makecontext(handle, (void(*)())FiberProc, 1, context);

#endif

	}

    void SwitchToFiber(ProcHandle handle) {

#ifdef _WIN32

        ::SwitchToFiber(handle);

#elif defined(__linux__)

        auto nowRunning = _nowRunning();

        ProcHandle prevHandle = nowRunning;

        nowRunning = handle;

        ::swapcontext(prevHandle, handle);

#endif

        if (exit)
            throw Fiber_Exit();

    }

	static void DeleteFiber(ProcHandle handle) {

#ifdef _WIN32

		::DeleteFiber(handle);

#endif

	}

	static ProcHandle GetFiber() {

#ifdef _WIN32

		return ::GetCurrentFiber();

#elif defined(__linux__)

		return nowRunning;

#endif

	}

	static std::unordered_map<ProcHandle, Fiber*> _fibers;
	static std::mutex _fibers_mutex;

	std::unordered_map<unsigned, Context> _contexts;

	std::stack<ProcHandle> _handleStack;

	std::unordered_set<ProcHandle> _dead;

	ProcHandle _handle;

public:

	Fiber() {}

	Fiber(const Fiber &fiber) {

		for (auto &i : fiber._contexts)
			_contexts.insert(std::make_pair(i.first, Context{ this, nullptr, i.second.functor }));

	}

	Fiber(Fiber &&fiber) {
        for (auto &i : fiber._contexts)
            _contexts.insert(std::make_pair(i.first, Context{ this, nullptr, std::move(i.second.functor) }));

	}

	void resume(unsigned n) {

		auto it = _contexts.find(n);

		if (it == _contexts.end())
			throw std::runtime_error("Fiber proc does not exist.");

		if (!it->second.proc)
			CreateFiber(&it->second);


		if (_dead.find(it->second.proc) != _dead.end())
			throw std::runtime_error("Drad Fiber");

		_handleStack.push(_handle);

		_handle = it->second.proc;

		_fibers_mutex.lock();
			_fibers.insert(std::make_pair(_handle, this));
    	_fibers_mutex.unlock();

		SwitchToFiber(_handle);
	}

	void yield() {

		_handle = _handleStack.top();

		_handleStack.pop();

		SwitchToFiber(_handle);

	}

	std::function<void()> &operator[](unsigned n) {

		auto it = _contexts.find(n);

		if (it == _contexts.end())
			it = _contexts.insert(std::make_pair(n, Context{ this, nullptr })).first;

		return it->second.functor;

	}

    //update on 2017.5.30 by sxysxy
    bool exist(unsigned n) {
        auto it = _contexts.find(n);
        return it != _contexts.end();
    }

	void run(unsigned n) {

#ifdef _WIN32

		_handle = ::ConvertThreadToFiber(nullptr);

#elif defined(__linux__)

		ucontext_t ucontext;

		nowRunning = _handle = &ucontext;

#endif

        exit = false;
		resume(n);
        exit = true;

			for (auto &i : _contexts) {

				ProcHandle &handle = i.second.proc;

				if (handle) {
                    if (_dead.find(handle) == _dead.end()) {
                        try {
                            resume(i.first);
                        }
                        catch (Fiber_Exit) {}
                    }


					DeleteFiber(handle);

					handle = nullptr;

#ifdef __linux__
                    free(i.second.pstack);
                    i.second.pstack = nullptr;
#endif

				}

			}

			_dead.clear();

            _fibers_mutex.lock();
				_fibers.clear();
			_fibers_mutex.unlock();

#ifdef _WIN32

			::ConvertFiberToThread();

#endif

	}

	FiberStatus status(unsigned n) const {

		auto it = _contexts.find(n);

		if (it == _contexts.end())
			throw std::runtime_error("Fiber proc does not exist.");

		ProcHandle handle = it->second.proc;

		if (handle == _handle)
			return FiberStatus::Running;

		if (_dead.find(handle) != _dead.end())
			return FiberStatus::Dead;

		return FiberStatus::HungUp;

	}

	bool isRunning(unsigned n) const {

		return status(n) == FiberStatus::Running;

	}

	bool isHungUp(unsigned n) const {

		return status(n) == FiberStatus::HungUp;

	}

	bool isDead(unsigned n) const {

		return status(n) == FiberStatus::Dead;

	}

	void kill(unsigned n) {

		auto it = _contexts.find(n);

		if (it == _contexts.end())
			throw std::runtime_error("Fiber proc does not exist.");

#ifndef __linux__

		ProcHandle handle = it->second.proc;

		if (handle)
			_dead.insert(handle);
		else
			throw std::runtime_error("Fiber proc did not run.");
#endif
		_contexts.erase(it);
	}

    void destroy(unsigned n){ //destroy all noete about Fiber n and release sources;

        HFASSERT(!isRunning(n), "Can not destroy a running fiber")

        auto it = _contexts.find(n);
        if(it == _contexts.end())return;

        ProcHandle &handle = it->second.proc;
        auto itd = _dead.find(handle);
        if(itd != _dead.end())_dead.erase(itd);

        DeleteFiber(handle);
        handle = nullptr;

#ifdef __linux__
        free(it->second.pstack);
        it->second.pstack = nullptr;
#endif
        _contexts.erase(it);
    }

	static Fiber &fiber() {
     	 _fibers_mutex.lock();

			ProcHandle handle = GetFiber();

			auto it = _fibers.find(handle);

			if (it == _fibers.end())
				throw std::runtime_error("Fiber proc does not exist");

			_fibers_mutex.unlock();

			return *it->second;

	}

};

#define FIBER_USER 20
