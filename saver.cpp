#include "saver.h"

#include <chrono>
#include <fstream>

using namespace std;

Saver::Saver(): m_is_running(true)
{
    auto handler = [&](string& filename){

                while (m_is_running.load()) {
                    // wait until main() sends data
                    unique_lock lk(m_mutex);
                    m_cv.wait(lk, [&]{ return m_ready; });

                    if (!m_commands_queue.empty()) {
                        auto blocks = m_commands_queue.front();
                        m_commands_queue.pop();

                        if (distance(blocks.begin(), blocks.end()) > 0) {
                            ofstream file;
                            file.open(filename, std::ios::out);

                            file << "bulk: ";
                            for (auto it = blocks.begin(); it != blocks.end(); ++it) {
                                file << *it;
                                if (it != blocks.end() -1) {
                                    file << ", ";
                                }
                            }

                            file.close();
                            filename.clear();
                            blocks.clear();
                        }
                    }

                    m_processed = true;

                    // manual unlocking is done before notifying, to avoid waking up
                    // the waiting thread only to block again (see notify_one for details)
                    lk.unlock();
                    m_cv.notify_one();
                }
            };


    m_thread_0 = shared_ptr<thread>(new thread(handler, std::ref(m_filename_0)));
    m_thread_1 = shared_ptr<thread>(new thread(handler, std::ref(m_filename_1)));
}

Saver::~Saver()
{
    m_is_running = false;

    {
        lock_guard lk(m_mutex);
        m_processed = false;
        m_ready = true;
    }

    m_cv.notify_one();

    m_thread_0.get()->join();
    m_thread_1.get()->join();
}

void Saver::init()
{
    auto file_creator = [](string* filename, string suffix){
        if (filename->empty()) {
            const auto now = std::chrono::system_clock::now();
            const std::time_t t_c = std::chrono::system_clock::to_time_t(now);
            *filename = std::to_string(t_c) + "_" + to_string(rand()) + suffix + ".log";
        }
    };

    file_creator(&m_filename_0, "_file0");
    file_creator(&m_filename_1, "_file1");
}

void Saver::out(vector<string>& blocks)
{
    {
        lock_guard lk(m_mutex);
        m_processed = false;
        m_ready = true;
    }

    m_commands_queue.push(blocks);
    m_cv.notify_one();

    {
        unique_lock lk(m_mutex);
        m_cv.wait(lk, [&]{ return m_processed; });
    }
}
