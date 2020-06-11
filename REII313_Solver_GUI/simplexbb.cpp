#include "simplexbb.h"

SimplexBB::SimplexBB()
{

}

SimplexBB::SimplexBB(vector<vector<float> > a, vector<float> b, vector<float> c, vector<string> ing, string P){

    problem = P;
    rows = a.size();
    cols = a[0].size();

    A.resize( rows , vector<float>( cols + rows , 0 ) );
    B.resize( b.size() );
    C.resize( cols + rows , 0 );
    Inegalite.resize( rows );

    Solution.resize( cols , 0);
    BasicSolution.resize( c.size() + b.size(), 0 );


    for(int i=0 ; i<rows ; i++)
        for(int j= 0; j< cols;j++ )
            A[i][j] = a[i][j];

    if(problem == "Max"){
        for(int i=0 ; i< cols ; i++ )
            C[i] = c[i];
    }

    else if(problem == "Min"){
        for(int i=0 ; i< cols ; i++ )
            C[i] = -c[i];
    }

    for(int i=0 ; i< rows ; i++ )
        B[i] = b[i];

    for(int i=0 ; i< rows ; i++ )
        Inegalite[i] = ing[i];

    StandarForme();
}

void SimplexBB::Solve(){

    if(towPhase){
        if(PhaseOne()) PivotageSimplex();
        else           {
            realisable = false;
            cout<<"Non realisable."<<endl;
        }
    }
    else  PivotageSimplex();
}

void SimplexBB::StandarForme(){
    BaseIndex.resize( rows );

    for(int i=0 ; i<rows ; i++)
        for(int j=cols ; j< cols+rows ; j++)
            if( i == j - cols){
                if     (Inegalite[i] == "<="){
                    A[i][j] = 1;
                    BaseIndex[i] = j;
                }
                else if(Inegalite[i] == ">=")  A[i][j] =-1;
            }
    for(int i=0 ; i<rows ; i++)
        if(Inegalite[i] == ">="){
            towPhase = true;
            for(int j=0 ; j<rows ; j++)
                A[j].resize(A[j].size()+1,0);
            A[i][A[i].size()-1]=1;
            BaseIndex[i] = A[i].size()-1;
        }
}

bool SimplexBB::PhaseOne(){
    printff();
    W.resize( A[0].size() ,0);
    ObjW = 0;

    for(int i=0 ; i<rows ; i++){
        if(Inegalite[i] == ">="){
            W[BaseIndex[i]] = -1;
        }
    }


    for(int i=0 ; i<rows ; i++){
        if(Inegalite[i] == ">="){
            ObjW += B[i];
            for(int j=0 ; j<A[i].size() ; j++){
                W[j] += A[i][j];
            }

        }
    }

    while(!ChekOptimalSimplex(W)){
        printff();
        int *Pivot = GetPivot(W);

        float valPivot = A[Pivot[0]][Pivot[1]];
        float   bPivot = B[Pivot[0]];
        float   wPivot = W[Pivot[1]];

        for(int i=0 ; i<A[0].size() ; i++)
            A[Pivot[0]][i] = A[Pivot[0]][i]/valPivot;

        B[Pivot[0]] = B[Pivot[0]]/valPivot;

        for(int i=0 ; i<rows ; i++)
            if(i != Pivot[0] )
                if(A[i][Pivot[1]] != 0){
                    float currentPivot = A[i][Pivot[1]];

                    for(int j=0 ; j<A[0].size() ; j++)
                        A[i][j] = A[i][j] - A[Pivot[0]][j]*currentPivot;

                    B[i] = B[i] - B[Pivot[0]]*currentPivot;
                }
        BaseIndex[Pivot[0]] = Pivot[1];

        if(W[Pivot[1]] != 0){
            for(int j=0 ; j<A[0].size() ; j++)
                W[j] = W[j] - wPivot*A[Pivot[0]][j];
            ObjW -= wPivot*B[Pivot[0]];
        }

        for(int i=0 ; i<rows ; i++)
            BasicSolution[BaseIndex[i]] = B[i];

        for(int i=0 ; i<cols ; i++)
            Solution[i] = BasicSolution[i];

    }

    if(ObjW == 0){
        for(int i=0 ; i<rows ; i++){
            if(Inegalite[i] == ">="){
                for(int j=0; j<rows ; j++)
                    A[j].resize(A[j].size()-1);
            }
        }

        for(int i=0 ; i<rows ; i++){
            if(C[BaseIndex[i]] != 0){
                float cCurrent = C[BaseIndex[i]];
                for(int j=0 ; j<C.size() ; j++)
                    C[j] = C[j] - cCurrent*A[i][j];
                ObjectifFunction -= cCurrent*B[i];
            }
        }
        return true;
    }
    else
        return false;
}

