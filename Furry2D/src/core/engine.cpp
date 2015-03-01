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

#include <furry2d/furry2d.h>
#include <cassert>

namespace {
	void glfwErrorCallback(int error, const char* description) {
		gLogError << "GLFW error (" << error << "): " << description;
	}
}

FURRY_NS_BEGIN

Engine::Engine() {
	Channel::add<OnStop>(this);
}

Engine::~Engine() {
	stop();
}

void Engine::setApplication(Application* application) {
	setApplication(std::unique_ptr<Application>(application));
}

void Engine::setApplication(std::unique_ptr<Application>&& application) {
	application_ = std::move(application);

	if (application_) {
		application_->engine_ = this;
		gLogDebug << "Application set: " << application_->getName();
	}
}

void Engine::add(System* system) {
	add(std::unique_ptr<System>(system));
}

void Engine::add(std::unique_ptr<System>&& system) {
	auto it = system_lookup_.find(system->getName());

	if (it == system_lookup_.end()) {
		system->engine_ = this;

		system_lookup_[system->getName()] = system.get();
		systems_.emplace_back(std::move(system));
	}
	else
		throw std::runtime_error(std::string("Duplicate system added: ") + system->getName());
}

void Engine::remove(std::string systemName) {
	System* s = nullptr;

	auto it = system_lookup_.find(systemName);

	if (it != system_lookup_.end())
		s = it->second;

	if (s)
		remove(s);
	else {
		gLogError << "Cannot remove system '" << systemName << "', not currently registered";
	}
				
}

void Engine::remove(System* s) {
	// first scan the systemlist for the given system
	auto it = std::remove_if(
		systems_.begin(),
		systems_.end(),
		[s](const SystemPtr& ptr) {
			return ptr.get() == s;
		}
	);

	if (it != systems_.end()) {
		// the system was found, perform a clean shutdown and remove it from the active registry/list
		s->shutdown();

		systems_.erase(it);

		for (auto jt = system_lookup_.begin(); jt != system_lookup_.end(); ++jt) {
			if (jt->second == s) {
				system_lookup_.erase(jt);
				break;
			}
		}
	}
}

void Engine::run() {
	if (initializeDependencies()) {
		initializeSystems();
		task_processor_.start();
		shutdownSystems();
		shutdownDependencies();

		// if we want to use Qt later on (for gui), the main loop should look as the following (thread issues with Qt).
		// just leave this here as a side note

		/*initializeSystems();
		auto t = std::thread([&] { //start an extra thread for scheduling
			task_processor_.start();
		});
					
		qApp->exec(); //has to be in the main thread...
		t.join();
		shutdownSystems();
		shutdownDependencies();*/
	}
}

void Engine::stop() {
	task_processor_.stop();
}

void Engine::initializeSystems() {
	// try and load a config file
	//mConfig.load("default.cfg");

	// now perform per-system initialization
	for (auto& system : systems_) {
		if (!system->initialize())
			gLogError << "Failed to initialize subsystem: " << system->getName();
	}

	// if an application was set, initialize it as well
	if (application_)
		if (!application_->initialize())
			gLogError << "Failed to initialize application: " << application_->getName();
}

void Engine::shutdownSystems() {
	if (application_)
		application_->shutdown();

	for (auto& system : systems_)
		system->shutdown();

	systems_.clear();
	system_lookup_.clear();
}

bool Engine::initializeDependencies() {
	//glfwSetErrorCallback(&glfwErrorCallback);

	//if (!glfwInit())
	//	return false;

	// Leave it to the application
	return application_->initializeDependencies();
}

void Engine::shutdownDependencies() {
	//glfwTerminate();

	// Leave it to the application
	application_->shutdownDependencies();
}

void Engine::updateSystem(System* system, bool repeating, bool background) {
	assert(system);

	task_processor_.addWork(
		[system] { 
			system->update(); 
		}, 
		repeating, 
		background
	);
}

void Engine::operator()(const OnStop&) {
	stop();
}

FURRY_NS_END