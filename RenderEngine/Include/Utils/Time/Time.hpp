#pragma once

#ifndef RENDERENGINE_TIME
#define RENDERENGINE_TIME

#include <chrono>
#include <ctime>
#include "Utils/Event/Event.hpp"

namespace RenderEngine::Utils
{
    /**
     * @brief Timer class implementation
    */
    class Time
    {
    private:
        float fixedTimeStep = 0.02f;
        float timeScale = 1.f;
        
        double deltaTime = 0.0;

        float fixedUpdateTime = 0.0;

        std::chrono::steady_clock timer;

        std::chrono::time_point<std::chrono::steady_clock> startTime;
        std::chrono::time_point<std::chrono::steady_clock> currentTime;
        
    public:
        Event<double> FixedUpdateEvent;

        Time() = default;
        Time(const float& _fixedTimestep, const float& _timescale);

        void Start();
        void Update();

        double GetDeltaTime() const;

        float GetFixedTimeStep() const;
        float GetTimeScale() const;

        void SetFixedTimeStep(const float& _fixedTimestep);
        void SetTimeScale(const float& _timescale);
    };
}

#endif