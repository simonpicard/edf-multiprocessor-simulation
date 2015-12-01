#include "simEDF.h"
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <math.h>
#include <fstream>

using namespace std;

simEDF::simEDF(int i, char* path)
{
    m_i = i;
    m_a = 0;
    m_path = path;
    m_s = false;
    parseTasks();
}

simEDF::~simEDF()
{
    //dtor
}

void simEDF::parseTasks()
{
    ifstream stream(m_path);
    if(stream)
    {
        int value;
        int O, P, D, C;
        int i = 0;
        while(stream >> value)
        {
            O = value;
            stream >> value;
            P = value;
            stream >> value;
            D = value;
            stream >> value;
            C = value;
            stringstream ss;
            ss << i;
            string str = ss.str();
            m_tasks.push_back(Task(O, P, D, C, str));
            i++;
        }
        stream.close();
    }
    else
    {
        cout << "error" << endl;
    }
}

void simEDF::firstJobs()
{
    for (int i = 0; i < m_tasks.size(); i++){
        m_jobs.push_back(m_tasks.at(i).getNextJob());
    }
}

void simEDF::setR(Job* j)
{
    if (j->getR() < m_a)
        j->SetR(m_a);
}

bool simEDF::sortR(Job i, Job j)
{
    int iR = i.getR();
    int jR = j.getR();
    if (iR < jR)
        return true;
    else if (iR > jR)
        return false;
    else{
        //cout<<"   "<<i.getName()<<" : "<<i.getD()<<" "<<j.getName()<<" : "<<j.getD()<<endl;
        return i.getD() < j.getD();
    }
}

bool simEDF::sortD(Job i, Job j)
{
    return i.getD() < j.getD();
}

void simEDF::simulate()
{
    firstJobs();
    unsigned long long HP = getPeriod();
    unsigned int idleTime = 0;
    unsigned int executionTime = 0;
    unsigned int switchingTime = 0;
    unsigned int wcet = 0;
    string current = "";
    int sT;
    int preemptions = 0;
    cout<<HP<<endl;
    for(m_a = 0; m_a < HP; m_a++){
        if (m_jobs.at(0).done()){
            Task* t = m_jobs.at(0).getTask();
            m_jobs.erase(m_jobs.begin());
            m_jobs.push_back(t->getNextJob());
            current = "";
        }
        for (int i = 0; i < m_jobs.size(); i++){
            setR(&m_jobs.at(i));
        }
        sort(m_jobs.begin(), m_jobs.end(), sortR);
        if (m_jobs.at(0).getD() < m_a){
            if (!m_s)
                cout<<"/!\\ MISSED DEADLINE"<<endl;
            else{
                ofstream stream("./test/out.txt");
                if(stream)
                {
                    stream<<HP<<" "<<0<<" "<<0<<" "<<0<<" "<<0;
                    stream.close();
                }
            }
            return;
        }
        if (m_jobs.at(0).getR() <= m_a){
            if (current != "" && current != m_jobs.at(0).getName()){
                sT = getSwitchingTime(wcet, m_jobs.at(0).getOrignalC());
                switchingTime += sT;
                m_a += sT;
                preemptions++;
            }
            else if (current == ""){
                sT = getSwitchingTime(m_jobs.at(0).getOrignalC());
                switchingTime += sT;
                m_a += sT;
            }
            m_jobs.at(0).run();
            current = m_jobs.at(0).getName();
            wcet = m_jobs.at(0).getOrignalC();
            executionTime++;
        }
        else{
            current = "";
            wcet = 0;
            idleTime++;
        }
    }
    if (!m_s){
        cout<<"I : "<<0<<" - "<<HP<<endl;
        cout<<"Execution ticks : "<<executionTime<<" ("<<(float)executionTime*100/(float)HP<<"%)"<<endl;
        cout<<"Idle ticks : "<<idleTime<<" ("<<(float)idleTime*100/(float)HP<<"%)"<<endl;
        cout<<"Switching ticks : "<<switchingTime<<" ("<<(float)switchingTime*100/(float)HP<<"%)"<<endl;
        cout<<"Preemptions : "<<preemptions<<endl;
    }
    else{
        ofstream stream("./test/out.txt");
        if(stream)
        {
            stream<<HP<<" "<<executionTime<<" "<<idleTime<<" "<<switchingTime<<" "<<preemptions;
            stream.close();
        }
    }
}

long long simEDF::getPeriod()
{
    long long res = 0;
    for (int i =0; i< m_tasks.size(); i++){
        if (m_tasks.at(i).getO() > res)
            res = m_tasks.at(i).getO();
    }
    res += 2*lcm();
    return res;
}

int simEDF::gcd(int a,int b)
{
    int rem;
    if(b==0)
        return a;
    if(a==0)
        return b;
    if(b>a)
    {
        a=a+b;
        b=a-b;
        a=a-b;
    }
    rem=a%b;
    while(rem>0)
    {
        a=b;
        b=rem;
        rem=a%b;
    }
    if(rem==0)
        return b;
    else
        return rem;
}

long long simEDF::lcm()
{
    /*LCM(a,b) = a*b/GCD(a,b)*/
    int no = m_tasks.size();
    if (no == 1)
        return m_tasks.at(0).getP();
    int i, num[no];
    for(i=0; i<no; i++)
    {
        num[i] = m_tasks.at(i).getP();
    }
    long long res;
    res=num[0]*num[1]/gcd(num[0],num[1]);
    for(i=2; i<no; i++){
        res=res*num[i]/gcd(res,num[i]);
    }
    return res;
}

int simEDF::getSwitchingTime(int a, int b){
    return ceil(((float) (a+b))*((float)m_i/100.0));
}

int simEDF::getSwitchingTime(int a){
    return ceil(((float) (a))*((float)m_i/100.0));
}

float simEDF::computeU()
{
    float res = 0;
    for(int i = 0; i<m_tasks.size(); i++){
        res += (float)m_tasks.at(i).getC()/(float)m_tasks.at(i).getP();
    }
    return res;
}
