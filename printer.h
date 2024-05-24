#ifndef PRINTER_H
#define PRINTER_H

#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include "out.h"

class Printer: public Observer
{
public:
    Printer();
    ~Printer();
    void init();
    void out(std::vector<std::string> &blocks) override;
private:
    std::string m_filename;
    std::shared_ptr<std::thread> m_thread;
    std::mutex m_mutex;
    std::vector<std::string> m_blocks;
    std::atomic_bool m_is_running;
};

#endif // PRINTER_H
