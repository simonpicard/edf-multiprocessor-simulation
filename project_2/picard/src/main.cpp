#include <iostream>
#include <mpi.h>
#include <fstream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <string>
#include <stdlib.h>
#include <ctime>
#include <sstream>
#include <chrono>

using namespace std;

vector<vector<int> > parseMatrix(char* path){
    ifstream stream(path);
	vector<vector<int> > mat;
	vector<int> tmp;
    if(stream)
    {
        string value;
        while(getline(stream, value))
        {
            for (int i = 0; i < value.size(); i++){
                if (value[i] == '1')
                    tmp.push_back(1);
                else if (value[i] == '0')
                    tmp.push_back(0);
            }
            mat.push_back(tmp);
            tmp.clear();
        }
    }
    else
    {
        cout << "error" << endl;
    }
    return mat;
}

int* getSliceSize(int n, int x, int y){
    vector<vector<int> > res;
    int a, b;

    a = sqrt(n);
    int i = 0;
    while(a >= 1){
        res.push_back(vector<int>());
        while(n%a != 0)
            a--;
        b = n/a;
        if(y > x){
            res[i].push_back(a);
            res[i].push_back(b);
        }
        else{
            res[i].push_back(b);
            res[i].push_back(a);
        }
        a--;
        i++;
    }
    float quotient = (float)x/(float)y;
    int j = -1;
    float current;
    for (int i = 0; i < res.size(); i++){
        if (j == -1 || fabs(quotient-(float)res[i][0]/(float)res[i][1]) < current){
            j = i;
            current = fabs(quotient-(float)res[i][0]/(float)res[i][1]);
        }
    }
    int* finalRes = new int[2];
    finalRes[0] = res[j][0];
    finalRes[1] = res[j][1];
    return finalRes;
}

int* getSlice(vector<vector<int> >& mat, int a, int b, int i, int j){
    int* res = new int[4];

    //the width of a portion
    int x = mat.size()/a;
    //the eventual rest
    int moreX = mat.size()%a;

    int y = mat.at(0).size()/b;
    int moreY = mat.at(0).size()%b;

    res[0] = i*x;
    res[1] = j*y;

    //adjusting the value if there is a rest
    if (i >= moreX)
        res[0] += moreX;
    else if (i < moreX)
        res[0] += i;
    if (j >= moreY)
        res[1] += moreY;
    else if (j < moreY)
        res[1] += j;

    res[2] = res[0] + x;
    res[3] = res[1] + y;

    if (i < moreX)
        res[2]++;
    if (j < moreY)
        res[3]++;

    return res;
}

int mod(int a, int b){
    if (a < 0)
        return b-1;
    else if (a >= b)
        return 0;
    else
        return a;
}

int* getNeighbours(int* sliceSize, int n){
    int* neighbours = new int[8];

    int a = n/sliceSize[1];
    int b = n%sliceSize[1];

    neighbours[0] = mod((a-1), sliceSize[0])*(sliceSize[1])+(b);
    neighbours[1] = mod((a+1), sliceSize[0])*(sliceSize[1])+(b);
    neighbours[2] = (a)*(sliceSize[1])+mod((b-1), sliceSize[1]);
    neighbours[3] = (a)*(sliceSize[1])+mod((b+1), sliceSize[1]);
    neighbours[4] = mod((a-1), sliceSize[0])*(sliceSize[1])+mod((b-1), sliceSize[1]);
    neighbours[5] = mod((a+1), sliceSize[0])*(sliceSize[1])+mod((b+1), sliceSize[1]);
    neighbours[6] = mod((a-1), sliceSize[0])*(sliceSize[1])+mod((b+1), sliceSize[1]);
    neighbours[7] = mod((a+1), sliceSize[0])*(sliceSize[1])+mod((b-1), sliceSize[1]);

    return neighbours;
}

void computeM(int** nMat, int x, int y){
    int k = 0;
    for (int i = 1; i<x+1; i++){
        for (int j = 1; j < y+1; j++){
            k = 0;
            if (nMat[i-1][j-1])
                k++;
            if (nMat[i-1][j])
                k++;
            if (nMat[i-1][j+1])
                k++;
            if (nMat[i][j-1])
                k++;
            if (nMat[i][j+1])
                k++;
            if (nMat[i+1][j-1])
                k++;
            if (nMat[i+1][j])
                k++;
            if (nMat[i+1][j+1])
                k++;

            if (nMat[i][j]){
                //cell alive
                if (k==3 || k==2)
                    nMat[i][j] = true;
                else
                    nMat[i][j] = false;
            }
            else{
                //cell dead
                if (k==3)
                    nMat[i][j] = true;
                else
                    nMat[i][j] = false;
            }
        }
    }
}

