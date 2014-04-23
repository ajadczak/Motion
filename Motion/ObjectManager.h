#pragma once
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <unordered_map>
#include "Model.h"

class ObjectManager
{
	std::unordered_map<std::string, Model> objects_;
	std::unordered_map<std::string, std::string> test_;

public:
	ObjectManager();
	~ObjectManager();
	void AddObject(Model &m);
	void AddObject(std::vector<Vertex> vertices, std::vector<long> indices, std::string name);
	std::unordered_map<std::string, Model> GetObjects() const;
	Model GetObject(std::string &name);
};

