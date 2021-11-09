#ifndef SIMEDF_H
#define SIMEDF_H
#include "Task.h"
#include "Job.h"
#include<vector>


class simEDF
{
    public:
        simEDF(int i, char* path);
        virtual ~simEDF();
        void setS(bool v){ m_s = v;}
        void simulate();
    protected:
    private:
        int m_i;
        long long m_a;
        char* m_path;
        std::vector<Task> m_tasks;
        std::vector<Job> m_jobs;
        bool m_s;

        void parseTasks();
        void firstJobs();
        static bool sortR(Job i, Job j);
        bool sortD(Job i, Job j);
        void setR(Job* j);
        long long getPeriod();
        int gcd(int a, int b);
        long long lcm();
        int getSwitchingTime(int a, int b);
        int getSwitchingTime(int a);
        float computeU();
};

#endif // SIMEDF_H
