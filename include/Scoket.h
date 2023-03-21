#pragma once
#include"common.h"
#include<string>
class Socket{
    private:
        int fd_;
    public:
        Socket();
        ~Socket();
};