#include <iostream>
#include <cctype>
#include <string>
#include <queue>
#include <cmath>
#include <stdexcept>

// Converts a plaintext string to an queue of integers according to the A0Z25 letter number code
std::queue<int> letterToNumber(const std::string plaintext) {
    std::queue<int> ciphertext;
    // Iterates through all characters in plaintext
    for (int i=0; i<plaintext.length(); i++) {
        // Finds the ASCII decimal value for the character after changing it to lowercase
        int ascii = std::tolower(plaintext[i]);;
        // Only adds letters to ciphertext, ignoring special characters and spaces
        if (ascii - 97 <= 25 && ascii - 97 >= 0) {
            // Adds the ASCII value-97 into ciphertext queue to convert to number with a=0,...,z=25
            ciphertext.push(ascii - 97);
        }

    }
    return ciphertext;
}

// Converst a ciphertext queue of integers to a string according to the A0Z25 letter number code
std::string numberToLetter(std::queue<int> ciphertext) {
    std::string plaintext = "";
    // Goes through all integers in ciphertext queue
    while (!ciphertext.empty()) {
        // Converts int in ciphertext queue to character using ASCII values and appends it onto plaintext
        plaintext += (char)('a' + ciphertext.front()%26);
        // Pops the front value of ciphertext
        ciphertext.pop();
    }
    return plaintext;
}

// Multiplies a matrix and a vector when vector is on right
std::vector<int> multiplyMatrixVector(const std::vector<std::vector<int>> m, const std::vector<int> v) {
    std::vector<int> result;
    // Throws exception is a and b cannot be multiplied
    if (m[0].size() != v.size()) {
        throw std::invalid_argument("Matrix and vector cannot be multiplied!");
    }

    for (int i=0; i<m.size(); i++) {
        result.push_back(0); 
        // Dot product of ith row of a and b
        for (int j=0; j<m[0].size(); j++) {
            result[i] += m[i][j] * v[j];
        }
    }
    return result;
}
// Multiplies a matrix and a vector when vector is on left
std::vector<int> multiplyMatrixVector(const std::vector<int> v, const std::vector<std::vector<int>> m) {
    std::vector<int> result;
    // Throws exception is a and b cannot be multiplied
    if (v.size() != m.size()) {
        throw std::invalid_argument("Matrix and vector cannot be multiplied!");
    }

    for (int i=0; i<m[0].size(); i++) {
        result.push_back(0); 
        // Dot product of ith row of a and b
        for (int j=0; j<m.size(); j++) {
            result[i] += m[j][i] * v[j];
        }
    }
    return result;
}
// Finds the minor of a matrix
std::vector<std::vector<int>> minor(std::vector<std::vector<int>> m, int x, int y) {
    // std::vector<std::vector<int>> minor; 
    // int index = 0;
    // for (int i=0; i<m.size(); i++) {
    //     // Creates a new row vector
    //     if (i != x) {
    //          minor.push_back(std::vector<int>(0, 0));
    //         // Populates new row vector ignoring value when j == x
    //         for (int j=0; j<m[0].size(); j++) {
    //             if (j != y) {
    //                 minor[0].push_back(m[i][j]);
    //             }
    //         }
    //         index++;
    //     }
    // }
    // return minor;

    // Erases the xth row of m
    m.erase(m.begin() + x);
    // Erases all elements in the yth row of m
    for (int i=0; i<m.size(); i++) {
        m[i].erase(m[i].begin() + y);
    }

    return m;
}
// Finds the determinant of a matrix
int determinant(const std::vector<std::vector<int>> m) {
    int detSum = 0;
    // Base case, determinant of a 1x1 matrix is the value
    if (m.size() == 1) {
        return m[0][0];
    }
    // Calculates determinant recursively with cofactors
    for(int i=0; i<m[0].size(); i++) {
        detSum += m[0][i] * pow(-1, i) * determinant(minor(m, 0, i));
    }
    return detSum;
}

// Finds the inverse mod of a value
int invMod(const int x) {
    // Brute force finds for a solution to an inverse mod
    for (int i=1; i<=25; i++) {
        int mod = (x*i) % 26;
        if (mod < 0 && mod + 26 == 1) return i;
        else if (mod == 1) return i;
    }
    throw std::invalid_argument("Inverse mod for this encoding matrix does not exist!");
}
// Finds the inverse of a matrix and takes mod 26 of the result
std::vector<std::vector<int>> invertMatrix(const std::vector<std::vector<int>> m) {
    // Finds the adjoint of the matrix
    std::vector<std::vector<int>> adjoint;
    for (int i=0; i<m.size(); i++) {
        adjoint.push_back(std::vector<int>(0, 0));
        for (int j=0; j<m[0].size(); j++) {
            // Swaps i and j when finding the cofactor
            adjoint[i].push_back(determinant(minor(m, j, i)) * pow(-1, i+j));
        }
    }
    // Finds the inverse of the matrix using the determinant and adjoint
    std::vector<std::vector<int>> inverse;
    for (int i=0; i<m.size(); i++) {
        inverse.push_back(std::vector<int>(0, 0));
        for (int j=0; j<m[0].size(); j++) {
            // Takes mod 26 of adjoint, and inverse mod of the determinant to ensure integer results
            inverse[i].push_back(adjoint[i][j] % 26 * invMod(determinant(m)));
            // Ensures all values of inverse are positive
            if (inverse[i][j] < 0) {
                inverse[i][j] = inverse[i][j] + 26;
            }
        }
    }
    return inverse;
}

