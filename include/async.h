#pragma once
#include <string>
#include <bulk.h>

namespace async {
    Bulk* connect(const size_t bulk_size);
    void receive(Bulk* context, const char* buffer, size_t size);
    void disconnect(Bulk* context);
};