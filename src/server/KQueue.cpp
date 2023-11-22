#include "../../inc/server/KQueue.hpp"

struct kevent *KQueue::getEventList()
{
    return (this->event_list);
}

struct kevent *KQueue::getChangeEvent()
{
    return (&this->change_event);
}

void KQueue::setKq(int kq)
{
    this->_kq = kq;
}

int KQueue::getKq()
{
    return (this->_kq);
}