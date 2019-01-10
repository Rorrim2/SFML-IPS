#include "ResourceManager.h"
#include <fstream> 
#include "Utilities.h"

/*
template<typename Derived, typename T>
T * ResourceManager<Derived, T>::GetResource(const std::string & l_id)
{
		auto res = Find(l_id);
		return(res ? res->first : nullptr);
}

template<typename Derived, typename T>
std::string ResourceManager<Derived, T>::GetPath(const std::string & l_id)
{
		auto path = m_paths.find(l_id);
		return(path != m_paths.end() ? path->second : "");
}

template<typename Derived, typename T>
bool ResourceManager<Derived, T>::RequireResource(const std::string& l_id) {
	auto res = Find(l_id);
	if (res) {
		++res->second;
		return true;
	}
	auto path = m_paths.find(l_id);
	if (path == m_paths.end()) { return false; }
	T* resource = Load(path->second);
	if (!resource) { return false; }
	m_resources.emplace(l_id, std::make_pair(resource, 1));
	return true;
}

template<typename Derived, typename T>
bool ResourceManager<Derived, T>::ReleaseResource(const std::string& l_id) {
	auto res = Find(l_id);
	if (!res) { return false; }
	--res->second;
	if (!res->second) { Unload(l_id); }
	return true;
}

template<typename Derived, typename T>
void ResourceManager<Derived, T>::PurgeResources() {
	while (m_resources.begin() != m_resources.end()) {
		delete m_resources.begin()->second.first;
		m_resources.erase(m_resources.begin());
	}
}

template<typename Derived, typename T>
T* ResourceManager<Derived, T>::Load(const std::string& l_path) {
	return static_cast<Derived*>(this)->Load(l_path);
}

template<typename Derived, typename T>
std::pair<T*, unsigned int>* ResourceManager<Derived, T>::Find(const std::string& l_id) {
	auto itr = m_resources.find(l_id);
	return (itr != m_resources.end() ? &itr->second : nullptr);
}

template<typename Derived, typename T>
bool ResourceManager<Derived, T>::Unload(const std::string& l_id) {
	auto itr = m_resources.find(l_id);
	if (itr == m_resources.end()) { return false; }
	delete itr->second.first;
	m_resources.erase(itr);
	return true;
}

template<typename Derived, typename T>
void ResourceManager<Derived, T>::LoadPaths(const std::string& l_pathFile) {
	std::ifstream paths;
	paths.open(Utils::GetWorkingDirectory() + l_pathFile);
	if (paths.is_open()) {
		std::string line;
		while (std::getline(paths, line)) {
			std::stringstream keystream(line);
			std::string pathName;
			std::string path;
			keystream >> pathName;
			keystream >> path;
			m_paths.emplace(pathName, path);
		}
		paths.close();
		return;
	}
	std::cerr <<
		"! Failed loading the path file: "
		<< l_pathFile << std::endl;
}*/