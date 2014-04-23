#include "stdafx.h"
#include "ObjectManager.h"
using namespace std;

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
	objects_.clear();
}

void ObjectManager::AddObject(Model &m)
{
	//objects_.insert(m.Name, m);
	//objects_.emplace(m.Name, m);
}

void ObjectManager::AddObject(vector<Vertex> vertices, vector<long> indices, string name)
{
	//test_.emplace(name, name);
	//auto pair = std::make_pair(name, vertices, indices, name);
	//objects_.emplace(name, vertices, indices, name);
}

unordered_map<string, Model> ObjectManager::GetObjects() const
{
	return objects_;
}

// should switch to unordered_map so we don't need to traverse whole list to get key
Model ObjectManager::GetObject(std::string &name)
{
	return objects_.at(name);
}