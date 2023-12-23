#ifndef THREADSOCKET_HPP_
#define THREADSOCKET_HPP_

#include <QThread>

class ThreadSocket : public QThread {

    Q_OBJECT

    public:
        ThreadSocket();
        ~ThreadSocket();
        
        void stop();

    protected:
        void run();
    
    private:
        bool m_running = false;
};

#endif /* !THREADSOCKET_HPP_ */
