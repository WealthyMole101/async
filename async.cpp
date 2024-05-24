#include "async.h"
#include "analyzer.h"

#include <string>

using namespace std;

int* connect(int block_count)
{
    return reinterpret_cast<int*>(new Analyzer(block_count));
}

void receive(const char* buf, size_t size, int* context)
{
    Analyzer* analyzer = reinterpret_cast<Analyzer*>(context);
    string line(buf, size);
    analyzer->analize(line);
}

void disconnect(int* context)
{
    Analyzer* analyzer = reinterpret_cast<Analyzer*>(context);
    delete analyzer;
}
