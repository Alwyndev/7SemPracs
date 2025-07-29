// 15-07-2025

#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Hill Cipher functions
static int determinant2x2(const vector<vector<int>>& matrix) {
    return (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]) % 26;
}

static int modInverse(int a, int m) {
    a = ((a % m) + m) % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) return x;
    }
    return -1;
}

static vector<vector<int>> inverse2x2(const vector<vector<int>>& matrix) {
    int det = determinant2x2(matrix);
    int detInv = modInverse(det, 26);
    if (detInv == -1) return {{-1, -1}, {-1, -1}};
    
    vector<vector<int>> inv(2, vector<int>(2));
    inv[0][0] = (matrix[1][1] * detInv) % 26;
    inv[0][1] = (-matrix[0][1] * detInv % 26 + 26) % 26;
    inv[1][0] = (-matrix[1][0] * detInv % 26 + 26) % 26;
    inv[1][1] = (matrix[0][0] * detInv) % 26;
    
    return inv;
}

static string prepareHillMessage(const string& message) {
    string cleaned;
    for (char c : message) {
        if (isalpha(c)) {
            cleaned += toupper(c);
        }
    }
    if (cleaned.length() % 2 != 0) {
        cleaned += 'X';
    }
    return cleaned;
}

string HillCipher(const string& message, const vector<vector<int>>& key) {
    string prepared = prepareHillMessage(message);
    string cipher;
    
    for (size_t i = 0; i < prepared.length(); i += 2) {
        int p1 = prepared[i] - 'A';
        int p2 = prepared[i + 1] - 'A';
        
        int c1 = (key[0][0] * p1 + key[0][1] * p2) % 26;
        int c2 = (key[1][0] * p1 + key[1][1] * p2) % 26;
        
        cipher += (char)(c1 + 'A');
        cipher += (char)(c2 + 'A');
    }
    
    return cipher;
}

string HillDecipher(const string& cipher, const vector<vector<int>>& key) {
    vector<vector<int>> invKey = inverse2x2(key);
    if (invKey[0][0] == -1) return "Key not invertible!";
    
    string plain;
    
    for (size_t i = 0; i < cipher.length(); i += 2) {
        int c1 = cipher[i] - 'A';
        int c2 = cipher[i + 1] - 'A';
        
        int p1 = (invKey[0][0] * c1 + invKey[0][1] * c2) % 26;
        int p2 = (invKey[1][0] * c1 + invKey[1][1] * c2) % 26;
        
        plain += (char)(p1 + 'A');
        plain += (char)(p2 + 'A');
    }
    
    return plain;
}

// Build 5×5 key matrix (remove dupes, map J→I)
static void buildKeyMatrix(const string &key, char keyMat[5][5]) {
    bool used[26] = {};
    string filtered;
    filtered.reserve(key.size());
    for (char ch : key) {
        if (!isalpha(ch)) continue;
        ch = toupper(ch);
        if (ch == 'J') ch = 'I';
        if (!used[ch - 'A']) {
            used[ch - 'A'] = true;
            filtered += ch;
        }
    }
    int idx = 0;
    for (char ch : filtered) {
        keyMat[idx/5][idx%5] = ch;
        idx++;
    }
    for (char ch = 'A'; ch <= 'Z'; ch++) {
        if (ch == 'J' || used[ch - 'A']) continue;
        keyMat[idx/5][idx%5] = ch;
        idx++;
    }
}

// O(1) lookup position in matrix
static inline void findPos(const char keyMat[5][5], char ch, int &r, int &c) {
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            if (keyMat[i][j] == ch) {
                r = i; c = j;
                return;
            }
}

// Prepare plaintext: uppercase, J→I, insert X between duplicates & pad
static string prepareMessage(const string &msg) {
    string tmp;
    tmp.reserve(msg.size());
    for (char ch : msg) {
        if (!isalpha(ch)) continue;
        ch = toupper(ch);
        if (ch == 'J') ch = 'I';
        tmp += ch;
    }

    string out;
    out.reserve(tmp.size()+1);
    for (size_t i = 0; i < tmp.size(); i++) {
        out += tmp[i];
        if (i+1 < tmp.size() && tmp[i] == tmp[i+1])
            out += 'X';
    }
    if (out.size() % 2) out += 'X';
    return out;
}

