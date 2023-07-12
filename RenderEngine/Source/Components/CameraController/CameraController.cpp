#include "Components/CameraController/CameraController.hpp"

#include "SceneGraph/Object/GameObject/GameObject.hpp"
#include "Transform/Transform.hpp"
#include "Mathlib/Mathlib/Include/Collections/Mathlib.hpp"

using namespace RenderEngine::Utils;
using namespace RenderEngine::Component;

void CameraController::Initialize()
{

}

void CameraController::Start()
{
	if (window != nullptr)
	{
		window->GetCursorPos(&oldMouseX, &oldMouseY);

		Mathlib::Vec3 eulerRotation = gameObject->GetLocalTransform().rotation.Euler();
		dx = eulerRotation.X;
		dy = eulerRotation.Y;
	}
}

void CameraController::Update(double _deltaTime)
{
}

void CameraController::FixedUpdate(double _deltaTime)
{
	if (window != nullptr)
	{
		float deltaTime = (float) _deltaTime;
		Mathlib::Transform gaoTransform = gameObject->GetLocalTransform();

		if (window->CheckKeyStatus(Input::KEY_RIGHT, InputStatus::PRESS))
			gaoTransform.position += gaoTransform.GetRightVector() * movementSpeed * deltaTime;
		if (window->CheckKeyStatus(Input::KEY_LEFT, InputStatus::PRESS))
			gaoTransform.position -= gaoTransform.GetRightVector() * movementSpeed * deltaTime;
		if (window->CheckKeyStatus(Input::KEY_LEFT_CONTROL, InputStatus::PRESS))
			gaoTransform.position -= gaoTransform.GetUpVector() * movementSpeed * deltaTime;
		if (window->CheckKeyStatus(Input::KEY_SPACE, InputStatus::PRESS))
			gaoTransform.position += gaoTransform.GetUpVector() * movementSpeed * deltaTime;
		if (window->CheckKeyStatus(Input::KEY_DOWN, InputStatus::PRESS))
			gaoTransform.position -= gaoTransform.GetForwardVector() * movementSpeed * deltaTime;
		if (window->CheckKeyStatus(Input::KEY_UP, InputStatus::PRESS))
			gaoTransform.position += gaoTransform.GetForwardVector() * movementSpeed * deltaTime;

		double mouseX, mouseY;

		window->GetCursorPos(&mouseX, &mouseY);

		if (mouseX != oldMouseX || mouseY != oldMouseY)
		{
			dx += static_cast<float>(mouseY - oldMouseY) * deltaTime * rotationSpeed;
			dy += static_cast<float>(mouseX - oldMouseX) * deltaTime * rotationSpeed;

			oldMouseX = mouseX;
			oldMouseY = mouseY;

			gaoTransform.rotation = Mathlib::Quat::FromEuler(Mathlib::Vec3(dx, dy, 0.f));
		}

		gameObject->SetLocalTransform(gaoTransform);

		if (window->CheckKeyStatus(Input::KEY_F1, InputStatus::PRESS) && !noUpdate)
		{
			enableMouse = !enableMouse;
			window->LockMouseToWindow(enableMouse);
			noUpdate = true;
		}
		if (window->CheckKeyStatus(Input::KEY_F1, InputStatus::RELEASE) && noUpdate)
		{
			noUpdate = false;
		}
	}
}
