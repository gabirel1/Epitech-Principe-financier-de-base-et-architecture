#include "Client/Data/UserInput.hpp"

UserInput::UserInput(EntryOutput &_output)
    : m_output(_output)
{
}

UserInput::~UserInput()
{
    (void)stop();
}

bool UserInput::start()
{
    if (!m_running) {
        m_thread = std::thread([this] () {
            loop();
        });
        m_running = true;
    }
    return m_running;
}

bool UserInput::stop()
{
    m_running = false;
    if (m_thread.joinable()) {
        m_thread.join();
        return true;
    }
    return false;
}

void UserInput::loop()
{
    while (m_running) {
        std::cout << "$> ";
        m_input.clear();
        std::cin >> m_input;
        m_output.push(m_input);
    }
}