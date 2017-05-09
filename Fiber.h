/*
  Thanks to JazzLeee https://github.com/JazzLeee
  Fiber
*/
#ifndef _FIBER_H
#define _FIBER_H

#include <Windows.h>

#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <stack>

enum class FiberStatus {

	Running,
	HungUp,
	Dead,

};

class Fiber {

	struct FiberProcContext {

		Fiber *self;

		PVOID proc;

		std::function<void()> functor;

	};

	inline static VOID WINAPI FiberProc(PVOID pvParam) {

		FiberProcContext &context = *reinterpret_cast<FiberProcContext*>(pvParam);

		if (context.functor)
			context.functor();

		context.self->_dead.insert(context.proc);

		context.self->yield();

	}

	std::unordered_map<unsigned, FiberProcContext> _contexts;

	std::stack<PVOID> _handleStack;

	std::unordered_set<PVOID> _dead;

	PVOID _handle;

public:

	Fiber() {}

	Fiber(const Fiber&) = delete;

	Fiber(Fiber&&) = delete;

	void resume(unsigned n) {

		auto it = _contexts.find(n);

		if (it == _contexts.end())
			throw std::runtime_error("Fiber proc does not exist.");

		if (!it->second.proc) {

			PVOID ret = ::CreateFiberEx(
				0,
				0,
				FIBER_FLAG_FLOAT_SWITCH,
				FiberProc,
				&it->second
			);

			if (ret)
				it->second.proc = ret;
			else
				throw std::runtime_error("Failed to create fiber proc");

		}

		if (_dead.find(it->second.proc) != _dead.end())
			throw std::runtime_error("Fiber proc is dead.");

		_handleStack.push(_handle);

		_handle = it->second.proc;

		::SwitchToFiber(_handle);

	}

	void yield() {

		_handle = _handleStack.top();

		_handleStack.pop();

		SwitchToFiber(_handle);

	}

	std::function<void()> &operator[](unsigned n) {

		auto it = _contexts.find(n);

		if (it == _contexts.end())
			it = _contexts.insert(std::make_pair(n, FiberProcContext{ this, nullptr })).first;

		if (it->second.proc)
			throw std::runtime_error("Fiber proc is running.");

		return it->second.functor;

	}

	void run(unsigned n) {

		_handle = ::ConvertThreadToFiber(nullptr);

		resume(n);

		for (auto &i : _contexts) {

			if (i.second.proc) {

				::DeleteFiber(i.second.proc);

				i.second.proc = nullptr;

			}

		}

		_dead.clear();

		::ConvertFiberToThread();

	}

	FiberStatus status(unsigned n) const {

		auto it = _contexts.find(n);

		if (it == _contexts.end())
			throw std::runtime_error("Fiber proc does not exist.");

		PVOID handle = it->second.proc;

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

		PVOID handle = it->second.proc;

		if (handle)
			_dead.insert(handle);
		else
			throw std::runtime_error("Fiber proc did not run.");

	}

};

#endif
