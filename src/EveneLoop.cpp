#include "include/EventLoop.h"
#include "include/Poller.h"
#include <Channel.h>
EventLoop::EventLoop()
{
    poller_ = std::make_unique<Poller>();
}
EventLoop::~EventLoop() {}

void EventLoop::loop() const
{
    while (true)
    {
        for (Channel *active_ch : poller_->poll())
        {
            active_ch->HandleEvent();
        }
    }
}

void UpdateChannel(Channel *ch) const
{
    poller_->UpdateChannel(ch);
}

void DeleteChannel(Channel *ch) const
{
    poller_->DeleteChannel(ch);
}
