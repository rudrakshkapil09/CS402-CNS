//Program to perform polynomial field addition
// Rudraksh Kapil - 177154
#include <iostream>
#include <vector>
using namespace std;

//utility function to print polynomials
void print_polynomial(vector<int> R)
{
    for(int i = R.size()-1; i > 0; i--) {
        //if (R[i] == 0)
        //    continue;
        if (R[i] != 1)
            cout << R[i];
        cout << "x^" << i << " + ";
    }
    cout << R[0] << "\n";
}

// Actual function
vector<int> add_polynomials(vector<int> X, vector<int> Y)
{
    // to return
    vector<int> R;

    // pad the smaller with 0s
    while (X.size() < Y.size())
        X.insert(X.end(), 0);
    while (Y.size() < X.size())
        Y.insert(Y.end(), 0);

    // perform addition
    for (int i = 0; i < X.size(); i++)
        R.push_back(X[i]^Y[i]);

    // return
    return R;
}

//MAIN
int main()
{
    cout << "\n\n---------------------------------------------------\n";
    cout << "Polynomial Arithmetic over GF(2^n) - Addition\n";
    cout << "---------------------------------------------------\n";

    // get degrees
    int d1, d2;
    cout << "Enter the degree of 1st polynomial (X): ";
    cin >> d1;
    cout << "Enter the degree of 2nd polynomial (Y): ";
    cin >> d2;

    //get actual polynomials
    vector<int> X(d1+1), Y(d2+1);
    cout << "Enter the coefficients of X from highest power to lowest: ";
    for(int i = 0; i < d1+1; i++)
        cin >> X[d1-i];

    cout << "Enter the coefficients of Y from highest power to lowest: ";
    for(int i = 0; i < d2+1; i++)
        cin >> Y[d2-i];

    print_polynomial(X);
    print_polynomial(Y);

    //print outputs
    cout << "\n\n-----Results:-----\n";
    cout << "Sum => ";
    vector<int> R = add_polynomials(X, Y);
    for(int i = R.size()-1; i > 0; i--) {
        if (R[i] == 0)
            continue;
        if (R[i] != 1)
            cout << R[i];
        cout << "x^" << i << " + ";
    }
    cout << R[0] << "\n";
    return 0;
}
