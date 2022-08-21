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

bool Object::GetIsHidden() const
{
	return isHidden;
}

void Object::SetName(std::string _name)
{
	name = _name;
}

void Object::SetIsHidden(bool _val)
{
	isHidden = _val;
}

bool Object::operator==(const Object& _rhs) const
{
	return uid == _rhs.uid;
}