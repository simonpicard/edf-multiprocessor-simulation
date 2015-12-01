#ifndef JOB_H
#define JOB_H
#include <string>
class Task;


class Job
{
    public:
        Job(int R, int D, int C, Task* m_task, std::string name);
        virtual ~Job();
        unsigned int getR() { return m_R; }
        void SetR(unsigned int val) { m_R = val; }
        unsigned int getC() { return m_C; }
        unsigned int getOrignalC() { return m_C; }
        void SetC(unsigned int val) { m_C = val; }
        unsigned int getD() { return m_D; }
        void SetD(unsigned int val) { m_D = val; }
        Task* getTask() { return m_task; }
        std::string getName() { return m_name; }
        void run();
        bool done();
    protected:
    private:
        unsigned int m_R;
        unsigned int m_C;
        unsigned int m_D;
        unsigned int m_originalC;
        Task* m_task;
        std::string m_name;
};

#endif // JOB_H
