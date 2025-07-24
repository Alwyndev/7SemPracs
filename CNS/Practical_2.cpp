#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

string railFenceCipher(string message){
    int len = message.length();
    int rail = 2;
    string cipher_text = "";

    for (int i = 0; i < rail; i++) {
        for (int j = i; j < len; j += 2 * (rail - 1)) {
            cipher_text += message[j];
            if (i != 0 && i != rail - 1 && j + 2 * (rail - 1 - i) < len) {
                cipher_text += message[j + 2 * (rail - 1 - i)];
            }
        }
    }
    return cipher_text;
}

string railFenceDecipher(string cipher_text){
    int len = cipher_text.length();
    int rail = 2;
    string message = "";
    
    // Create a 2D vector to represent the rail fence
    vector<vector<char>> fence(rail, vector<char>(len, '\0'));
    
    // Mark the positions where characters should be placed
    bool direction_down = false;
    int row = 0;
    
    for (int i = 0; i < len; i++) {
        // Mark the position
        fence[row][i] = '*';
        
        // Change direction at top and bottom rails
        if (row == 0 || row == rail - 1) {
            direction_down = !direction_down;
        }
        
        // Move to next row
        if (direction_down) {
            row++;
        } else {
            row--;
        }
    }
    
    // Fill the marked positions with characters from cipher_text
    int index = 0;
    for (int i = 0; i < rail; i++) {
        for (int j = 0; j < len; j++) {
            if (fence[i][j] == '*' && index < len) {
                fence[i][j] = cipher_text[index++];
            }
        }
    }
    
    // Read the fence in zigzag pattern to get original message
    direction_down = false;
    row = 0;
    
    for (int i = 0; i < len; i++) {
        // Add character to result
        message += fence[row][i];
        
        // Change direction at top and bottom rails
        if (row == 0 || row == rail - 1) {
            direction_down = !direction_down;
        }
        
        // Move to next row
        if (direction_down) {
            row++;
        } else {
            row--;
        }
    }
    
    return message;
}


string singleColumnTranspositionCipher(const string& message, const vector<int>& key) {
    int n = key.size();
    int len = message.length();
    int rows = (len + n - 1) / n; // Calculate number of rows needed
    vector<string> grid(rows, string(n, ' '));

    // Fill the grid
    for (int i = 0; i < len; i++) {
        grid[i / n][i % n] = message[i];
    }

    // Read columns in order of the key
    string cipher;
    for (int k : key) {
        for (int r = 0; r < rows; r++) {
            if (k - 1 < n && r < grid.size()) {
                cipher += grid[r][k - 1];
            }
        }
    }
    
    return cipher;
}

string singleColumnTranspositionDecipher(const string& cipher, const vector<int>& key) {
    int n = key.size();
    int len = cipher.length();
    int rows = (len + n - 1) / n; // Calculate number of rows needed
    vector<string> grid(rows, string(n, ' '));

    // Fill the grid with cipher text
    int index = 0;
    for (int k : key) {
        for (int r = 0; r < rows; r++) {
            if (k - 1 < n && index < len) {
                grid[r][k - 1] = cipher[index++];
            }
        }
    }

    // Read the grid row-wise to get the original message
    string message;
    for (const auto& row : grid) {
        message += row;
    }
    
    return message;
}

string doubleColumnTranspositionCipher(const string& message, const vector<int>& key) {
    // First transposition: write row-wise, read column-wise
    int n = key.size();
    int len = message.length();
    int rows = (len + n - 1) / n;
    vector<string> grid(rows, string(n, ' '));

    // Fill the grid row-wise
    for (int i = 0; i < len; i++) {
        grid[i / n][i % n] = message[i];
    }

    // Read columns in order of the key (first transposition)
    string firstCipher;
    for (int k : key) {
        for (int r = 0; r < rows; r++) {
            if (k - 1 < n && r < grid.size()) {
                firstCipher += grid[r][k - 1];
            }
        }
    }
    
    // Second transposition: apply the same process again
    len = firstCipher.length();
    rows = (len + n - 1) / n;
    vector<string> grid2(rows, string(n, ' '));

    // Fill the grid row-wise with first cipher
    for (int i = 0; i < len; i++) {
        grid2[i / n][i % n] = firstCipher[i];
    }

    // Read columns in order of the key (second transposition)
    string finalCipher;
    for (int k : key) {
        for (int r = 0; r < rows; r++) {
            if (k - 1 < n && r < grid2.size()) {
                finalCipher += grid2[r][k - 1];
            }
        }
    }
    
    return finalCipher;
}

