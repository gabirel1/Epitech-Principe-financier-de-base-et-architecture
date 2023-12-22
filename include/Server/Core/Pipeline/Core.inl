#include "Server/Core/Pipeline/Core.hpp"
#include "Common/Core/Logger.hpp"

template<class T>
Pipeline<T>::Pipeline()
{
    Logger::Log("[Pipeline] New pipeline created");
}

template<class T>
Pipeline<T>::~Pipeline()
{
    (void)stop();
}

template<class T>
std::future_status Pipeline<T>::stop()
{
    std::future_status ret = std::future_status::ready;

    m_running = false;
    for (; ret == std::future_status::ready; ret = tstatus(500)) {}
    return ret;
}

template<class T>
void Pipeline<T>::tstart(T *_pipeline)
{
    m_running = true;
    m_futur = std::async(std::launch::async, &T::loop, _pipeline);
}

template<class T>
std::future_status Pipeline<T>::tstatus(ms _to)
{
    return m_futur.wait_for(std::chrono::milliseconds(_to));
}