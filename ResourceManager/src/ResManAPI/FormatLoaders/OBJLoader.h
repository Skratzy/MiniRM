#ifndef _RM_OBJ_LOADER_HPP
#define _RM_OBJ_LOADER_HPP

#include <vector>
#include <string>

#include "FormatLoader.h"

class OBJLoader : public MiniRM::FormatLoader {
private:


public:
	OBJLoader() {
		m_supportedExtensions.push_back(".obj");
	}
	virtual ~OBJLoader() {}

	virtual MiniRM::Resource* load(const char* path, const long GUID);
};

#endif //_RM_OBJ_LOADER_HPP