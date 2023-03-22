#pragma once
#include "common.h"
#include <memory>
class EventLoop
{
private:
    std::unique_ptr<Poller> poller_;

public:
    DISALLOW_COPY_AND_MOVE(EventLoop);
    EventLoop();
    ~EventLoop();
    void loop() const;
    void UpdateChannel(Channel *ch) const;
    void DeleteChannel(Channel *ch) const;
};