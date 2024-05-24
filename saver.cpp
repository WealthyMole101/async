#include "saver.h"

#include <chrono>
#include <fstream>

using namespace std;

Saver::Saver(): m_is_running(true)
{
    auto handler = [&](string& filename){
                while (m_is_running.load()) {
                    const std::lock_guard<std::mutex> lock(m_mutex);

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

                    this_thread::sleep_for(std::chrono::microseconds(100));
                }
            };

    m_thread_0 = shared_ptr<thread>(new thread(handler, std::ref(m_filename_0)));
    m_thread_1 = shared_ptr<thread>(new thread(handler, std::ref(m_filename_1)));
}

Saver::~Saver()
{
    m_is_running = false;
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
            int a = 0;
        }
    };

    file_creator(&m_filename_0, "_file0");
    file_creator(&m_filename_1, "_file1");
}

void Saver::out(vector<string>& blocks)
{
    const std::lock_guard<std::mutex> lock(m_mutex);
    m_commands_queue.push(blocks);
}
