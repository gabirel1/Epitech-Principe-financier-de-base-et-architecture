#pragma once

#include <queue>
#include <mutex>

namespace ts
{
    template<class T>
    class Queue
    {
        public:
            Queue() = default;
            ~Queue() = default;

            [[nodiscard]] bool empty() const;
            [[nodiscard]] size_t size() const;

            void push(const T &_data);
            void push(T &&_data);

            template<class ...Ts>
            void append(Ts &&..._args);

            [[nodiscard]] const T &front() const;

            void pop();
            [[nodiscard]] T pop_front();

        private:
            std::mutex m_mutex;
            std::queue<T> m_queue;
    };
}

#include "Common/Thread/Queue.inl"