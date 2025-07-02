#include "buffer.hpp"

void Buffer::Resize()
{
    unsigned int new_capacity = capacity + INIT_BUFFER_SIZE;
    char *new_buf = new char[new_capacity];
    for (unsigned int i = 0; i < capacity; i++) {
        new_buf[i] = buf[i];
    }
    if (buf) {
        delete[] buf;
    }
    buf = new_buf;
    capacity = new_capacity;
}

char *Buffer::String() const
{
    char *s = new char[size + 1];
    for (unsigned int i = 0; i < size; i++) {
        s[i] = buf[i];
    }
    s[size] = '\0';
    return s;
}
