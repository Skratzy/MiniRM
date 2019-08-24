#include <thread>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <atomic>
#include <crtdbg.h>
#include <experimental/filesystem>

#include "Defines.h"

#include "ResManAPI/ResourceManager.h"
#include "ResManAPI/FormatLoaders/PNGLoader.h"
#include "ResManAPI/FormatLoaders/OBJLoader.h"

#include "ResManAPI/Resources/Resource.h"
#include "ResManAPI/Resources/MeshResource.h"
#include "ResManAPI/Resources/TextureResource.h"


class Model {
public:
	MeshResource* m_mesh = nullptr;
	TextureResource* m_tex = nullptr;

	Model() {

	}

	~Model() {
		if (m_mesh)
			ResourceManager::getInstance().decrementReference(m_mesh->getGUID());
		if (m_tex)
			ResourceManager::getInstance().decrementReference(m_tex->getGUID());
	}

	void setMeshCallback(Resource* mesh) {
		//std::lock_guard<std::mutex> lock(tempMutex);
		if (m_mesh != nullptr)
			ResourceManager::getInstance().decrementReference(m_mesh->getGUID());
		m_mesh = reinterpret_cast<MeshResource*>(mesh);
		RM_DEBUG_MESSAGE("Set a new mesh -- " + std::string(m_mesh->getPath()), 0);
	}

	void setTextureCallback(Resource* tex) {
		// TODO: Update size
		if (m_tex != nullptr)
			ResourceManager::getInstance().decrementReference(m_tex->getGUID());
		m_tex = reinterpret_cast<TextureResource*>(tex);
		RM_DEBUG_MESSAGE("Set a new texture -- " + std::string(m_tex->getPath()), 0);
	}
};

struct ResourceData {
	std::vector<Model*> models;
	std::string tempMeshPath;
	std::string tempTexPath;
};

ResourceData initResMngr() {
	ResourceData rd;
	ResourceManager &rm = ResourceManager::getInstance();
	rm.init(1024, 1024 * 1024 * 1024);

	// Roughly 25 bytes per loader (vector with strings of supported formats per formatloader)
	// Gets deleted in the resource manager
	rm.registerFormatLoader(new PNGLoader);
	rm.registerFormatLoader(new OBJLoader);

	rd.models.push_back(new Model());
	rd.models.push_back(new Model());
	rd.models.push_back(new Model());
	rd.models.push_back(new Model());

	// Load the async temp mesh and texture
	rd.tempMeshPath = "Assets/meshes/tempOBJRes.obj";
	auto tempMesh = ResourceManager::getInstance().load("Assets/meshes/tempOBJRes.obj");
	rd.tempTexPath = "Assets/textures/tempJPGRes.jpg";
	auto tempTex = ResourceManager::getInstance().load("Assets/textures/tempPNGRes.png");

	return rd;
}







int main() {
	// Check for memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Initialize the resource manager and register the format loaders to it and create variables for different tests.
	ResourceData resourceData = initResMngr();

	ResourceManager &rm = ResourceManager::getInstance();

	bool keepRunning = true;


	auto duration = std::chrono::seconds(10);

	auto startTime1 = std::chrono::high_resolution_clock::now() - duration;
	auto startTime2 = startTime1 + std::chrono::seconds(2);
	std::vector<ResourceManager::AsyncJobIndex> activeJobs;


	auto end = std::chrono::high_resolution_clock::now() + std::chrono::seconds(60);
	while (end > std::chrono::high_resolution_clock::now()) {
		auto resources = rm.getResources();
		std::string resString = "";
		for (auto res : resources) {
			resString.append("GUID: ");
			resString.append(std::to_string(res.second->getGUID()));
			resString.append("  Path: ");
			resString.append(res.second->getPath());
			resString.append("\n");
		}
		if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime1) > duration) {

			RM_DEBUG_MESSAGE("----------CLEARING----------", 0);

			rm.clearResourceManager();
			activeJobs.clear();

			for (auto m : resourceData.models) {
				if (m->m_mesh != nullptr)
					m->m_mesh->derefer();
				if (m->m_tex != nullptr)
					m->m_tex->derefer();
			}
			resourceData.models.clear();

			resourceData.models.push_back(new Model());
			resourceData.models.push_back(new Model());
			resourceData.models.push_back(new Model());
			resourceData.models.push_back(new Model());


			RM_DEBUG_MESSAGE("----------DONE CLEARING----------", 0);

			startTime1 = std::chrono::high_resolution_clock::now();
		}

		if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime2) > duration) {

			RM_DEBUG_MESSAGE("----------INIT----------", 0);

			// Async jobs creation
			// Uses a path and a callback function to a model
			activeJobs.push_back(rm.asyncLoad("Assets/meshes/cow-nonormals.obj", std::bind(&Model::setMeshCallback, resourceData.models[0], std::placeholders::_1)));
			activeJobs.push_back(rm.asyncLoad("Assets/textures/testfile.png", std::bind(&Model::setTextureCallback, resourceData.models[0], std::placeholders::_1)));

			// Example of how to load an asset from a zip-file
			//activeJobs.push_back(rm.asyncLoad("Assets/AssetsPackage.zip/AssetsPackage/meshes/cow-nonormals.rmmesh", std::bind(setMeshCallback, resourceData.models[1], std::placeholders::_1)));
			activeJobs.push_back(rm.asyncLoad("Assets/meshes/cow-normals.obj", std::bind(&Model::setMeshCallback, resourceData.models[1], std::placeholders::_1)));
			activeJobs.push_back(rm.asyncLoad("Assets/textures/testfile.png", std::bind(&Model::setTextureCallback, resourceData.models[1], std::placeholders::_1)));

			activeJobs.push_back(rm.asyncLoad("Assets/meshes/cow-nonormals.obj", std::bind(&Model::setMeshCallback, resourceData.models[2], std::placeholders::_1)));
			activeJobs.push_back(rm.asyncLoad("Assets/textures/testfile.png", std::bind(&Model::setTextureCallback, resourceData.models[2], std::placeholders::_1)));

			activeJobs.push_back(rm.asyncLoad("Assets/meshes/cow-normals.obj", std::bind(&Model::setMeshCallback, resourceData.models[3], std::placeholders::_1)));

			RM_DEBUG_MESSAGE("----------DONE INIT----------", 0);

			startTime2 = std::chrono::high_resolution_clock::now();
		}
	}

	for (auto job : activeJobs)
		rm.removeAsyncJob(job);

	for (auto m : resourceData.models) {
		m->~Model();
		free(m);
	}

	rm.cleanup();

	/*
		END OF SOKOL RENDERING
	*/

	keepRunning = false;

	return 0;
}