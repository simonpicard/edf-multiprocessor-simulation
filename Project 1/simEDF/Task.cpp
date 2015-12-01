#include "Task.h"
#include <sstream>

Task::Task(int O, int P, int D, int C, std::string name)
{
    m_O = O;
    m_P = P;
    m_D = D;
    m_C = C;
    m_a = 0;
    m_name = name;
}

Task::~Task()
{
    //dtor
}

Job Task::getJob(int i)
{
    std::stringstream ss;
    ss << m_a;
    std::string str = ss.str();
    return Job(m_O+i*m_P, m_O+i*m_P+m_D, m_C, this, m_name+":"+str);
}

Job Task::getNextJob()
{
    return getJob(m_a++);
}
