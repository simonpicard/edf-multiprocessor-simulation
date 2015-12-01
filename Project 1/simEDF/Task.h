#ifndef TASK_H
#define TASK_H
#include "Job.h"
#include <string>


class Task
{
    public:
        Task(int O, int P, int D, int C, std::string name);
        virtual ~Task();
        unsigned int getO() { return m_O; }
        void SetO(unsigned int val) { m_O = val; }
        unsigned int getP() { return m_P; }
        void SetP(unsigned int val) { m_P = val; }
        unsigned int getD() { return m_D; }
        void SetD(unsigned int val) { m_D = val; }
        unsigned int getC() { return m_C; }
        void SetC(unsigned int val) { m_C = val; }
        std::string getName() { return m_name; }
        Job getJob(int i);
        Job getNextJob();
    protected:
    private:
        unsigned int m_O;
        unsigned int m_P;
        unsigned int m_D;
        unsigned int m_C;
        int m_a;
        std::string m_name;
};

#endif // TASK_H
