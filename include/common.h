// 该文件夹是用于定义常用的宏和类头
#pragma once
class TcpServer;
class EventLoop;
class Socket;
class Acceptor;
class Channel;
class Connection;
class ThreadPool;

// 禁止拷贝和移动构造
#define DISALLOW_COPY(cname)  \
  cname(const cname &) = delete; \
  cname &operator=(const cname &) = delete;

#define DISALLOW_MOVE(cname)   \
  cname(const cname &&) = delete; \
  cname &operator=(const cname &&) = delete;

#define DISALLOW_COPY_AND_MOVE(cname) \
  DISALLOW_COPY(cname); \
  DISALLOW_MOVE(cname); \

  
  
  enum RC {
  RC_UNDEFINED,
  RC_SUCCESS,
  RC_SOCKET_ERROR,
  RC_POLLER_ERROR,
  RC_CONNECTION_ERROR,
  RC_ACCEPTOR_ERROR,
  RC_UNIMPLEMENTED
};
