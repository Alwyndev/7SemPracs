#include <iostream>
#include <string>
#include <vector>
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

int main(){
    string message;
    cout<< "Enter the message :";
    getline(cin, message);
    string encrypted = railFenceCipher(message);
    cout << "Encrypted: " << encrypted << endl;

    string decrypted = railFenceDecipher(encrypted);
    cout << "Decrypted: " << decrypted << endl;

    return 0;
}