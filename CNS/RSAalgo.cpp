#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <limits>

using namespace std;

long long gcd(long long a, long long b){
    while(b){
        long long t = b;
        b = a % b;
        a = t;
    }
    return a;
}

long long extendedGCD(long long a, long long b, long long &x, long long &y){
    if(b == 0){
        x = 1;
        y = 0;
        return a;
    }
    long long x1, y1;
    long long g = extendedGCD(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

long long modInverse(long long e, long long phi){
    long long x, y;
    long long g = extendedGCD(e, phi, x, y);
    if(g != 1) return -1;
    x %= phi;
    if(x < 0) x += phi;
    return x;
}

long long modPow(long long base, long long exp, long long mod){
    long long res = 1 % mod;
    base %= mod;
    while(exp > 0){
        if(exp & 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return res;
}

// Simple primality test (sufficient for classroom-sized inputs)
bool isPrime(long long n){
    if(n < 2) return false;
    if(n % 2 == 0) return n == 2;
    for(long long i = 3; i * i <= n; i += 2)
        if(n % i == 0) return false;
    return true;
}

/* ---------- Added helpers for tiny modulus handling ----------
   When n is small, even a single byte (0..255) may be >= n.
   Strategy:
     If n > 255: encrypt each byte directly (original behavior).
     Else: represent each byte in base B = n-1, producing L digits where
           L minimal with B^L >= 256. Each digit < B <= n-1 < n, so valid.
*/
struct EncodeMode {
    bool digitMode = false;
    int base = 0;
    int digitsPerByte = 0;
};

static int calcDigitsPerByte(int base){
    int acc = 1, d = 0;
    while(acc < 256){
        acc *= base;
        ++d;
    }
    return d;
}

static vector<long long> rsaEncryptMessage(const string& msg, long long e, long long n, EncodeMode &mode){
    vector<long long> out;
    if(n > 255){
        // Simple mode: each byte < n
        mode.digitMode = false;
        for(unsigned char c : msg){
            if(c >= n){
                // Should not happen because we checked n > 255
                throw runtime_error("Byte >= n in simple mode.");
            }
            out.push_back(modPow(c, e, n));
        }
        return out;
    }
    // Digit mode
    mode.digitMode = true;
    mode.base = static_cast<int>(n - 1);       // digits range: 0 .. base-1
    if(mode.base < 2) throw runtime_error("Modulus too small (n-1 < 2).");
    mode.digitsPerByte = calcDigitsPerByte(mode.base);
    for(unsigned char c : msg){
        int value = c;
        // Decompose into fixed-length little-endian base-(n-1) digits
        vector<int> digits(mode.digitsPerByte, 0);
        for(int i = 0; i < mode.digitsPerByte; ++i){
            digits[i] = value % mode.base;
            value /= mode.base;
        }
        // Encrypt each digit
        for(int d : digits){
            out.push_back(modPow(d, e, n));
        }
    }
    return out;
}

static string rsaDecryptMessage(const vector<long long>& cipher, long long d, long long n, const EncodeMode &mode){
    string recovered;
    if(!mode.digitMode){
        for(long long c : cipher){
            long long m = modPow(c, d, n);
            if(m < 0 || m > 255) throw runtime_error("Decrypted value out of byte range.");
            recovered.push_back(static_cast<char>(m));
        }
        return recovered;
    }
    // Digit mode
    int B = mode.base;
    int L = mode.digitsPerByte;
    if(L <= 0) throw runtime_error("Invalid digitsPerByte.");
    if(cipher.size() % L != 0) throw runtime_error("Cipher length not multiple of digitsPerByte.");
    for(size_t i = 0; i < cipher.size(); i += L){
        int value = 0;
        int mul = 1;
        for(int j = 0; j < L; ++j){
            long long digit = modPow(cipher[i + j], d, n);
            if(digit < 0 || digit >= B) throw runtime_error("Digit out of range after decryption.");
            value += static_cast<int>(digit) * mul;
            mul *= B;
        }
        if(value > 255) throw runtime_error("Reconstructed byte out of range.");
        recovered.push_back(static_cast<char>(value));
    }
    return recovered;
}

int main(){
    long long p, q;
    // Enforce primality of p
    while(true){
        cout << "Enter prime p: ";
        if(!(cin >> p)){ return 0; }
        if(isPrime(p)) break;
        cout << "p is not prime. Try again.\n";
    }
    // Enforce primality of q
    while(true){
        cout << "Enter prime q: ";
        if(!(cin >> q)){ return 0; }
        if(isPrime(q) && q != p) break;
        if(q == p) cout << "q must differ from p.\n";
        else cout << "q is not prime. Try again.\n";
    }

    long long n = p * q;
    long long phi = (p - 1) * (q - 1);

    // Prefer standard exponent 65537 if valid; else fallback search
    long long e = 65537;
    if(e >= phi || gcd(e, phi) != 1){
        e = 3;
        while(e < phi && gcd(e, phi) != 1) e += 2;
    }

    long long d = modInverse(e, phi);
    if(d == -1){
        cout << "Failed to compute modular inverse (choose different primes).\n";
        return 0;
    }

    cout << "Public key (e, n): (" << e << ", " << n << ")\n";
    cout << "Private key (d, n): (" << d << ", " << n << ")\n";

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string message;
    cout << "Enter message (ASCII): ";
    getline(cin, message);

    EncodeMode mode;
    vector<long long> cipher = rsaEncryptMessage(message, e, n, mode);

    if(mode.digitMode){
        cout << "[Digit mode active] n is too small for raw bytes.\n";
        cout << "Base (n-1): " << mode.base << ", digits/byte: " << mode.digitsPerByte << "\n";
    } else {
        cout << "[Direct byte mode]\n";
    }

    cout << "Encrypted (space-separated integers): ";
    for(auto v : cipher) cout << v << ' ';
    cout << "\n";

    try{
        string recovered = rsaDecryptMessage(cipher, d, n, mode);
        cout << "Decrypted message: " << recovered << "\n";
    } catch(const exception &ex){
        cout << "Decryption error: " << ex.what() << "\n";
    }
    return 0;
}