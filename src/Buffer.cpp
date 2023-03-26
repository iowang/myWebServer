#include "include/Buffer.h""

size_t Buffer::Size() const
{
    return buf_.size();
}

const std::string &Buffer::buf() const
{
    return buf_;
}

const char *c_str() const
{
    return buf_.c_str();
}

void Buffer::set_buf(const char *buf)
{
    std::string new_buf(buf);
    buf_.swap(new_buf);
}

void Buffer::Append(const char *_str, int _size)
{
    for (int i = 0; i < _size; ++i)
    {
        if (str[i] == '\0')
        {
            break;
        }
        buf_.push_back(str[i]);
    }
}

void Buffer::Clear()
{
    buf_.clear();
}