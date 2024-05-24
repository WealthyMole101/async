#ifndef SAVER_H
#define SAVER_H

#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include "out.h"

class Saver: public Observer
{
public:
    Saver();
    ~Saver();
    void init();
    void out(std::vector<std::string> &blocks) override;
private:
    std::string m_filename_0;
    std::string m_filename_1;
    std::shared_ptr<std::thread> m_thread_0;
    std::shared_ptr<std::thread> m_thread_1;

    std::mutex m_mutex;
    std::atomic_bool m_is_running;
    std::queue<std::vector<std::string>> m_commands_queue;
};

#endif // SAVER_H
