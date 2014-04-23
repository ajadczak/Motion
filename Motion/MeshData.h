#pragma once
#include <vector>
#include "Vertex.h"

struct MeshData
{
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;
};