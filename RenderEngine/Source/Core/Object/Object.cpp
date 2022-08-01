#include "Core/Object/Object.hpp"
#include <time.h>

using namespace RenderEngine::Core;

unsigned int Object::nextId = 0;

Object::Object()
{
	uid = nextId;
	nextId++;
}

unsigned int Object::GetUId()
{
	return uid;
}

std::string Object::GetName()
{
	return name;
}

void Object::SetName(std::string _name)
{
	name = _name;
}

bool Object::operator==(const Object& _rhs)
{
	return uid == _rhs.uid;
}