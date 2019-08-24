#ifndef MESH_RESOURCE_HPP
#define MESH_RESOURCE_HPP

#include <vector>
#include "Resource.h"
#include "../../Defines.h"

class MeshResource : public Resource {
private:
	float* m_vertices;
	unsigned int* m_indices;
	unsigned int m_vertexCount;
	unsigned int m_indexCount;

public:
	MeshResource(float* vertices, uint32_t* indices, unsigned int numVertices, unsigned int numIndices, const long GUID);
	virtual ~MeshResource();

	std::vector<unsigned int> indices;

	const float* getVertices() const;
	const unsigned int* getIndices() const;

	const unsigned int getVertexCount() const;
	const unsigned int getIndexCount() const;
};

#endif
