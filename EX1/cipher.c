#include "cipher.h"
#include <string.h>

/// defines the starting character code for capital letters in ASCII.
#define START_OF_CAPITAL 65
/// Defines the ending character code for capital letters in ASCII.
#define END_OF_CAPITAL 90
/// Defines the starting character code for lowercase letters in ASCII.
#define START_OF_LOWERCASE 97
/// Defines the ending character code for lowercase letters in ASCII.
#define END_OF_LOWERCASE 122
/// Defines the total number of letters in the English alphabet.
#define NUM_OF_LETTERS 26

/**
 * Calculates the mathematical modulo of an integer `k` with respect to
 * `NUM_OF_LETTERS`.
 *
 * @param k The integer to be calculated.
 * @return The mathematical modulo of `k`.
 */
int modulo(int k)
{
    if (k < 0) {
        return NUM_OF_LETTERS + k % NUM_OF_LETTERS;
    }
    return k % NUM_OF_LETTERS;
}

/**
 * Encodes a given string `s` by shifting each letter in it `k` positions to the
 * right in the English alphabet. Only the letters in the ASCII character set
 * are shifted; all other characters are left untouched.
 *
 * @param s The string to be encoded.
 * @param k The number of positions to shift each letter in the string.
 */
void encode(char s[], int k)
{
    int s_length = strlen(s);
    for (int i = 0; i < s_length; ++i) {
        // Check if the current character is a capital letter.
        if ((s[i] - START_OF_CAPITAL) * (s[i] - END_OF_CAPITAL) <= 0) {
            s[i] = modulo(s[i] % START_OF_CAPITAL + modulo(k)) +
                    START_OF_CAPITAL;
        }
            // Check if the current character is a lowercase letter.
        else if ((s[i] - START_OF_LOWERCASE) * (s[i] - END_OF_LOWERCASE)
        <= 0) {
            s[i] = modulo(s[i] % START_OF_LOWERCASE + modulo(k)) +
                    START_OF_LOWERCASE;
        }
        // Otherwise, leave the character unchanged.
    }
}

/**
 * Decodes a given string `s` by shifting each letter in it `k`
 * positions to the
 * left in the English alphabet. Only the letters in the ASCII character set
 * are shifted; all other characters are left untouched.
 *
 * @param s The string to be decoded.
 * @param k The number of positions to shift each letter in the string.
 */
void decode(char s[], int k)
{
    // To decode a string, simply encode it with the negated key.
    encode(s, -k);
}