void SimplexBB::PivotageSimplex(){

    while(!ChekOptimalSimplex(C)){
        printtff();
        int *Pivot = GetPivot(C);

        float valPivot = A[Pivot[0]][Pivot[1]];
        float   bPivot = B[Pivot[0]];
        float   cPivot = C[Pivot[1]];

        for(int i=0 ; i<cols+rows ; i++)
            A[Pivot[0]][i] = A[Pivot[0]][i]/valPivot;

        B[Pivot[0]] = B[Pivot[0]]/valPivot;

        for(int i=0 ; i<rows ; i++)
            if(i != Pivot[0] )
                if(A[i][Pivot[1]] != 0){
                    float currentPivot = A[i][Pivot[1]];

                    for(int j=0 ; j<cols+rows ; j++)
                        A[i][j] = A[i][j] - A[Pivot[0]][j]*currentPivot;

                    B[i] = B[i] - B[Pivot[0]]*currentPivot;
                }
        BaseIndex[Pivot[0]] = Pivot[1];

        if(C[Pivot[1]] != 0){
            for(int j=0 ; j<cols+rows ; j++)
                C[j] = C[j] - cPivot*A[Pivot[0]][j];
            ObjectifFunction -= cPivot*B[Pivot[0]];
        }
        for(int i=0 ; i<rows ; i++)
            BasicSolution[BaseIndex[i]] = B[i];

        for(int i=0 ; i<cols ; i++)
            Solution[i] = BasicSolution[i];
    }


    if(problem == "Max")
        ObjectifFunction = -ObjectifFunction;
    printtff();

}

bool SimplexBB::ChekOptimalSimplex(vector<float> T){
    for(int i=0 ; i<T.size() ; i++)
        if(T[i] > 0)
            return false;
    return true;
}

int *SimplexBB::GetPivot(vector<float> G){
    int *Pivot = new int[2];
    vector <float> tmp(B.size(),-1);

    Pivot[0] = -1;
    Pivot[1] = -1;

    Pivot[1] = IndexMinPositiveStrinct(G);
    //Pivot[1] = max_element(G.begin(),G.end()) - G.begin();

    for(int i=0; i<rows ; i++)
        if(A[i][Pivot[1]]>0)
            tmp[i] = B[i]/A[i][Pivot[1]];

    Pivot[0] = IndexMinPositive(tmp);

    return Pivot;
}

int SimplexBB::IndexMinPositive(vector<float> V){
    int indexMin = max_element(V.begin(),V.end()) - V.begin();

    for(int i=0; i<V.size() ; i++)
        if(V[i]>=0 && V[i]<V[indexMin])
            indexMin = i;

    return (V[indexMin]>=0) ? indexMin : -1 ;
}

int SimplexBB::IndexMinPositiveStrinct(vector<float> V){
    int indexMin = max_element(V.begin(),V.end())-V.begin();

    for(int i=0; i<V.size() ; i++)
        if(V[i]>0 && V[i]<V[indexMin])
            indexMin = i;

    return (V[indexMin]>0) ? indexMin : -1 ;
}

void SimplexBB::print(){
    cout<<"\n\n";
    for(int j=0 ; j<A.size() ; j++){
        for(int i=0 ; i<A[0].size() ; i++)
            printf("    %4.2f",A[j][i]);
        printf("       %4.2f\n",B[j]);
    }
    cout<<""<<endl;
    for(int i=0 ; i<cols+rows ; i++)
        printf("    %4.2f",C[i]);

    printf("   Z*=%4.2f",ObjectifFunction);

    cout<<"\n\n"<<"l\'indice de la base :\n    I = { ";
    for(int i=0 ; i<rows ; i++){
        cout<<BaseIndex[i];
        if(i != rows-1) cout<<", ";
    }
    cout<<"}"<<endl;

    cout<<"\n"<<"Solution de base :\n    S = { ";
    for(int i=0 ; i<rows+cols ; i++){
        cout<<BasicSolution[i];
        if(i != rows+cols-1) cout<<", ";
    }
    cout<<"}"<<endl;

    cout<<"\n"<<"Solution : "<<endl;
    for(int i=0 ; i<cols ; i++)
        cout<<"   Variable_"<<i+1<<" = "<<Solution[i]<<endl;
    cout<<"\nMax Z = "<<ObjectifFunction<<endl;

}

void SimplexBB::printff(){

    cout<<"\n\n_____Resolution Branch and Bound_____\n"<<endl;
    for(int j=0 ; j<A.size() ; j++){
        for(int i=0 ; i<A[0].size() ; i++)
            printf("    %4.2f",A[j][i]);
        cout<<"  "<<Inegalite[j];
        printf("       %4.2f\n",B[j]);
    }
    cout<<""<<endl;
    for(int i=0 ; i<cols ; i++)
        printf("    %4.2f",C[i]);
    cout<<"\n"<<endl;
    for(int i=0; i<Solution.size() ; i++)
        cout<<"x"<<i<<" = "<<Solution[i]<<endl;
    cout<<"\nMax = "<<ObjectifFunction<<"\n\n"<<endl;

}

void SimplexBB::printtff(){
    cout<<"\n\n";
    for(int j=0 ; j<A.size() ; j++){
        for(int i=0 ; i<A[0].size() ; i++)
            printf("    %4.2f",A[j][i]);
        printf("       %4.2f\n",B[j]);
    }
    cout<<""<<endl;
    for(int i=0 ; i<W.size() ; i++)
        printf("    %4.2f",W[i]);

    printf("    W=%4.2f",ObjW);

    cout<<"\n\n"<<"l\'indice de la base :\n    I = { ";
    for(int i=0 ; i<rows ; i++){
        cout<<BaseIndex[i];
        if(i != rows-1) cout<<", ";
    }
    cout<<"}"<<endl;

    cout<<"\n"<<"Solution de base :\n    S = { ";
    for(int i=0 ; i<rows+cols ; i++){
        cout<<BasicSolution[i];
        if(i != rows+cols-1) cout<<", ";
    }
    cout<<"}"<<endl;
}
