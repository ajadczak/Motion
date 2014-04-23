#pragma once
#include "Vertex.h"
#include <vector>
#include <algorithm>

class Model
{
private:
	std::vector<Vertex> _vertices;
	std::vector<long> _indices;

public:
	std::string Name;
	Model::Model(std::vector<Vertex> &vertices, std::vector<long> &indices, std::string name);
	~Model();
};

