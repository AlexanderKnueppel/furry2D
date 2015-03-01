#ifndef __FURRY_CORE_ENGINE_H
#define __FURRY_CORE_ENGINE_H

/*
* ****************************************
*
* This file is part of Furry2D, a simple gameframework for 2D desktop games.
*
* Copyright (c) 2015 Furry2D. All rights reserved.
*
* For the full copyright and license information, please view the LICENSE.txt
* file that was distributed with this source code.
*
* \Author Alexander Knueppel
*
* ****************************************
*/

#include <memory>
#include <vector>
#include <map>

FURRY_NS_BEGIN

class TaskProcessor;
class FURRY_API Engine {
public:
	typedef std::unique_ptr<System> SystemPtr;
	typedef std::vector<SystemPtr> SystemList;
	typedef std::map<std::string, System*> SystemMapping;

	Engine();
	~Engine();

	void setApplication(Application* application); // the Engine will take ownership of the application

	void add(System* system); //note that the Engine will take ownership of the system added

	template <typename T>
	T* get() const;

	void remove(std::string systemName);
	void remove(System* system);

	void run();
	void stop();

	void updateSystem(System* system, bool repeating = false, bool background = false);

	// Signals
	struct OnStop {};

	// Handlers
	void operator()(const OnStop&);

private:
	void setApplication(std::unique_ptr<Application>&& application);
	void add(std::unique_ptr<System>&& system);

	void initializeSystems();
	void shutdownSystems();

	bool initializeDependencies();
	void shutdownDependencies();

	SystemList systems_;
	SystemMapping system_lookup_;
	TaskProcessor task_processor_;
	Config mConfig;

	std::unique_ptr<Application> application_;
};

// Note -- if systems are removed regularly, from non-main threads this needs to become threadsafe as well
//		   I don't expect this to be the typical use case, so I'll leave it for later


template <typename T>
T* Engine::get() const {
	T* result = nullptr;

	for (const auto& system : mSystems) {
		result = dynamic_cast<T*>(system.get());

		if (result)
			return result;
	}

	return nullptr;
}

FURRY_NS_END

#endif