string itoc(int i){
    stringstream out;
    out << i;
    return out.str();
}

void saveImg(vector<vector<int> > mat, int n, int s){
    string path = "./"+itoc(n)+".pbm";
    ofstream stream(path.c_str());
    if(stream)
    {
        string line;
        stream<<"P1\n"<<mat.at(0).size()*s<<" "<<mat.size()*s<<"\n";
        for(unsigned int i = 0; i < (int)mat.size(); i++){
            line = "";
            for(unsigned int j = 0; j < (int)mat.at(0).size(); j++){
                if (!mat[i][j])
                    for (unsigned int k = 0; k<s; k++)
                        line = line + "1 ";
                else
                    for (unsigned int k = 0; k<s; k++)
                        line = line + "0 ";
            }
            for (unsigned int k = 0; k<s; k++)
                stream<<line<<"\n";
        }
    }
}

int main(int argc, char **argv) {

    chrono::milliseconds msStart = chrono::duration_cast< chrono::milliseconds >(chrono::high_resolution_clock::now().time_since_epoch());

    int PROCESS_ID, NB_PROCESS, BASE_NB_PROCESS, len;
    char name[MPI_MAX_PROCESSOR_NAME];
    int x, y, a, b;
    int N = atoi(argv[2]);
    int inter = 0;
    int factor = 1;
    if (argc >= 4){
        inter = atoi(argv[3]);
    }
    if (argc >= 5){
        factor = atoi(argv[4]);
        if (factor <1 )
            factor = 1;
    }

    vector<vector<int> > mat = parseMatrix(argv[1]);

    MPI_Init(&argc, &argv);
    double procStart = MPI_Wtime();
    MPI_Comm_rank(MPI_COMM_WORLD, &PROCESS_ID);
    MPI_Comm_size(MPI_COMM_WORLD, &NB_PROCESS);
    BASE_NB_PROCESS = NB_PROCESS;
    MPI_Get_processor_name(name, &len);

    if (PROCESS_ID == 0)
        saveImg(mat, 0, factor);

    MPI_Group world_group;
    MPI_Comm_group(MPI_COMM_WORLD, &world_group);
    int* sliceSize = getSliceSize(NB_PROCESS, mat.size(), mat.at(0).size());

    if (mat.size() > mat.at(0).size())
        swap(sliceSize[0], sliceSize[1]);

    while (sliceSize[0] > mat.size() || sliceSize[1] > mat.at(0).size()){
        NB_PROCESS--;
        sliceSize = getSliceSize(NB_PROCESS, mat.size(), mat.at(0).size());
        if (mat.size() > mat.at(0).size())
            swap(sliceSize[0], sliceSize[1]);
    }

    MPI_Comm newworld;
    if (BASE_NB_PROCESS != NB_PROCESS){
        MPI_Group new_group;
        int range[1][3];
        range[0][0] = NB_PROCESS;
        range[0][1] = BASE_NB_PROCESS-1;
        range[0][2] = 1;
        MPI_Group_range_excl(world_group, 1, range, &new_group);
        MPI_Comm_create(MPI_COMM_WORLD, new_group, &newworld);
    }
    else{
        MPI_Comm_create(MPI_COMM_WORLD, world_group, &newworld);
    }


    if (PROCESS_ID >= NB_PROCESS){
        MPI_Finalize();
        exit(0);
    }

    int** allSlice = NULL;
    int* slice = new int[4];

    if (PROCESS_ID == 0) {
        allSlice = new int*[NB_PROCESS];
        int k = 0;
        for(unsigned int i = 0; i < sliceSize[0]; i++){
            for(unsigned int j = 0; j < sliceSize[1]; j++){
                allSlice[k] = getSlice(mat, sliceSize[0], sliceSize[1], i, j);
                k++;
            }
        }
        slice = allSlice[0];
    }
    else{
        a = PROCESS_ID/sliceSize[1];
        b = PROCESS_ID%sliceSize[1];

        slice = getSlice(mat, sliceSize[0], sliceSize[1] , a, b);
    }

    x = slice[2]-slice[0];
    y = slice[3]-slice[1];

    int maxX;
    int maxY;

    if (PROCESS_ID == 0) {
        maxX = x;
        maxY = y;
    }

    MPI_Bcast(&maxX, 1, MPI_INT, 0, newworld);
    MPI_Bcast(&maxY, 1, MPI_INT, 0, newworld);

    int** nMat = new int*[x+2];
    for(int i = 0; i < x+2; i++)
        nMat[i] = new int[y+2];


    for(unsigned int i = 0; i < x+2; i++){
        for(unsigned int j = 0; j < y+2; j++){
                a = slice[0]-1+i;
                b = slice[1]-1+j;
                a = mod(a, (int) mat.size());
                b = mod(b, (int) mat.at(0).size());
            nMat[i][j] = mat[a][b];
        }
    }

    int* neighbours = getNeighbours(sliceSize, PROCESS_ID);

    MPI_Status status;

    int rep;
    int* buf = new int[NB_PROCESS];
    int* sndX = new int[x];
    int* rcvX = new int[x];

    for (int o = 0; o < N; o++){

        computeM(nMat, x, y);

        MPI_Sendrecv(nMat[1]+1, y, MPI_INT, neighbours[0], 0, nMat[x+1]+1, y, MPI_INT, neighbours[1], 0, newworld, &status);

        MPI_Sendrecv(nMat[x]+1, y, MPI_INT, neighbours[1], 0, nMat[0]+1, y, MPI_INT, neighbours[0], 0, newworld, &status);

        for (int i = 1; i < x+1; i++){
            sndX[i-1] = nMat[i][1];
        }

        MPI_Sendrecv(sndX, x, MPI_INT, neighbours[2], 0, rcvX, x, MPI_INT, neighbours[3], 0, newworld, &status);

        for (int i = 1; i < x+1; i++){
            nMat[i][y+1] = rcvX[i-1];
            sndX[i-1] = nMat[i][y];
        }

        MPI_Sendrecv(sndX, x, MPI_INT, neighbours[3], 0, rcvX, x, MPI_INT, neighbours[2], 0, newworld, &status);

        for (int i = 1; i < x+1; i++){
            nMat[i][0] = rcvX[i-1];
        }

        MPI_Sendrecv(&nMat[1][1], 1, MPI_INT, neighbours[4], 0, &nMat[x+1][y+1], 1, MPI_INT, neighbours[5], 0, newworld, &status);
        MPI_Sendrecv(&nMat[x][y], 1, MPI_INT, neighbours[5], 0, &nMat[0][0], 1, MPI_INT, neighbours[4], 0, newworld, &status);
        MPI_Sendrecv(&nMat[1][y], 1, MPI_INT, neighbours[6], 0, &nMat[x+1][0], 1, MPI_INT, neighbours[7], 0, newworld, &status);
        MPI_Sendrecv(&nMat[x][1], 1, MPI_INT, neighbours[7], 0, &nMat[0][y+1], 1, MPI_INT, neighbours[6], 0, newworld, &status);

        if ((inter != 0 && (o+1)%inter == 0) || o == N-1){
            for(unsigned int i = 0; i < maxX; i++){
                for(unsigned int j = 0; j < maxY; j++){
                    if (i+1 < x && j+1 < y)
                        rep = nMat[i+1][j+1];
                    else
                         rep = 2;
                    MPI_Gather(&rep, 1, MPI_INT, buf, 1, MPI_INT, 0, newworld);
                    if (PROCESS_ID == 0){
                        for(unsigned int k = 0; k < NB_PROCESS; k++){
                            if (buf[k] != 2)
                                mat[allSlice[k][0]+i][allSlice[k][1]+j] = buf[k];
                        }
                    }
                }
            }
            if (PROCESS_ID == 0 && o != N-1)
                saveImg(mat, o+1, factor);
        }
    }
    double  executionTime = (MPI_Wtime()-procStart);
    double* executionTimes = new double[NB_PROCESS];
    MPI_Gather(&executionTime, 1, MPI_DOUBLE, executionTimes, 1, MPI_DOUBLE, 0, newworld);
    MPI_Finalize();

    if (PROCESS_ID == 0){
        cout<<"Time spent :"<<endl;
        for(unsigned int k = 0; k < NB_PROCESS; k++){
            cout<<"Process "<<k<<" : "<<executionTimes[k]*1000<<" ms"<<endl;
        }
    }
    chrono::milliseconds msEnd = chrono::duration_cast< chrono::milliseconds >(chrono::high_resolution_clock::now().time_since_epoch());
    if(PROCESS_ID == 0){
        saveImg(mat, N, factor);
        cout<<"Total time : "<<msEnd.count()-msStart.count()<<" ms"<<endl;
        cout<<BASE_NB_PROCESS-NB_PROCESS<<" process discarded"<<endl;
    }

    exit(0);
}
