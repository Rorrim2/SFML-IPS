#pragma once
#define RUNNING_WINDOWS
#include <iostream>
#include <string>
#include <algorithm>
#include <wchar.h>
namespace Utils {
	#ifdef RUNNING_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <Shlwapi.h>
   inline std::string GetWorkingDirectory() {
         HMODULE hModule = GetModuleHandle(nullptr);
         if (hModule) {
            wchar_t path[256];
            GetModuleFileName(hModule, path, sizeof(path));
            PathRemoveFileSpec(path);
            wcscat_s(path, L"\\"); // new
            std::wstring wp(path);
            return std::string(wp.begin(), wp.end()); // new
         }
      return "";
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