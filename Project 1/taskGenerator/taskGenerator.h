#ifndef TASKGENERATOR_H
#define TASKGENERATOR_H
#include <string>


class taskGenerator
{
    public:
        taskGenerator(int nbTasks, int utilisation, char* path);
        virtual ~taskGenerator();
        void generateTasks();
        void setHP(int v);
        void setLP(int v);
        void setHO(int v);
        void setLO(int v);
        void setI(bool v);
    protected:
    private:
        int m_nbTasks;
        int m_utilisation;
        std::string m_path;
        int m_highP;
        int m_highO;
        int m_lowP;
        int m_lowO;
        bool m_i;

        int* generateUtilisation();
        int* generatePCD(int utilisation, int lowP, int highP);
        int* generateO(int low, int high);
        void editUtilisation(int** tasks);
        int computeU(int** tasks);

};

#endif // TASKGENERATOR_H
