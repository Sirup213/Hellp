#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

int main(){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution dist(1,6);

    vector <int> S = {};
    float avg=0;

    for(int i=0; i<60; i++){
        S.push_back(dist(gen));

        sort(S.begin(), S.end());
    }
    
    for(int i=1; i<=6;i++){
        int count_i=count(S.begin(), S.end(), i);
        cout<<i<<" = "<<count_i<<endl;
        avg+=count_i;
    }

    cout<<avg/6;
}