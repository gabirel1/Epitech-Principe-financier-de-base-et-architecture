#pragma once

#include "Client/Data/IProcessor.hpp"
#include "Common/Thread/Queue.hpp"

using EntryOutput = ts::Queue<std::string>;

class UserInput
{
    public:
        UserInput(EntryOutput &_output);
        ~UserInput();

        [[nodiscard]] bool start();

        [[nodiscard]] bool stop();

    private:
        void loop();

        bool m_running = false;

        EntryOutput &m_output;
        std::string m_input;
        std::thread m_thread;
};