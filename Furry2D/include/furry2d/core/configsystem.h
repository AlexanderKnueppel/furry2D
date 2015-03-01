#ifndef __FURRY_CORE_CONFIGSYSTEM_H__
#define __FURRY_CORE_CONFIGSYSTEM_H__

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

#include <furry2d/core/ConfigFile.h>
#include <memory>

FURRY_NS_BEGIN

class Config : System {
public:
	Config() : System("Config"){
		config_file_ = std::unique_ptr<ConfigFile>(new ConfigFile());
	}

	virtual ~Config() { }

	bool load(const std::string &filename) {
		try {
			config_file_->addConfigFile(filename);
		}
		catch (const std::string str) {
			gLogDebug << "Failed to open config file: " << filename;
			return false;
		}
		return true;
	}

	void parseCommandLine(int argc, char* argv[]) {
		/* TODO */
	}

	//get sprecified variable
	template <typename T>
	T get(std::string const& section, std::string const& entry) const {
		try {
			ConfigFile::Conversion cv = mConfigFile->Value(section, entry);
			return (T)cv;
		}
		catch (const char *str) {
			gLogDebug << "Failed to find variable: " << section << "/" << entry; 
			return T();
		}
	}

	//get specified variable with default value
	template <typename T>
	T get(std::string const& section, std::string const& entry, const T value) const {
		ConfigFile::Conversion cv = mConfigFile->Value(section, entry, value);
		return (T)cv;
	}

	//spceial case for const char*
	std::string get(std::string const& section, std::string const& entry, const char *value) const {
		std::string strValue(value);
		ConfigFile::Conversion cv = config_file_->Value(section, entry, strValue);
		return (std::string)cv;
	}

private:
	std::unique_ptr<ConfigFile> config_file_;
};

FURRY_NS_END

#endif