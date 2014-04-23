#include "stdafx.h"
#include "Model.h"
using namespace std;

Model::Model(vector<Vertex> &vertices, vector<long> &indices, string name)
{
	_vertices = vertices;
	_indices = indices;
	Name = name;
}

Model::~Model()
{
	_vertices.clear();
	_indices.clear();
}