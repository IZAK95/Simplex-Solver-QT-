#include <QCoreApplication>

#include <iostream>

using namespace std;

//max function 4(x1) + 6(x2)
//st.  -x1 + x2 + x3 = 11
//      x1 + x2 + x4 = 27
//     2x1 + 5x2 + x5 = 90

double b[3] = {11,27, 90}; // initial basis
double c[5] = {4, 6, 0, 0, 0}; // cost matrix
double A[3][5] = {-1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 2, 5, 0, 0, 1};

int main()
{
    cout << "Simplex Method" << endl;

    double xb[3] = {11, 27, 90};//x3, x4, x5

    bool x1Enter = false, x2Enter = false;//hou rekord van watter vektpr die basis moet betree
    double Ax[3];
    int k, r;//tellers

    // bepaal watter vektor moet die basis betree, dis so eenvoudig aangeisne hierdie slegs die eerste iterasie is, dws, Cb = 0
    cout << "\n[" << c[0] << " " << c[1] << "]" << endl;
    if(c[1] > c[2])
    {
        cout << "x1 Needs to enter base matrix\n" << endl;
        x1Enter = true;
        for(k=0;k<3;k++)
        {
            Ax[k] = A[k][1];//bepaal waardeur xb gedeel moet word
        }//end for
    }//end if
    else
    {
        cout << "x2 Needs to enter base matrix\n" << endl;
        x2Enter = true;
        for(k=0;k<3;k++)
        {
            Ax[k] = A[k][2];
        }//end for
    }

    //bepaal watter vektor die basis moet verlaat
    double kleinsteGetal = 99999999;
    for(r=0;r<3;r++)
    {
        if(((xb[r]/Ax[r]) < kleinsteGetal) && ((xb[r]/Ax[r])>0))
            kleinsteGetal = r;
        //cout << xb[r] << "\t" << Ax[r] << "\t" << xb[r]/Ax[r] << endl;

    }//end for


    if(kleinsteGetal == 0)
        cout << "x3 Must leave the base\n";
    else if(kleinsteGetal == 1)
        cout << "x4 Must leave the base\n";
    else if(kleinsteGetal == 2)
        cout << "x5 Must leave the base\n";
    else cout << "ERROR ...";


    return 0;
}
