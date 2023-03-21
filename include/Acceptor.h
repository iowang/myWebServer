#pragma once
#include <memory>
#include "common.h"
#include <functional>

class Acceptor
{
private:
    // 首先要有Socket这个类
    std::unique_ptr<Socket> socket_;
    // 要有处理事件的Channel
    std::unique_ptr<Channel> channel_;
};