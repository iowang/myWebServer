// 这个类是Epoll类的替换，使用此类可以再macos上运行
#pragma once
#include "common.h"
#include <vector>

#ifdef OS_LINUX
#include <sys/epoll.h>
#endif

#ifdef OS_MACOS
#include <sys/event.h>
#endif

class Poller
{
private:
    int fd_;
#ifdef OS_LINUX
    struct epoll_event *events_{nullptr};
#endif
#ifdef OS_MACOS
    struct kevent *events_;
#endif

public:
    DISALLOW_COPY_AND_MOVE(Poller);
    Poller();
    ~Poller();
    // Channel是用来分发fd的，把channel上树
    RC UpdateChannel(Channel *ch) const;
    RC DeleteChannel(Channel *ch) const;
    // 还需要一个处理epoll返回的函数返回事件结果的向量
    std::vector<Channel *> Poll(long timeout = -1) const;
};