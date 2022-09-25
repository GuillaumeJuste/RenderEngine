#include "Core/Object/Components/Transfom/Transform.hpp"

using namespace RenderEngine::Core;

Transform::Transform(Mathlib::Transform _transform) :
	transform { _transform}
{
}

void Transform::Initialize()
{
}

void Transform::Start()
{
}

void Transform::Update()
{
}

const Mathlib::Transform& Transform::GetLocalTransform() const
{
	return transform;
}