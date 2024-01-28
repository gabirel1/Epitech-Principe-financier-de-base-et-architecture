#ifndef THREADHEARTBEAT_HPP_
#define THREADHEARTBEAT_HPP_

#include <QThread>

class ThreadHeartBeat : public QThread {

    Q_OBJECT

    public:
        ThreadHeartBeat();
        ~ThreadHeartBeat();

    protected:
        void run();
    private:

    signals:
        void callHeartBeat();
};

#endif /* !THREADHEARTBEAT_HPP_ */
