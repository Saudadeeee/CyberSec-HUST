#include <iostream>
#include <string>
#include <vector>
#include <cctype>
using namespace std;

string prepareKey(const string &key) {
    string result;
    bool seen[26] = {false};
    for (char c : key) {
        c = tolower(c);
        if (c < 'a' || c > 'z') continue;
        if (c == 'j') c = 'i';
        if (!seen[c - 'a']) {
            seen[c - 'a'] = true;
            result.push_back(c);
        }
    }
    for (char c = 'a'; c <= 'z'; c++) {
        if (c == 'j') continue; 
        if (!seen[c - 'a']) {
            seen[c - 'a'] = true;
            result.push_back(c);
        }
    }
    return result;
}
vector<vector<char>> buildMatrix(const string &key) {
    vector<vector<char>> matrix(5, vector<char>(5));
    for (int i = 0; i < 25; i++) {
        matrix[i / 5][i % 5] = key[i];
    }
    return matrix;
}
void findPosition(const vector<vector<char>> &matrix, char c, int &row, int &col) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (matrix[i][j] == c) {
                row = i;
                col = j;
                return;
            }
        }
    }
}
string preprocessText(const string &text) {
    string result;
    for (char c : text) {
        if (isalpha(static_cast<unsigned char>(c))) {
            c = tolower(c);
            if (c == 'j') c = 'i'; 
            result.push_back(c);
        }
    }
    for (int i = 0; i < (int)result.size() - 1; i += 2) {
        if (result[i] == result[i+1]) {
            result.insert(result.begin() + i + 1, 'x');
        }
    }

    if (result.size() % 2 != 0) {
        result.push_back('x');
    }
    return result;
}
string encryptPlayfair(const string &plaintext, const vector<vector<char>> &matrix) {
    string ciphertext;
    for (int i = 0; i < (int)plaintext.size(); i += 2) {
        char a = plaintext[i], b = plaintext[i+1];
        int r1, c1, r2, c2;
        findPosition(matrix, a, r1, c1);
        findPosition(matrix, b, r2, c2);
        if (r1 == r2) {
            c1 = (c1 + 1) % 5;
            c2 = (c2 + 1) % 5;
        } else if (c1 == c2) {
         
            r1 = (r1 + 1) % 5;
            r2 = (r2 + 1) % 5;
        } else {
            int temp = c1;
            c1 = c2;
            c2 = temp;
        }
        ciphertext.push_back(matrix[r1][c1]);
        ciphertext.push_back(matrix[r2][c2]);
    }
    return ciphertext;
}
int main() {
    cout << "Nhap key: ";
    string key;
    getline(cin, key);
    string processedKey = prepareKey(key);
    vector<vector<char>> matrix = buildMatrix(processedKey);
    cout << "Nhap ban ro: ";
    string plaintext;
    getline(cin, plaintext);
    plaintext = preprocessText(plaintext);
    string encrypted = encryptPlayfair(plaintext, matrix);
    cout << "Ban ma: " << encrypted << endl;
    return 0;
}