#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <iterator>
#include <string>
#include <algorithm>

using namespace std;

string itoc(int i){
    stringstream out;
    out << i;
    return out.str();
}

unsigned long long* genRun(int u, int nb, int i, bool I, bool S){
    string res = "";
    string command = "./taskGenerator -u "+itoc(u)+" -n "+itoc(nb)+" -o ./test/"+itoc(u)+"-"+itoc(nb)+".txt";
    if (I)
        command += " -i";
    if (S)
        command += " -s";
    system(command.c_str());
    command = "./simEDF ./test/"+itoc(u)+"-"+itoc(nb)+".txt "+itoc(i)+" s";
    system(command.c_str());
    ifstream stream("./test/out.txt");
    if(stream){
        getline(stream, res);
        stream.close();
    }
    istringstream iss(res);
    vector<string> tokens{istream_iterator<string>{iss}, istream_iterator<string>{}};

    unsigned long long* ret = new unsigned long long[5];

    for (int i = 0; i < tokens.size(); i++){
        ret[i] = atoi(tokens[i].c_str());
    }
    return ret;
}

unsigned long long* run(int u, int nb, int i){
    string res = "";
    string command = "./simEDF ./test/"+itoc(u)+"-"+itoc(nb)+".txt "+itoc(i)+" s";
    system(command.c_str());
    ifstream stream("./test/out.txt");
    if(stream){
        getline(stream, res);
        stream.close();
    }
    istringstream iss(res);
    vector<string> tokens{istream_iterator<string>{iss}, istream_iterator<string>{}};

    unsigned long long* ret = new unsigned long long[5];

    for (int i = 0; i < tokens.size(); i++){
        ret[i] = atoi(tokens[i].c_str());
    }
    return ret;
}



int main()
{
    string output = "./study.txt";
    ofstream stream(output);

    vector<int> testU;
    testU.push_back(40);
    testU.push_back(60);
    testU.push_back(80);
    testU.push_back(90);
    testU.push_back(95);
    testU.push_back(99);
	vector<int> resU (testU.size(), 0);

    vector<int> testN;
    testN.push_back(1);
    testN.push_back(2);
    testN.push_back(3);
    testN.push_back(4);
    testN.push_back(5);
    testN.push_back(6);
	vector<int> resN (testN.size(), 0);

    vector<int> testI;
    testI.push_back(0);
    testI.push_back(5);
    testI.push_back(10);
    testI.push_back(15);
    testI.push_back(20);
	vector<int> resI (testI.size(), 0);

    unsigned long long* res = new unsigned long long[5];
    cout<<"Schedulability test"<<endl;
	
    for (int i = 0; i< 100; i++){
		for (int j = 0; j< testU.size(); j++){
		    res = genRun(testU[j], 3, 0, false, false);
		    if (res[1] != 0)
				resU[j]++;
		}
		for (int j = 0; j< testN.size(); j++){
		    res = genRun(50, testN[j], 0, false, false);
		    if (res[1] != 0)
				resN[j]++;
		}
		for (int j = 0; j< testI.size(); j++){
		    res = genRun(50, 3, testI[j], false, false);
		    if (res[1] != 0)
				resI[j]++;
		}
	}

	for (int j = 0; j< testU.size(); j++){
	    stream<<testU[j]<<"\t";
	}
	stream<<"\n";
	for (int j = 0; j< testU.size(); j++){
	    stream<<resU[j]<<"\t";
	}
	stream<<"\n\n\n";

	for (int j = 0; j< testN.size(); j++){
	    stream<<testN[j]<<"\t";
	}
	stream<<"\n";
	for (int j = 0; j< testN.size(); j++){
	    stream<<resN[j]<<"\t";
	}
	stream<<"\n\n\n";

	for (int j = 0; j< testI.size(); j++){
	    stream<<testI[j]<<"\t";
	}
	stream<<"\n";
	for (int j = 0; j< testI.size(); j++){
	    stream<<resI[j]<<"\t";
	}
	stream<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";

	testN.clear();

    testN.push_back(2);
    testN.push_back(4);
    testN.push_back(6);
    testN.push_back(8);

    testN.push_back(10);
    testN.push_back(15);
    testN.push_back(20);
    testN.push_back(30);

    cout<<"Period test"<<endl;
    stream<<"Nb tasks\t";
    for (int j = 0; j< testN.size(); j++)
        stream<<testN[j]<<"\t";
    stream<<"\n";

    stream<<"Period\t";
    for (int j = 0; j< testN.size(); j++){
        res = genRun(80, testN[j], 75, false, false);
        stream<<res[0]<<"\t";
    }
    stream<<"\n\n\n\n";

    testN.pop_back();
    testN.pop_back();
    testN.pop_back();
    testN.pop_back();


    cout<<"Number of tasks test"<<endl;
    stream<<"Nb tasks\tExecution time\tIdle time\tSwitching time\tPreemptions\n";
    for (int j = 0; j< testN.size(); j++){
        res = genRun(85, testN[j], 0, true, true);
        stream<<testN[j]<<"\t"<<(float)res[1]*100/(float)res[0]<<"\t"<<(float)res[2]*100/(float)res[0]<<"\t"<<(float)res[3]*100/(float)res[0]<<"\t"<<res[4]*100/(float)res[0];
        stream<<"\n";
    }
    stream<<"\n\n\n";

    cout<<"Utilisation test"<<endl;
    stream<<"Utilisation\tExecution time\tIdle time\tSwitching time\tPreemptions\n";
    for (int j = 0; j< testU.size(); j++){
        res = genRun(testU[j], 4, 0, true, true);
        stream<<testU[j]<<"\t"<<(float)res[1]*100/(float)res[0]<<"\t"<<(float)res[2]*100/(float)res[0]<<"\t"<<(float)res[3]*100/(float)res[0]<<"\t"<<res[4]*100/(float)res[0];
        stream<<"\n";
    }
    stream<<"\n\n\n";

    cout<<"Switching time percent test"<<endl;
    testI.push_back(1);
    testI.push_back(3);
    sort(testI.begin(), testI.end());
    testI.push_back(20);
    testI.push_back(30);
    testI.push_back(40);
    testI.push_back(50);
    testI.push_back(60);
    testI.push_back(70);
    testI.push_back(80);
    testI.push_back(90);
    testI.push_back(100);
    stream<<"Switching percent\tExecution time\tIdle time\tSwitching time\tPreemptions\n";
    res = genRun(40, 4, testI[0], true, true);
    stream<<testI[0]<<"\t"<<(float)res[1]*100/(float)res[0]<<"\t"<<(float)res[2]*100/(float)res[0]<<"\t"<<(float)res[3]*100/(float)res[0]<<"\t"<<res[4];
    stream<<"\n";
    for (int j = 1; j< testI.size(); j++){
        res = run(40, 4, testI[j]);
        stream<<testI[j]<<"\t"<<(float)res[1]*100/(float)res[0]<<"\t"<<(float)res[2]*100/(float)res[0]<<"\t"<<(float)res[3]*100/(float)res[0]<<"\t"<<res[4];
        stream<<"\n";
    }



    return 0;
}
