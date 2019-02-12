#pragma once
#define RUNNING_WINDOWS
#include <iostream>
#include <string>
#include <algorithm>
#include <filesystem>
#include <wchar.h>
namespace Utils {
	#ifdef RUNNING_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <Shlwapi.h>
   inline std::string GetWorkingDirectory() {
      return std::filesystem::current_path().string();
		}
	#elif defined RUNNING_LINUX
	#include <unistd.h>
		inline std::string GetWorkingDirectory() {
			char cwd[1024];
			if (getcwd(cwd, sizeof(cwd)) != nullptr) {
				return std::string(cwd) + std::string("/");
			}
			return "";
		}
	#endif
}