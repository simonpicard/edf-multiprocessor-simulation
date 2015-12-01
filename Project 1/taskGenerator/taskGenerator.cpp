#include "taskGenerator.h"
#include <iostream>
#include <math.h>
#include <random>
#include <chrono>
#include <fstream>

taskGenerator::taskGenerator(int nbTasks, int utilisation, char* path)
{
    srand(std::chrono::system_clock::now().time_since_epoch().count());
    m_nbTasks = nbTasks;
    m_utilisation = utilisation;
    m_path = path;
    m_highP = 20;
    m_lowP = 1;
    m_highO = 5;
    m_lowO = 0;
    m_i = false;
}

taskGenerator::~taskGenerator()
{
    //dtor
}

void taskGenerator::generateTasks()
{
    int** tasks = new int*[m_nbTasks];
    for(unsigned int i = 0; i < m_nbTasks; i++)
        tasks[i] = new int[4];

    int* utilisation = generateUtilisation();
    int* PCD;
    int* O = generateO(m_lowO, m_highO);
    for(int i = 0; i<m_nbTasks; i++){
        PCD = generatePCD(utilisation[i], m_lowP, m_highP);
        tasks[i][1] = PCD[0];
        tasks[i][3] = PCD[1];
        tasks[i][2] = PCD[2];
        tasks[i][0] = O[i];
    }

    editUtilisation(tasks);
    std::ofstream stream(m_path);
    if(stream)
    {
        for(int i = 0; i<m_nbTasks; i++){
            for(int j = 0; j<4; j++){
                stream<<tasks[i][j]<<" ";
            }
        stream<<'\n';
        }
        stream.close();
    }
    else
    {
        std::cout << "error while writting output" << std::endl;
    }
}

int* taskGenerator::generateUtilisation()
{
    float averageUtilisation = (float) m_utilisation/(float) m_nbTasks;

    int *utilisation = new int[m_nbTasks];
    int sum = 0;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::normal_distribution<double> distribution(averageUtilisation, 4.0);

    for(unsigned int i = 0; i < m_nbTasks; i++) {
        utilisation[i] = distribution(generator);
        if (utilisation[i] <= 0)
        {
            utilisation[i] = 1;
        }
        else if (utilisation[i] >= m_utilisation-m_nbTasks-i)
        {
            utilisation[i] = m_utilisation-m_nbTasks-i;
        }
        sum += utilisation[i];
    }

    int i = 0;
    int j = 0;
    bool same = false;
    while (sum < m_utilisation)
    {
        if (utilisation[i%m_nbTasks] < m_utilisation){
            utilisation[i%m_nbTasks]++;
            sum++;
            j = i;
            same = false;
        }
        else{
            if (i == j && same)
                break;
            else
                same = true;
        }
        i++;
    }
    while (sum > m_utilisation)
    {
        if (utilisation[i%m_nbTasks] > 1){
            utilisation[i%m_nbTasks]--;
            same = false;
            sum--;
            j = i;
        }
        else{
            if (i == j && same)
                break;
            else
                same = true;
        }
        i++;
    }
    return utilisation;
}

int* taskGenerator::generatePCD(int utilisation, int lowP, int highP)
{
    int* res = new int[3];
    res[0] = ((rand()%(highP-lowP))+lowP)*10;
    res[1] = round((float) res[0]*((float)utilisation/100));
    if (res[1] == 0)
        res[1]++;
    if (m_i){
        res[2] = res[0];
    }
    else{
        res[2] = res[1] + (rand()%(res[0]-res[1]));
    }
    return res;
}

int* taskGenerator::generateO(int low, int high)
{
    int* res = new int[m_nbTasks];
    for(unsigned int i = 0; i < m_nbTasks; i++) {
        res[i] = rand()%(high-low)+low;
    }
    return res;
}

void taskGenerator::editUtilisation(int** tasks)
{
    int i = 0;
    while(computeU(tasks)<m_utilisation){
        tasks[i][3]++;
        i++;
        i%=m_nbTasks;
    }
    while(computeU(tasks)>m_utilisation){
        tasks[i][3]--;
        i++;
        i%=m_nbTasks;
    }
}

int taskGenerator::computeU(int** tasks)
{
    float res = 0;
    for(int i = 0; i<m_nbTasks; i++){
        res += (float)tasks[i][3]/(float)tasks[i][1];
    }
    return (int) round(res*100);
}

void taskGenerator::setHP(int v)
{
	m_highP = v;
}

void taskGenerator::setHO(int v)
{
	m_highO = v;
}

void taskGenerator::setLP(int v)
{
	m_lowP = v;
}

void taskGenerator::setLO(int v)
{
	m_lowO = v;
}

void taskGenerator::setI(bool v)
{
	m_i = v;
}
