#include "Job.h"

Job::Job(int R, int D, int C, Task* task, std::string name)
{
    m_R = R;
    m_D = D;
    m_C = C;
    m_originalC = C;
    m_task = task;
    m_name = name;
}

Job::~Job()
{
    //dtor
}

void Job::run()
{
    m_C--;
}

bool Job::done()
{
    return m_C == 0;
}
