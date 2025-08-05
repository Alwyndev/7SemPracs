#include <iostream>
#include <tuple>
using namespace std;

int euclidsGCD(int a, int b){
    if(a < b){
        swap(a, b);
    }
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

tuple<int, int, int> extendedEuclidsGCD(int a, int b){
    if(b == 0){
        return make_tuple(a, 1, 0);
    }
    tuple<int, int, int> result = extendedEuclidsGCD(b, a % b);
    int gcd = get<0>(result);
    int x1 = get<1>(result);
    int y1 = get<2>(result);
    
    int x = y1;
    int y = x1 - (a / b) * y1;
    return make_tuple(gcd, x, y);
}

int main() {
    int a, b;
    cout << "Enter the first number: ";
    cin >> a;
    cout << "Enter the second number: ";
    cin >> b;

    cout << "GCD of " << a << " and " << b << " is: " << euclidsGCD(a, b) << endl;

    tuple<int, int, int> result = extendedEuclidsGCD(a, b);
    int gcd = get<0>(result);
    int x = get<1>(result);
    int y = get<2>(result);
    
    cout << "Extended GCD: " << gcd << " = " << x << "*" << a << " + " << y << "*" << b << endl;
    return 0;
}