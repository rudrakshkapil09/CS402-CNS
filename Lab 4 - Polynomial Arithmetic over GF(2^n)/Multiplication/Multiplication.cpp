//Program to perform polynomial field multiplication
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

// division using the reducing polynomial as the divisor — the remainder is the product
// returns the remainder
vector<int> get_remainder(vector<int> num, vector<int> den)
{

    //get sizes of quotient and remainder
    int dq = (num.size()-1) - (den.size()-1); // second term is den.size()-1;
    int dN = num.size()-1;
    int dD = den.size()-1;
    int dd = 0;
    int dr = dN;
    vector<int> d(dN+1,0);
    vector<int> q(dq+1,0);
    vector<int> R(dr+1,0);  //to return

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
		R[i] = num[i];

    // return remainder
    return R;
}



//MAIN
int main()
{
    cout << "\n\n---------------------------------------------------\n";
    cout << "Polynomial Arithmetic over GF(2^n) - Multiplication\n";
    cout << "---------------------------------------------------\n";

    // get degrees
    int d1, d2, dn;
    cout << "Enter the degree of 1st polynomial (X): ";
    cin >> d1;
    cout << "Enter the degree of 2nd polynomial (Y): ";
    cin >> d2;
    cout << "Enter the degree of modulo polynomial (M): ";
    cin >> dn;

    //get actual polynomials
    vector<int> X(d1+1), Y(d2+1), M(dn+1);
    cout << "Enter the coefficients of X from highest power to lowest: ";
    for(int i = 0; i < d1+1; i++)
        cin >> X[d1-i];

    cout << "Enter the coefficients of Y from highest power to lowest: ";
    for(int i = 0; i < d2+1; i++)
        cin >> Y[d2-i];

    cout << "Enter the coefficients of M from highest power to lowest: ";
    for(int i = 0; i < dn+1; i++)
        cin >> M[dn-i];

    print_polynomial(X);
    print_polynomial(Y);
    print_polynomial(M);

    //print outputs
    cout << "\n\n-----Results:-----\n";
    cout << "Product before % => \n";
    vector<int> R = mul_polynomials(X, Y);
    print_polynomial(R);

    cout << "\nProduct after mod with irreducible M => \n";
    R = get_remainder(R, M);
    print_polynomial(R);
    return 0;
}