// Encodes a plaintext string with a Hill Cipher
std::string encode(const std::string plaintext, const std::vector<std::vector<int>> encoding) {
    std::queue<int> plaintextNumbers = letterToNumber(plaintext);
    std::queue<int> ciphertextNumbers;
    // Throws exception if encoding matrix is not square (nxn) 
    if (encoding.size() != encoding[0].size()) {
        throw std::invalid_argument("Encoding Matrix is not square!");
    }
    // Determinant of encoding matrix must be relatively prime to 26, not even or 13
    if (determinant(encoding) % 2 == 0 || determinant(encoding) == 13) {
        throw std::invalid_argument("Determinant of encoding matrix is not relatively prime to 26!");
    }
    // Ensures plaintextNumbers is divisible by n by repeating the last element
    int lastNum = plaintextNumbers.back();
    for (int i=0; i<plaintextNumbers.size() % encoding.size(); i++) {
        plaintextNumbers.push(lastNum);
    }
    // Continues until plaintextNumbers is empty
    while (!plaintextNumbers.empty()) {
        std::vector<int> v; // Vector with length n
        for (int i=0; i<encoding.size(); i++) { // Repeats n times
            // Adds the front element of ciphertext to v and removes it from plaintextNumbers
            v.push_back(plaintextNumbers.front());
            plaintextNumbers.pop();
        }
        // Multiplies v with the encoding matrix and adds the result to ciphertextNumbers
        v = multiplyMatrixVector(encoding, v);
        for (int & num: v) {
            ciphertextNumbers.push(num % 26);
        }
    }
    return numberToLetter(ciphertextNumbers);
}

// Decodes a ciphertext encoded with a Hill Cypher knowing the encoding matrix
std::string decode(const std::string ciphertext, const std::vector<std::vector<int>> encoding) {
    std::queue<int> ciphertextNumbers = letterToNumber(ciphertext);
    std::queue<int> plaintextNumbers;
    // Throws exception if encoding matrix is not square (nxn) 
    if (encoding.size() != encoding[0].size()) {
        throw std::invalid_argument("Encoding Matrix is not square!");
    }
    // Determinant of encoding matrix must be relatively prime to 26, not even or 13
    if (determinant(encoding) % 2 == 0 || determinant(encoding) == 13) {
        throw std::invalid_argument("Determinant of encoding matrix is not relatively prime to 26!");
    }
    // Ensures ciphertextNumbers is divisible by n by repeating the last element
    int lastNum = ciphertextNumbers.back();
    for (int i=0; i<ciphertextNumbers.size() % encoding.size(); i++) {
        ciphertextNumbers.push(lastNum);
    }
    std::vector<std::vector<int>> inverse = invertMatrix(encoding);
    // Continues until plaintextNumbers is empty
    while (!ciphertextNumbers.empty()) {
        std::vector<int> v; // Vector with length n
        for (int i=0; i<inverse.size(); i++) { // Repeats n times
            // Adds the front element of ciphertext to v and removes it from plaintextNumbers
            v.push_back(ciphertextNumbers.front());
            ciphertextNumbers.pop();
        }
        // Multiplies v with the encoding matrix and adds the result to ciphertextNumbers
        v = multiplyMatrixVector(v, inverse);
        for (int & num: v) {
            plaintextNumbers.push(num % 26);
        }
    }
    return numberToLetter(plaintextNumbers);
}

int main() {
    std::string message = "To be, or not to be, that is the question:"
                            "Whether 'tis nobler in the mind to suffer"
                            "The slings and arrows of outrageous fortune,"
                            "Or to take arms against a sea of troubles";
    std::vector<std::vector<int>> key {{1, 3}, {3, 4}};
    std::string encoded = encode(message, key);
    std::string decoded = decode(encoded, key);
    std::cout<< encoded << " ";
    std::cout << decoded;
    std::vector<std::vector<int>> inverse = invertMatrix(key);
    std::vector<std::vector<int>> a = minor(key, 0, 0);
    //std::cout << determinant(key);
    // for (int i=0; i < inverse.size(); i++) {
    //     for (int j=0; j<inverse.size(); j++) {
    //         std::cout << inverse[i][j] << " ";
    //     }
    // }
    return 0;
}