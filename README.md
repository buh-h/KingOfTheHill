#Hill Cipher Encoder and Decoder
Encodes messages with a Hill Cipher, or decodes messages when provided an encoding matrix.

#Encoding:
1. Converts the letters in a message to a corresponding integer value based on the A0Z25 encoding scheme
2. Utilizes an encoding matrix to encrypt the message by multiplying the integer values by the matrix
3. Takes the modulus 26 of each resulting value to ensure all integers are still between 0-25
4. Converts the encrypted integers back into letters.

#Decoding (with encoding matrix): 
1. Converts the letters in an encoding message into integer values
2. Finds the inverse of the encoding matrix by calculating the determinant and adjoint matrix. Takes modulus of the
values in the adjoint matrix and the modular multiplicative inverse of the determinant to ensure all values are integers
3. Multiplies the integer values by the matrix and mods each value by 26
4. Converts the decrypted integers back into letters

#Future Additions: 
- Implement the known plaintext attack, which solves for the encoding matrix given enough of a plaintext message
- Implement a method that attempts to bruteforce solve for an encoding matrix with only the encoded message