string doubleColumnTranspositionDecipher(const string& cipher, const vector<int>& key) {
    // Create inverse key for decryption
    vector<int> inverseKey(key.size());
    for (int i = 0; i < key.size(); i++) {
        inverseKey[key[i] - 1] = i + 1;
    }
    
    // First decryption (reverse of second encryption)
    int n = key.size();
    int len = cipher.length();
    int rows = (len + n - 1) / n;
    vector<string> grid(rows, string(n, ' '));

    // Fill the grid with cipher text using inverse key
    int index = 0;
    for (int k : key) {
        for (int r = 0; r < rows; r++) {
            if (k - 1 < n && index < len) {
                grid[r][k - 1] = cipher[index++];
            }
        }
    }

    // Read the grid row-wise to get first decryption
    string firstDecrypt;
    for (const auto& row : grid) {
        firstDecrypt += row;
    }
    
    // Second decryption (reverse of first encryption)
    len = firstDecrypt.length();
    rows = (len + n - 1) / n;
    vector<string> grid2(rows, string(n, ' '));

    // Fill the grid with first decryption using inverse key
    index = 0;
    for (int k : key) {
        for (int r = 0; r < rows; r++) {
            if (k - 1 < n && index < len) {
                grid2[r][k - 1] = firstDecrypt[index++];
            }
        }
    }

    // Read the grid row-wise to get original message
    string message;
    for (const auto& row : grid2) {
        message += row;
    }
    
    return message;
}
int main(){
    int choice;
    string message, encrypted, decrypted;
    
    do {
        cout << "\n=============== CIPHER MENU ===============\n";
        cout << "1. Rail Fence Cipher\n";
        cout << "2. Single Column Transposition Cipher\n";
        cout << "3. Double Column Transposition Cipher\n";
        cout << "4. Exit\n";
        cout << "==========================================\n";
        cout << "Enter your choice (1-4): ";
        cin >> choice;
        cin.ignore(); // Clear the input buffer
        
        switch(choice) {
            case 1: {
                cout << "\n--- Rail Fence Cipher ---\n";
                cout << "Enter the message: ";
                getline(cin, message);
                
                encrypted = railFenceCipher(message);
                cout << "Encrypted: " << encrypted << endl;
                
                decrypted = railFenceDecipher(encrypted);
                cout << "Decrypted: " << decrypted << endl;
                break;
            }
            
            case 2: {
                cout << "\n--- Single Column Transposition Cipher ---\n";
                cout << "Enter the message: ";
                getline(cin, message);
                
                cout << "Enter the key size: ";
                int keySize;
                cin >> keySize;
                
                vector<int> key(keySize);
                cout << "Enter the key (numbers 1 to " << keySize << " in any order): ";
                cout << "\nExample for key size 4: 3 1 4 2\n";
                cout << "Key: ";
                for(int i = 0; i < keySize; i++) {
                    cin >> key[i];
                }
                cin.ignore();
                
                encrypted = singleColumnTranspositionCipher(message, key);
                cout << "Encrypted: " << encrypted << endl;
                
                decrypted = singleColumnTranspositionDecipher(encrypted, key);
                cout << "Decrypted: " << decrypted << endl;
                break;
            }
            
            case 3: {
                cout << "\n--- Double Column Transposition Cipher ---\n";
                cout << "Enter the message: ";
                getline(cin, message);
                
                cout << "Enter the key size: ";
                int keySize;
                cin >> keySize;
                
                vector<int> key(keySize);
                cout << "Enter the key (numbers 1 to " << keySize << " in any order): ";
                cout << "\nExample for key size 4: 3 1 4 2\n";
                cout << "Key: ";
                for(int i = 0; i < keySize; i++) {
                    cin >> key[i];
                }
                cin.ignore();
                
                // Apply double transposition
                encrypted = doubleColumnTranspositionCipher(message, key);
                cout << "Encrypted: " << encrypted << endl;
                
                // Decrypt to get original message
                decrypted = doubleColumnTranspositionDecipher(encrypted, key);
                cout << "Decrypted: " << decrypted << endl;
                break;
            }
            
            case 4:
                cout << "\nExiting program. Goodbye!\n";
                break;
                
            default:
                cout << "\nInvalid choice! Please enter a number between 1-4.\n";
                break;
        }
        
        if(choice != 4) {
            cout << "\nPress Enter to continue...";
            cin.get();
        }
        
    } while(choice != 4);
    
    return 0;
}