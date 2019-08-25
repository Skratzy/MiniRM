#include "PNGLoader.h"
#include "../Resources/TextureResource.h"

#include "ext/LodePNG.h"
#include <experimental/filesystem>

MiniRM::Resource* PNGLoader::load(const char* path, const long GUID) {
	std::vector<unsigned char> image;
	unsigned int width;
	unsigned int height;
	MiniRM::Resource* resource = nullptr;

	std::string filePath = path;
	size_t check = 0;
	check = filePath.find(".zip");
	bool loadZipped = false;
	if (check < filePath.length()) {
		loadZipped = true;
		filePath = extractFile(path, check);
	}

	// 'decode' both checks for error and assigns values to 'image', 'width, and 'height'
	if (unsigned int error = lodepng::decode(image, width, height, filePath.c_str())) {
		RM_DEBUG_MESSAGE(std::to_string(error) + lodepng_error_text(error) + " at filePath: " + path, 1);
	}
	else {

		unsigned char* image_data = RM_PLACEMENT_NEW(image.size() * sizeof(unsigned char), unsigned char);
		memcpy_s(image_data, image.size() * sizeof(unsigned char), image.data(), image.size() * sizeof(unsigned char));

		// Fix size (VRAM vs RAM)
		unsigned int size = sizeof(TextureResource);// +sizeof(unsigned int) * image.size();
		// Attach the formatted image to a textureresource
		resource = RM_PLACEMENT_NEW(size, TextureResource(width, height, image_data, GUID));
		// Size on DRAM
		resource->setSizeCPU(size);
		// Size on VRAM
		resource->setSizeGPU(width * height * sizeof(unsigned char) * 4);
	}

	if (loadZipped) {
		// Deleting extracted file once loaded in to memory
		std::experimental::filesystem::remove(filePath.c_str());
	}

	return resource;
}
