#pragma once
#include <string>
#include <unordered_map>


template<typename Derived, typename T>
class ResourceManager {
public:
	ResourceManager(const std::string& l_pathsFile) {
		LoadPaths(l_pathsFile);
	};
	virtual ~ResourceManager() { PurgeResources(); }
	T* GetResource(const std::string& l_id);
	std::string GetPath(const std::string& l_id);
	bool RequireResource(const std::string& l_id);
	bool ReleaseResource(const std::string& l_id);
	void PurgeResources();
	T* Load(const std::string& l_path);
	std::pair<T*, unsigned int>* Find(const std::string& l_id);
	bool Unload(const std::string& l_id);
	void LoadPaths(const std::string& l_pathFile);
private:
	std::unordered_map<std::string,
		std::pair<T*, unsigned int>> m_resources;
	std::unordered_map<std::string, std::string> m_paths;
};
