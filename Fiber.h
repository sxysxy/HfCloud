/***************************
	class Fiber (for win32)
	author: Rio Shiina
	E-mail: 390855044@qq.com
****************************/

#pragma once

#include <Windows.h>

#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <stack>
#include <mutex>

enum class FiberStatus {

	Running,
	HungUp,
	Dead,

};

class Fiber {

	struct Context {

		Fiber *self;

		PVOID proc;

		std::function<void()> functor;

	};

	inline static VOID WINAPI FiberProc(PVOID pvParam) {

		Context &context = *reinterpret_cast<Context*>(pvParam);

		if (context.functor)
			context.functor();

		context.self->_dead.insert(context.proc);

		context.self->yield();

	}

	static std::unordered_map<PVOID, Fiber*> _fibers;

	static std::mutex mutex;

	std::unordered_map<unsigned, Context> _contexts;

	std::stack<PVOID> _handleStack;

	std::unordered_set<PVOID> _dead;

	PVOID _handle;

public:

	Fiber() {}

	Fiber(const Fiber &fiber) {

		for (auto &i : fiber._contexts)
			_contexts.insert(std::make_pair(i.first, Context{ this, nullptr, i.second.functor }));

	}

	Fiber(Fiber &&fiber) {

		for (auto &i : fiber._contexts) {

			for (auto &i : fiber._contexts)
				_contexts.insert(std::make_pair(i.first, Context{ this, nullptr, std::move(i.second.functor) }));

		}

	}

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

		mutex.lock();

		_fibers.insert(std::make_pair(_handle, this));

		mutex.unlock();

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
			it = _contexts.insert(std::make_pair(n, Context{ this, nullptr })).first;

		if (it->second.proc)
			throw std::runtime_error("Fiber proc is running.");

		return it->second.functor;

	}

	void run(unsigned n) {

		_handle = ::ConvertThreadToFiber(nullptr);

		resume(n);

		for (auto &i : _contexts) {

			PVOID &handle = i.second.proc;

			if (handle) {

				mutex.lock();

				_fibers.erase(handle);

				::DeleteFiber(handle);

				mutex.unlock();

				handle = nullptr;

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

	static Fiber &fiber() {

		PVOID handle = ::GetCurrentFiber();

		mutex.lock();

		auto it = _fibers.find(handle);

		if (it == _fibers.end())
			throw std::runtime_error("Fiber proc does not exist");

		mutex.unlock();

		return *it->second;

	}

};
