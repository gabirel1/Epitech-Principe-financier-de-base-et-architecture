#pragma once

#include <concepts>
#include <future>

using ms = unsigned long long int;

template<class T>
class Pipeline
{
    public:
        Pipeline();
        virtual ~Pipeline();

        [[nodiscard]] std::future_status stop();

    protected:
        void tstart(T *_pipeline);

        [[nodiscard]] std::future_status tstatus(ms _to);

        bool m_running = false;

    private:
        std::future<void> m_futur;
};

#include "Server/Core/Pipeline/Core.inl"