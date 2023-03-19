#include "brackets_checker.h"

/// @brief Check if brackets sequence "()" is correct.
/// @param sequence sequence to check
/// @return true if brackets sequence is correct, false otherwise
bool checkBracketsSequence(const char* sequence) {
    // Create a counter to store the number of openning brackets
    int number_of_openning_brackets = 0;
    // Loop through the given sequence
    for (int i = 0; i < strlen(sequence); i++) {
        // If the current character is an openning bracket
        if (sequence[i] == '(') {
            // Increment the counter
            number_of_openning_brackets++;
        } else if (sequence[i] == ')') {
            // Decrement the counter
            number_of_openning_brackets--;
        }
        // If the counter is less than 0
        if (number_of_openning_brackets < 0) {
            // Return false
            return false;
        }
    }

    // If the counter is equal to 0 then the sequence is correct
    return number_of_openning_brackets == 0;
}
