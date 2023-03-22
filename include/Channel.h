#pragma once
#include <functional>
#include <memory>
#include "common.h"
class Channel
{
private:
    // 首先要有EventLoop
    std::unique_ptr<EventLoop> loop_;
    // 既然Channel负责分发fd就应该有个fd
    int fd_;
    // 这两个还没想明白
    short listen_events_;
    short ready_events_;
    // 判断是否在红黑树上
    bool exist_;
    // 读写回调函数
    std::function<void()> read_callback_;
    std::function<void()> write_callback_;

public:
    DISALLOW_COPY_AND_MOVE(Channel);
    Channel(int fd, EventLoop *loop);
    ~Channel();

    // 最底层要处理读写事件啦
    void HandleEvent() const;
    void EnableRead();
    void EnableWrite();

    int fd() const;
    short listen_events() const;
    short ready_events() const;
    bool exist() const;
    void set_exits(bool in = true);
    void EnableET();
    void set_ready_event(short ev);
    void set_read_callback(std::function<void()> const &callback);
    void set_write_callback(std::function<void()> const &callback);

    static const short READ_EVENT;
    static const short WRITE_EVENT;
    static const short ET;
};