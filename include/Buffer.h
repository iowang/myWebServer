#pragma once
#include "common.h"
#include <string>
#include <memory>

class Buffer
{
private:
    std::string buf_;

public:
    DISALLOW_COPY_AND_MOVE(Buffer);
    Buffer() = default;
    ~Buffer() = default;
    size_t Size() const;
    const std::string &buf() const;
    const char *c_str() const;
    void set_buf(const char *buf);
    void Append(const char *_str, int _size);
    void Clear();
};