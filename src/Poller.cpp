#include "include/Poller.h"
#include <unistd.h>
#include "include/util.h"
#include "include/Channel.h"
#include "include/Socket.h"
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

std::vector<Channel *> Poller::Poll(int timeout)
{
    std::vector<Channel *> active_channels;
    int nfds = epoll_wait(fd_, events_, MAX_EVENTS, timeout);
    errif(nfds == -1, "epoll wait error");
    for (int i = 0; i < nfds; ++i)
    {
        Channel *ch = (Channel *)events_[i].data.ptr;
        int events = events_[i].events;
        if (events & EPOLLIN)
        {
            ch->set_ready_event(Channel::READ_EVENT);
        }
        if (events & EPOLLOUT)
        {
            ch->set_ready_event(Channel::WRITE_EVENT);
        }
        if (events & EPOLLET)
        {
            ch->set_ready_event(Channel::ET);
        }
        active_channels.push_back(ch);
    }
    return active_channels;
}

RC Poller::UpdateChannel(Channel *ch) const
{
    int sockfd = ch->fd();
    struct epoll_event ev
    {
    };
    if (ch->listen_events() & Channel::READ_EVENT)
    {
        ev.events |= EPOLLIN | EPOLLPRI;
    }
    if (ch->listen_events() & Channel::WRITE_EVENT)
    {
        ev.events |= EPOLLOUT;
    }
    if (ch->listen_events() & Channel::ET)
    {
        ev.events |= EPOLLET;
    }
    if (!ch->exist())
    {
        errif(epoll_ctl(fd_, EPOLL_CTL_ADD, sockfd, &ev) == -1, "epoll add error");
        ch->set_exist(true);
    }
    else
    {
        errif(epoll_ctl(fd_, EPOLL_CTL_MOD, sockfd, &ev) == -1, "epoll mod error");
    }
}

RC Poller::DeleteChannel(Channel *ch) const
{
    int sockfd = ch->fd();
    errif(epoll_ctl(fd_, EPOLL_CTL_DEL, sockfd, nullptr) == -1, "epoll delete error");
    ch->set_exist(false);
}

#endif