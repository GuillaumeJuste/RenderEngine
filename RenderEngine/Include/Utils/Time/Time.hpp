#pragma once

#ifndef RENDERENGINE_TIME
#define RENDERENGINE_TIME

#include <chrono>
#include <ctime>
#include "Utils/Event/Event.hpp"

namespace RenderEngine::Utils
{
    /**
     * @brief Time class implementation
    */
    class Time
    {
    private:
        /**
         * @brief interval between fixed update
        */
        float fixedTimeStep = 0.02f;

        /**
         * @brief time scale
        */
        float timeScale = 1.f;
        
        /**
         * @brief current frame delta time
        */
        double deltaTime = 0.0;

        /**
         * @brief time since last fixed update call
        */
        float fixedUpdateTime = 0.0;

        /**
         * @brief clock to manage the time
        */
        std::chrono::steady_clock timer;

        /**
         * @brief starting time of the clock
        */
        std::chrono::time_point<std::chrono::steady_clock> startTime;

        /**
         * @brief time of the current frame
        */
        std::chrono::time_point<std::chrono::steady_clock> currentTime;
        
    public:
        /**
         * @brief event called when a fxied update should happen
        */
        Event<double> FixedUpdateEvent;

        /// Default constructor.
        Time() = default;

        /**
         * @brief parametered constructor
         * @param _fixedTimestep fixed update interval
         * @param _timescale time scale
        */
        Time(const float& _fixedTimestep, const float& _timescale);

        /**
         * @brief start timer
        */
        void Start();

        /**
         * @brief update timer
        */
        void Update();

        /**
         * @brief Get the frame delta time
        */
        double GetDeltaTime() const;

        /**
         * @brief Get current time between fixed update
        */
        float GetFixedTimeStep() const;

        /**
         * @brief Get current time scale
        */
        float GetTimeScale() const;

        /**
         * @brief Set the time between fixed update
         * @param _fixedTimestep new fixed update time step
        */
        void SetFixedTimeStep(const float& _fixedTimestep);

        /**
         * @brief set the time scale
         * @param _timescale new time scale
        */
        void SetTimeScale(const float& _timescale);
    };
}

#endif