string PlayfairCipher(const string &message, const string &key) {
    char keyMat[5][5];
    buildKeyMatrix(key, keyMat);

    string msg = prepareMessage(message);
    string cipher;
    cipher.reserve(msg.size());

    for (size_t i = 0; i < msg.size(); i += 2) {
        int r1,c1,r2,c2;
        findPos(keyMat, msg[i],   r1, c1);
        findPos(keyMat, msg[i+1], r2, c2);

        if (r1 == r2) {
            cipher += keyMat[r1][(c1 + 1) % 5];
            cipher += keyMat[r2][(c2 + 1) % 5];
        }
        else if (c1 == c2) {
            cipher += keyMat[(r1 + 1) % 5][c1];
            cipher += keyMat[(r2 + 1) % 5][c2];
        }
        else {
            cipher += keyMat[r1][c2];
            cipher += keyMat[r2][c1];
        }
    }
    return cipher;
}

string PlayfairDecipher(const string &cipher, const string &key) {
    char keyMat[5][5];
    buildKeyMatrix(key, keyMat);

    string plain;
    plain.reserve(cipher.size());

    for (size_t i = 0; i < cipher.size(); i += 2) {
        int r1,c1,r2,c2;
        findPos(keyMat, cipher[i],   r1, c1);
        findPos(keyMat, cipher[i+1], r2, c2);

        if (r1 == r2) {
            plain += keyMat[r1][(c1 + 4) % 5];
            plain += keyMat[r2][(c2 + 4) % 5];
        }
        else if (c1 == c2) {
            plain += keyMat[(r1 + 4) % 5][c1];
            plain += keyMat[(r2 + 4) % 5][c2];
        }
        else {
            plain += keyMat[r1][c2];
            plain += keyMat[r2][c1];
        }
    }
    return plain;
}

// Caesar Cipher functions
string encipherCeaserCipher(string message, int key){
    string cipher_text = "";
    for (int i = 0; i < message.length(); i++ ){
        char ch = message[i];
        if (isalpha(ch)) {
            char base = isupper(ch) ? 'A' : 'a';
            ch = (ch - base + key) % 26 + base; // shift by key
        }
        cipher_text += ch;
    }
    return cipher_text;
}

string decipherCeaserCipher(string cipher_text, int key){
    string message = "";
    for(int i = 0; i < cipher_text.length(); i++){
        char ch = cipher_text[i];
        if(isalpha(ch)){
            char base = isupper(ch)? 'A' : 'a';
            ch = (ch - base - key + 26) % 26 + base;
        }
        message += ch;
    }
    return message;
}

// Vigenère Cipher functions
string encipherVinereCipher(string message, string key){
    string cipher_text = "";
    for(int i = 0; i < message.length(); i++){
        char ch = message[i];
        char k = key[i % key.length()];
        if (isalpha(ch)){
            char msg_base = isupper(ch)? 'A' : 'a';
            char key_base = isupper(k)? 'A' : 'a';
            if(key_base == msg_base){
                ch = (ch - msg_base + (k - key_base)) % 26 + msg_base;
            }
            else{
                cout<<"\nMessage and key should be in the same case!";
                break;
            }
        }
        cipher_text += ch;
    } 
    return cipher_text;
}

string decipherVinereCipher(string cipher, string key){
    string plain_text = "";
    for(int i = 0; i < cipher.length(); i++){
        char ch = cipher[i];
        char k = key[i % key.length()];
        if (isalpha(ch)){
            char msg_base = isupper(ch)? 'A' : 'a';
            char key_base = isupper(k)? 'A' : 'a';
            if(key_base == msg_base){
                ch = ((ch - msg_base - (k - key_base )) + 26) % 26 + msg_base;
            }
            else{
                cout<<"\nMessage and key should be in the same case!";
                break;
            }
        }
        plain_text += ch;
    } 
    return plain_text;
}

// Affine Cipher functions
string affineCipher(string message, int key1, int key2) {
    string cipher_text = "";
    for (int i = 0; i < message.length(); i++ ){
        char ch = message[i];
        if (isalpha(ch)) {
            char base = isupper(ch) ? 'A' : 'a';
            ch = (key1 * (ch - base) + key2) % 26 + base; // apply affine formula
        }
        cipher_text += ch;
    }
    return cipher_text;
}

string affineDecipher(string cipher_text, int key1, int key2) {
    string message = "";
    int key1_inv = modInverse(key1, 26); // find modular inverse of key1
    for (int i = 0; i < cipher_text.length(); i++ ){
        char ch = cipher_text[i];
        if (isalpha(ch)) {
            char base = isupper(ch) ? 'A' : 'a';
            ch = (key1_inv * ((ch - base) - key2 + 26)) % 26 + base; // apply inverse affine formula
        }
        message += ch;
    }
    return message;
}

