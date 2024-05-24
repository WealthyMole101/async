#include "analyzer.h"

#include <iostream>

using namespace std;

Analyzer::Analyzer(int block_count): m_block_count(block_count)
{
    subscribe(&m_saver);
    subscribe(&m_printer);
}

Analyzer::~Analyzer()
{
    if (m_commands.size() > 0 && m_dynamic_validator.empty()) {
        notify(m_commands);
    }
}

void Analyzer::analize(std::string line)
{
    if (line.compare("{") == 0) {
        if (!m_commands.empty() && m_dynamic_validator.empty()) {
            notify(m_commands);
            m_commands.clear();
        }

        m_dynamic_validator.push(true);
    } else if (line.compare("}") == 0) {
        m_dynamic_validator.pop();

        if (!m_commands.empty() && m_dynamic_validator.empty()) {
            notify(m_commands);
            m_commands.clear();
        }
    } else {
        m_commands.push_back(line);
        m_saver.init();
    }

    if ((!(m_commands.size() % m_block_count)) && m_commands.size() != 0 && m_dynamic_validator.empty()) {
        notify(m_commands);
        m_commands.clear();
    }
}

void Analyzer::subscribe(Observer* obj)
{
    subscribers.push_back(obj);
}

void Analyzer::notify(vector<string>& blocks)
{
    for (auto s: subscribers) {
        s->out(blocks);
    }
}
