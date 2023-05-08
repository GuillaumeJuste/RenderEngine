#include "SceneGraph/Components/Transfom/Transform.hpp"

using namespace RenderEngine::SceneGraph;

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

void Transform::SetLocalTransform(const Mathlib::Transform& _transform)
{
	transform = _transform;
}