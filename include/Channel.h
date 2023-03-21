#pragma once
#include<memory>
#include"common.h"
class Channel{
    private:
    //首先要有EventLoop
    std::unique_ptr<EventLoop> eventloop_;
};