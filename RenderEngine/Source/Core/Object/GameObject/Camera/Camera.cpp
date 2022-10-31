#include "Core/Object/GameObject/Camera/Camera.hpp"
#include "Matrix/Mat4.hpp"
#include "Misc/Math.hpp"

using namespace RenderEngine::Core;

Camera::Camera() :
	fov{ 45.f }, near{ 0.1f }, far{ 100.f}
{

}

Mathlib::Mat4 Camera::GetProjectionMatrix(float _windowAspectRatio) const
{
	return Mathlib::Mat4::PerspectiveMatrix(Mathlib::COORDINATE_SYSTEM::LEFT_HAND, Mathlib::Math::Radians(fov), _windowAspectRatio, near, far).Transpose();
}

Mathlib::Mat4 Camera::GetViewMatrix() const
{
	Mathlib::Transform tmp = GetWorldTransform();
	
	return Mathlib::Mat4::ViewMatrix(Mathlib::COORDINATE_SYSTEM::LEFT_HAND, tmp.position, tmp.GetForwardVector(), tmp.GetUpVector()).Transpose();
}