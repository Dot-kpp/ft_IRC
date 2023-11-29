
#ifndef KQUQUE_HPP
#define KQUQUE_HPP

#include <sys/event.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>

class KQueue {
private:
    int _kq;
    struct kevent change_event;
    struct kevent event_list[1024];

public:
    KQueue() {};
    KQueue(int kq) : _kq(kq) {};
    ~KQueue() {};

    struct kevent *getEventList();
    struct kevent *getChangeEvent();
    void setKq(int kq);
    int getKq();
};

#endif