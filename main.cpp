#include <iostream>
#include <string>
#include <vector>

#include "version.h"
#include <async/async.h>

int version() {
    return PROJECT_VERSION_PATCH;
}

using namespace std;

int main(int argc, char* argv[])
{
    auto context = connect(3);
    vector<string> test = {"CMD1", "CMD2", "{", "CMD3", "CMD4", "}", "{", "CMD5", "CMD6", "{", "CMD7", "CMD8", "}", "CMD9", "}", "{", "CMD10", "CMD11"};

    for (auto cmd: test) {
        receive(cmd.c_str(), cmd.size(), context);
    }

    disconnect(context);
}
