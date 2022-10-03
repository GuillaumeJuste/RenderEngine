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

bool Object::operator==(const Object& _rhs) const
{
	return uid == _rhs.uid;
}