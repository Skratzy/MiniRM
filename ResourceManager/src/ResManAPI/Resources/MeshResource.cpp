#include "MeshResource.h"

MeshResource::MeshResource(float* vertices, uint32_t* indices, unsigned int numVertices, unsigned int numIndices, const long GUID)
	: Resource(GUID)
{
	m_vertexCount = numVertices;
	m_vertices = vertices;

	m_indexCount = numIndices;
	m_indices = indices;
}

MeshResource::~MeshResource()
{
	RM_FREE(m_vertices);
	RM_FREE(m_indices);
}

const float* MeshResource::getVertices() const {
	return m_vertices;
}

const unsigned int* MeshResource::getIndices() const {
	return m_indices;
}

const unsigned int MeshResource::getVertexCount() const
{
	return m_vertexCount;
}

const unsigned int MeshResource::getIndexCount() const
{
	return m_indexCount;
}
