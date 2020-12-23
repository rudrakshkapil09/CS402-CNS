//Program to perform polynomial field Inverse
// Rudraksh Kapil - 177154
#include <iostream>
#include <vector>
using namespace std;

//utility function to print polynomials
void print_polynomial(vector<int> R)
{
    for(int i = R.size()-1; i > 0; i--) {
        if (R[i] == 0)
            continue;
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

// Actual function
vector<int> mul_polynomials(vector<int> X, vector<int> Y)
{
    //get size of result
    int dr = (X.size()-1) + (Y.size()-1) ;
    vector<int> R (dr+1, 0);

    //calculate
    for (int i = 0; i < X.size(); i++) {
        for (int j = 0; j < Y.size(); j++) {
            R[i+j] += (X[i] * Y[j]);
            R[i+j] %= 2;
        }
    }

    //return
    return R;
}

// division using the reducing polynomial as the divisor
// returns [quotient, remainder]
vector<vector<int> > div_polynomials(vector<int> num, vector<int> den)
{
    vector<vector<int> > R;

    //get sizes of quotient and remainder
    int dq = (num.size()-1) - (den.size()-1); // second term is den.size()-1;
    int dN = num.size()-1;
    int dD = den.size()-1;
    int dd = 0;
    int dr = dN;
    vector<int> d(dN+1,0);
    vector<int> q(dq+1,0);
    vector<int> r(dr+1,0);  //to return

    // reduce
	if (dN >= dD) {
		while (dN >= dD) {
			// calculating one element of q
			q[dN-dD] = num[dN] / den[dD];

			for (int i = 0; i < dD + 1; i++)
				d[i+dN-dD] = den[i] * q[dN-dD];

			for (int i = 0; i < dN + 1; i++)
				num[i] = (num[i] ^ d[i]);

			dN--;
		}
	}

	// r equals N
	for (int i = 0; i <= dN; i++)
		r[i] = num[i];

    // return remainder
    R.push_back(q);
    R.push_back(r);
    return R;
}

// does X mod Y
vector<int> mod_polynomials(vector<int> X, vector<int> Y)
{
    // reduce using above function, Y is Denominator -> we want remainder
    vector<int> R = div_polynomials(X, Y)[1];

    // trim - 1100
    while(R.size() != 0 && R[R.size()-1] == 0)
        R.pop_back();

    return R;
}

//actual GCD - euclids algo
vector<int> GCD(vector<int> X, vector<int> Y)
{
    if (Y.size() == 0)
        return X;
    else return GCD(Y, mod_polynomials(X,Y));
}

// function to calculate inverse of X mod Y
vector<int> inverse(vector<int> X, vector<int> Y)
{
    // s0 = 0
    vector<int> Spp; //s prev prev
    Spp.push_back(0);

    // s1 = 1
    vector<int> Sp; //s prev
    Sp.push_back(1);

    // loop while no remainder
    int i = 2;
    while (true) {
        // get quotient and remainder
        vector<int> Quotient = div_polynomials(Y,X)[0];
        vector<int> Remainder = mod_polynomials(Y,X);

        //check break condition
        if (Remainder.size() == 0)
            return Sp;

        cout << "\n\nQ => ";
        print_polynomial(Quotient);
        cout << "R => ";
        print_polynomial(Remainder);

        // calculate T and S values using Sp and Spp
        vector<int> T = mul_polynomials(Quotient, Sp);
        vector<int> S = add_polynomials(Spp, T);

        cout << "S => ";
        print_polynomial(S);
        cout << "T => ";
        print_polynomial(T);

        // update s values
        Spp = Sp;
        Sp = S;

        // update X and Y values
        Y = X;
        X = Remainder;

        //increment counter
        i++;
    }
}

//MAIN
int main()
{
    cout << "\n\n---------------------------------------------------\n";
    cout << "Polynomial Arithmetic over GF(2^n) - Inverse\n";
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
    cout << "Inverse of X mod Y => ";
    vector<int> R = inverse(X, Y);
    print_polynomial(R);
    return 0;
}
