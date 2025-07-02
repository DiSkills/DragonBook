#ifndef BUFFER_HPP
#define BUFFER_HPP

#ifndef INIT_BUFFER_SIZE
#define INIT_BUFFER_SIZE 256
#endif

class Buffer {
    char *buf;
    unsigned int capacity, size;
public:
    Buffer() : buf(0), capacity(0), size(0) {}
    ~Buffer() { if (buf) delete[] buf; }
    void Append(char c)
        { if (size >= capacity) Resize(); buf[size] = c; size++; }
    char *String() const;
    void Clear() { size = 0; }
private:
    void Resize();
private:
    Buffer(const Buffer &);
    void operator=(const Buffer &);
};

#endif
