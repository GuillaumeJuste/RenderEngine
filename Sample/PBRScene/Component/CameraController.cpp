#include "CameraController.hpp"

#include "SceneGraph/Object/GameObject/GameObject.hpp"
#include "Transform/Transform.hpp"
#include "Mathlib/Mathlib/Include/Misc/Constants.hpp"

void CameraController::Initialize()
{

}

void CameraController::Start()
{
	if (window != nullptr)
	{
		window->GetCursorPos(&oldMouseX, &oldMouseY);
	}
}

void CameraController::Update(double _deltaTime)
{
}

void CameraController::FixedUpdate(double _deltaTime)
{
	if (window != nullptr)
	{
		float deltaTime = (float)_deltaTime;
		Mathlib::Transform gaoTransform = gameObject->GetLocalTransform();

		if (window->GetKeyPressed(262, 1))
			gaoTransform.position += gaoTransform.GetRightVector() * movementSpeed * deltaTime;
		if (window->GetKeyPressed(263, 1))
			gaoTransform.position -= gaoTransform.GetRightVector() * movementSpeed * deltaTime;
		if (window->GetKeyPressed(341, 1))
			gaoTransform.position -= gaoTransform.GetUpVector() * movementSpeed * deltaTime;
		if (window->GetKeyPressed(32, 1))
			gaoTransform.position += gaoTransform.GetUpVector() * movementSpeed * deltaTime;
		if (window->GetKeyPressed(264, 1))
			gaoTransform.position -= gaoTransform.GetForwardVector() * movementSpeed * deltaTime;
		if (window->GetKeyPressed(265, 1))
			gaoTransform.position += gaoTransform.GetForwardVector() * movementSpeed * deltaTime;

		double mouseX, mouseY;

		window->GetCursorPos(&mouseX, &mouseY);

		if (mouseX != oldMouseX || mouseY != oldMouseY)
		{
			dx += static_cast<float>(mouseX - oldMouseX) * deltaTime * rotationSpeed * Mathlib::Math::DegToRad;
			dy += static_cast<float>(mouseY - oldMouseY) * deltaTime * rotationSpeed * Mathlib::Math::DegToRad;

			oldMouseX = mouseX;
			oldMouseY = mouseY;

			dx = dx > Mathlib::Math::Pi ? dx - Mathlib::Math::Pi : dx < -Mathlib::Math::Pi ? dx + Mathlib::Math::Pi : dx;
			dy = dy > Mathlib::Math::Pi ? dy - Mathlib::Math::Pi : dy < -Mathlib::Math::Pi ? dy + Mathlib::Math::Pi : dy;

			gaoTransform.rotation = Mathlib::Quat(cos(dx), 0, sin(dx), 0) * Mathlib::Quat(cos(dy), sin(dy), 0, 0);
		}

		gameObject->SetLocalTransform(gaoTransform);
	}
}
