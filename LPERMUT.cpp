/*alurquiza*/
//LPERMUT_SPOJ
#include <bits/stdc++.h>

using namespace std;

const int SIZE = 1e5 + 1,logSIZE = 18;
int RMQ[SIZE][logSIZE][2],N;

void build(){
    for(int k = 1;(1 << k) <= N;k++){
        for(int i = 1;i + (1 << k) - 1 <= N;i++){
            RMQ[i][k][0] = max(RMQ[i][k - 1][0],RMQ[i + (1 << (k - 1))][k - 1][0]);
            RMQ[i][k][1] = max(RMQ[i][k - 1][1],RMQ[i + (1 << (k - 1))][k - 1][1]);
        }
    }
}

int query(int ini,int fin,int tp){
    int k = 31 - __builtin_clz(fin - ini + 1);

    return max(RMQ[ini][k][tp],RMQ[fin - (1 << k) + 1][k][tp]);
}

vector<long long> PS(SIZE + 5);

int DyV(int ini,int fin){
    if(fin == ini){
        if(RMQ[ini][0][0] == 1)
            return 1;
        else
            return 0;
    }
    int mit = (ini + fin) >> 1;
    int sol = max(DyV(ini,mit),DyV(mit + 1,fin));

    fill(PS.begin() + ini,PS.begin() + mit + 2,0);
    for(int i = mit;i >= ini;i--)
        PS[i] = PS[i + 1] + RMQ[i][0][0];

    long long sum = 0,may = 0;
    for(int i = mit + 1;i <= fin;i++){
        may = max(may,1LL * RMQ[i][0][0]);
        sum += RMQ[i][0][0];
        long long falta = ((may + 1) * may / 2) - sum;

        int ini2 = ini,fin2 = mit + 1;
        while(ini2 < fin2){
            int mit2 = (ini2 + fin2) >> 1;

            if(PS[mit2] > falta)
                ini2 = mit2 + 1;
            else
                fin2 = mit2;
        }

        if(PS[ini2] == falta && query(ini2,i,0) == (i - ini2 + 1) && query(ini2,i,1) < ini2)
            sol = max(sol,i - ini2 + 1);
    }

    fill(PS.begin() + mit,PS.begin() + fin + 2,0);
    for(int i = mit + 1;i <= fin;i++)
        PS[i] = PS[i - 1] + RMQ[i][0][0];

    sum = 0,may = 0;
    for(int i = mit;i >= ini;i--){
        may = max(may,1LL * RMQ[i][0][0]);
        sum += RMQ[i][0][0];
        long long falta = ((may + 1) * may / 2) - sum;

        int ini2 = mit,fin2 = fin;
        while(ini2 < fin2){
            int mit2 = (ini2 + fin2 + 1) >> 1;

            if(PS[mit2] > falta)
                fin2 = mit2 - 1;
            else
                ini2 = mit2;
        }

        if(PS[ini2] == falta && query(i,ini2,0) == (ini2 - i + 1) && query(i,ini2,1) < i)
            sol = max(sol,ini2 - i + 1);
    }

    return sol;
}

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    cin >> N;

    vector<int> UA(N + 1,0),UA2(N + 1,0);
    for(int i = 1;i <= N;i++){
        cin >> RMQ[i][0][0];

        UA[i] = UA2[RMQ[i][0][0]];
        RMQ[i][0][1] = UA[i];

        UA2[RMQ[i][0][0]] = i;
    }

    build();

    cout << DyV(1,N) << '\n';

    return 0;
}
