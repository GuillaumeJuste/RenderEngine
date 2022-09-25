#include "Core/Object/Object.hpp"
#include <time.h>

using namespace RenderEngine::Core;

unsigned int Object::nextId = 0;

Object::Object()
{
	uid = nextId;
	nextId++;
}

unsigned int Object::GetUId() const
{
	return uid;
}

std::string Object::GetName() const
{
	return name;
}

bool Object::GetIsEnabled() const
{
	return enable;
}

void Object::SetName(std::string _name)
{
	name = _name;
}

void Object::SetIsEnabled(bool _val)
{
	enable = _val;
}

bool Object::operator==(const Object& _rhs) const
{
	return uid == _rhs.uid;
}