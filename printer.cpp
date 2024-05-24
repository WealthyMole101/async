#include "printer.h"

#include <iostream>

using namespace std;

Printer::Printer(): m_is_running(true)
{
    m_thread = shared_ptr<thread>(new thread([&](){
            while (m_is_running.load()) {
                const std::lock_guard<std::mutex> lock(m_mutex);

                if (distance(m_blocks.begin(), m_blocks.end()) > 0) {
                    cout << "bulk: ";
                    for (auto it = m_blocks.begin(); it != m_blocks.end(); ++it) {
                        cout << *it;
                        if (it != m_blocks.end() -1) {
                            cout << ", ";
                        }
                    }

                    cout << endl;
                    m_blocks.clear();
                }

                this_thread::sleep_for(std::chrono::microseconds(100));
            }
        })
    );
}

void Printer::init()
{

}

Printer::~Printer()
{
    m_is_running = false;
    m_thread.get()->join();
}

void Printer::out(vector<string>& blocks)
{
    const std::lock_guard<std::mutex> lock(m_mutex);
    m_blocks = blocks;
}
