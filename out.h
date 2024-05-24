#ifndef OUT_H
#define OUT_H

#include <string>
#include <vector>

class Observer
{
public:
    virtual void out(std::vector<std::string>& blocks) = 0;
};

class Observable
{
public:
    virtual void subscribe(Observer* obj) = 0;
};

#endif // OUT_H