// Vernam Cipher functions
string classicVernamCipher(string message, string key) {
    string cipher_text = "";
    for (size_t i = 0; i < message.length(); i++) {
        char ch = message[i];
        char k = key[i % key.length()]; // repeat key as necessary
        if (isalpha(ch) && isalpha(k)) {
            char base = isupper(ch) ? 'A' : 'a';
            ch = (ch - base + (k - base)) % 26 + base;
        }
        cipher_text += ch;
    }
    return cipher_text;
}

string classicVernamDecipher(string cipher_text, string key) {
    string message = "";
    for (size_t i = 0; i < cipher_text.length(); i++) {
        char ch = cipher_text[i];
        char k = key[i % key.length()]; // repeat key as necessary
        if (isalpha(ch) && isalpha(k)) {
            char base = isupper(ch) ? 'A' : 'a';
            ch = (ch - base - (k - base) + 26) % 26 + base;
        }
        message += ch;
    }
    return message;
}

// string modifiedVernamCipher(string message, string key){

// }

int main() {
    int cipherType;
    cout << "=== Cipher Type Menu ===\n";
    cout << "1. Monoalphabetic Cipher\n";
    cout << "2. Polyalphabetic Cipher\n";
    cout << "Enter choice (1-2): ";
    cin >> cipherType;
    cin.ignore(); // Clear input buffer

    if (cipherType == 1) {
        int choice;
        cout << "=== Monoalphabetic Cipher Menu ===\n";
        cout << "1. Caesar Cipher\n";
        cout << "2. Affine Cipher\n";
        cout << "Enter choice (1-2): ";
        cin >> choice;
        cin.ignore();

        string message;
        cout << "Enter the Message: ";
        getline(cin, message);

        switch (choice) {
            case 1: {
                int key;
                cout << "Enter key (1-25): ";
                cin >> key;

                string cipher = encipherCeaserCipher(message, key);
                cout << "\nCaesar Encoded: " << cipher;
                cout << "\nCaesar Decoded: " << decipherCeaserCipher(cipher, key);
                break;
            }
            case 2: {
                int key1, key2;
                cout << "Enter key1 (multiplicative key): ";
                cin >> key1;
                cout << "Enter key2 (additive key): ";
                cin >> key2;

                string cipher = affineCipher(message, key1 % 26, key2 % 26);
                cout << "\nAffine Encoded: " << cipher;
                cout << "\nAffine Decoded: " << affineDecipher(cipher, key1 % 26, key2 % 26);
                break;
            }
            default:
                cout << "Invalid choice! Please select 1-2.";
        }
    } else if (cipherType == 2) {
        int choice;
        cout << "=== Polyalphabetic Cipher Menu ===\n";
        cout << "1. Playfair Cipher\n";
        cout << "2. Hill Cipher\n";
        cout << "3. Vigenere Cipher\n";
        cout << "4. Vernam Cipher\n";
        cout << "Enter choice (1-4): ";
        cin >> choice;
        cin.ignore();

        string message;
        cout << "Enter the Message: ";
        getline(cin, message);

        switch (choice) {
            case 1: {
                string key;
                cout << "Enter the key: ";
                getline(cin, key);

                string cipher = PlayfairCipher(message, key);
                cout << "\nPlayfair Encoded: " << cipher;
                cout << "\nPlayfair Decoded: " << PlayfairDecipher(cipher, key);
                break;
            }
            case 2: {
                vector<vector<int>> key(2, vector<int>(2));
                cout << "Enter 2x2 key matrix:\n";
                for (int i = 0; i < 2; i++) {
                    for (int j = 0; j < 2; j++) {
                        cout << "key[" << i << "][" << j << "]: ";
                        cin >> key[i][j];
                    }
                }

                string cipher = HillCipher(message, key);
                cout << "\nHill Encoded: " << cipher;
                cout << "\nHill Decoded: " << HillDecipher(cipher, key);
                break;
            }
            case 3: {
                string key;
                cout << "Enter key: ";
                getline(cin, key);

                string cipher = encipherVinereCipher(message, key);
                cout << "\nVigenere Encoded: " << cipher;
                cout << "\nVigenere Decoded: " << decipherVinereCipher(cipher, key);
                break;
            }
            case 4: {
                string key;
                cout << "Enter key (same length as message): ";
                getline(cin, key);

                string cipher = classicVernamCipher(message, key);
                cout << "\nVernam Encoded: " << cipher;
                cout << "\nVernam Decoded: " << classicVernamDecipher(cipher, key);
                break;
            }
            default:
                cout << "Invalid choice! Please select 1-4.";
        }
    } else {
        cout << "Invalid cipher type! Please select 1-2.";
    }

    return 0;
}