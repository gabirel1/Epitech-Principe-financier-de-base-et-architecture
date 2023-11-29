#pragma once

#include <concepts>
#include <future>

using ms = unsigned long long int;

template<class T>
class Pipeline
{
    public:
        Pipeline() = default;
        virtual ~Pipeline() = default;

        [[nodiscard]] std::future_status stop();

    protected:
        void tstart(T *_pipeline);

        [[nodiscard]] std::future_status tstatus(ms _to);

        bool m_running = true;

    private:
        std::future<void> m_futur;
};

#include "Core/Pipeline/Core.inl"