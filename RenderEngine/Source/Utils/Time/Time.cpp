#include "Utils/Time/Time.hpp"
#include "Mathlib/Mathlib/Include/Misc/Math.hpp"

#include <iostream>
#include <string>

using namespace RenderEngine::Utils;
using ms = std::chrono::duration<float, std::milli>;

Time::Time(const float& _fixedTimestep, const float& _timescale) :
	fixedTimeStep{ _fixedTimestep }, timeScale{ _timescale}
{
}

void Time::Start()
{
	startTime = timer.now();;
	currentTime = startTime;
}

void Time::Update()
{
	std::chrono::time_point<std::chrono::steady_clock> newTime = timer.now();

	deltaTime = deltaTime = std::chrono::duration_cast<ms>(newTime - currentTime).count() / 1000.0 * (double)timeScale;
	fixedUpdateTime += (float)deltaTime;
	if (fixedUpdateTime >= fixedTimeStep)
	{
		int stepCount = (int)(fixedUpdateTime / fixedTimeStep);
		
		for(int i = 0; i < stepCount; i++)
			FixedUpdateEvent(fixedTimeStep);

		fixedUpdateTime -= stepCount * fixedTimeStep;
	}

	currentTime = newTime;
}

double Time::GetDeltaTime() const
{
	return deltaTime;
}

float Time::GetFixedTimeStep() const
{
	return fixedTimeStep;
}

float Time::GetTimeScale() const
{
	return timeScale;
}

void Time::SetFixedTimeStep(const float& _fixedTimestep)
{
	fixedTimeStep = Mathlib::Math::Max(0.f, _fixedTimestep);
}

void Time::SetTimeScale(const float& _timescale)
{
	timeScale = Mathlib::Math::Max(0.f, _timescale);
}