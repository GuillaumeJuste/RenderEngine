#include "RotatorComponent.hpp"
#include "SceneGraph/Object/GameObject/GameObject.hpp"
#include "Transform/Transform.hpp"


void RotatorComponent::Initialize()
{

}

void RotatorComponent::Start()
{
	Mathlib::Quat initialRotation = gameObject->GetLocalTransform().rotation;
	switch (rotationAxis)
	{
	case ROTATION_AXIS::X:
		currentRotation = initialRotation.Euler().X;
		break;
	case ROTATION_AXIS::Y:
		currentRotation = initialRotation.Euler().Y;
		break;
	case ROTATION_AXIS::Z:
		currentRotation = initialRotation.Euler().Z;
		break;
	default:
		break;
	}
}

void RotatorComponent::Update()
{
	Mathlib::Transform transform = gameObject->GetLocalTransform();

	switch (rotationAxis)
	{
	case ROTATION_AXIS::X:
		transform.rotation = Mathlib::Quat::FromEuler(Mathlib::Vec3(currentRotation, 0.f,0.f));
		break;
	case ROTATION_AXIS::Y:
		transform.rotation = Mathlib::Quat::FromEuler(Mathlib::Vec3(0.f, currentRotation, 0.f));
		break;
	case ROTATION_AXIS::Z:
		transform.rotation = Mathlib::Quat::FromEuler(Mathlib::Vec3(0.f, 0.f, currentRotation));
		break;
	default:
		break;
	}

	currentRotation += 0.01f;
	if (currentRotation >= 360.f)
		currentRotation -= 360.f;

	gameObject->SetLocalTransform(transform);
}