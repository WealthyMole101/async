#include "printer.h"

#include <iostream>

using namespace std;

Printer::Printer(): m_is_running(true)
{
    m_thread = shared_ptr<thread>(new thread([&](){
            while (m_is_running.load()) {
                // wait until main() sends data
                unique_lock lk(m_mutex);
                m_cv.wait(lk, [&]{ return m_ready; });

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

                m_processed = true;

                // manual unlocking is done before notifying, to avoid waking up
                // the waiting thread only to block again (see notify_one for details)
                lk.unlock();
                m_cv.notify_one();
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

    {
        lock_guard lk(m_mutex);
        m_processed = false;
        m_ready = true;
    }

    m_cv.notify_one();

    m_is_running = false;
    m_thread.get()->join();
}

void Printer::out(vector<string>& blocks)
{
    {
        lock_guard lk(m_mutex);
        m_processed = false;
        m_ready = true;
    }

    m_blocks = blocks;
    m_cv.notify_one();

    {
        unique_lock lk(m_mutex);
        m_cv.wait(lk, [&]{ return m_processed; });
    }
}
