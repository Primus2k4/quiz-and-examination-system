#include "InputValidator.h"
#include <cctype>

bool InputValidator::isPositiveInt(const std::string& str) {
    if (str.empty()) return false;
    for (int i = 0; i < (int)str.size(); i++) {
        if (!isdigit(str[i])) return false;
    }
    if (str.find_first_not_of('0') == std::string::npos) return false;
    return true;
}


bool InputValidator::isNonNegativeInt(const std::string& str) {
    if (str.empty()) return false;
    for (int i = 0; i < (int)str.size(); i++) {
        if (!isdigit(str[i])) return false;
    }
    return true;
}

bool InputValidator::isValidChoice(char choice, char minChoice, char maxChoice) {
    char c = toupper(choice);
    return c >= toupper(minChoice) && c <= toupper(maxChoice);
}

bool InputValidator::isNonEmptyString(const std::string& str) {
    return !str.empty();
}
