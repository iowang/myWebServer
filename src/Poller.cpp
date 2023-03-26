#include "include/Poller.h"
#include <unistd.h>
#include "include/util.h"
#include <vector>

#define MAX_EVENTS 1000
#ifdef OS_LINUX
Poller::Poller()
{
    fd_ = epoll_create(0);
    errif(fd_ == -1, "epoll create error");
    events_ = new epoll_event[MAX_EVENTS];
    memset(events_, 0, sizeof(*events_) * MAX_EVENTS);
}

Poller::~Poller()
{
    if (fd_ != -1)
    {
        close(fd_);
    }
    delete[] events_;
}

std::vector<Channel*>Poller::Poll(int timeout){
    std::vector<Channel*> active_channels;
    int nfds = epoll_wait(fd_,events_,MAX_EVENTS,timeout);
    errif(nfds==-1,"epoll wait error");
    
}

#endif