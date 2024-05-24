#ifndef ASYNC_H
#define ASYNC_H

#include <cstddef>

int* connect(int block_count);
void receive(const char* buf, size_t size, int* context);
void disconnect(int* context);

#endif // ASYNC_H
