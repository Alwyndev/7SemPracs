#include <iostream>
using namespace std;

int euclidsGCD(int a, int b){
    if (a >= b) {
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
    return a;
    }
}

int main() {
    int a, b;
    cout << "Enter the first number: ";
    cin >> a;
    cout << "Enter the second number: ";
    cin >> b;

    cout << "GCD of " << a << " and " << b << " is: " << euclidsGCD(a, b) << endl;
    return 0;
}