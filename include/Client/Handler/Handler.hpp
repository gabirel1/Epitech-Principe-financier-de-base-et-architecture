#pragma once

#include <thread>

#include "Common/Thread/Queue.hpp"

namespace io
{
    enum Side
    {
        Recv,
        Send
    };

    template<class T, class _T>
    class Handler
    {
        public:
            Handler() = default;
            virtual ~Handler();

            bool start();

            bool stop();

            void send_to_send(const T &&_val);
            void send_to_recv(const _T &&_val);

            [[nodiscard]] bool empty(Side _side) const;

            T pop_front_send();
            _T pop_front_recv();

            operator bool() const;

        protected:
            virtual void loop() = 0;

        private:
            ts::Queue<T> m_input{};
            ts::Queue<_T> m_output{};

            bool m_running = false;

            std::thread m_thread;
    };
}

#include "Client/Handler/Handler.inl"