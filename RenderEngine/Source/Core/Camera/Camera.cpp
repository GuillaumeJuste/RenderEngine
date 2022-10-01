#include "Core/Camera/Camera.hpp"
#include "Matrix/Mat4.hpp"
#include "Misc/Math.hpp"

using namespace RenderEngine::Core;

Camera::Camera() :
	position{ Mathlib::Vec3(0.0f, 0.0f, -10.0f) }, lookAt{ Mathlib::Vec3(0.0f, 0.0f, 0.0f) }, up{ Mathlib::Vec3(0.0f, -1.0f, 0.0f) },
	fov{ 45.f }, near{ 0.1f }, far{ 100.f}
{

}

Mathlib::Mat4 Camera::GetProjectionMatrix(float _windowAspectRatio) const
{
	return Mathlib::Mat4::PerspectiveMatrix(Mathlib::COORDINATE_SYSTEM::RIGHT_HAND, Mathlib::Math::Radians(fov), _windowAspectRatio, near, far).Transpose();
}

Mathlib::Mat4 Camera::GetViewMatrix() const
{
	return Mathlib::Mat4::ViewMatrix(Mathlib::COORDINATE_SYSTEM::RIGHT_HAND, position, lookAt, up).Transpose();
}