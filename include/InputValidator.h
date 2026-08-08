#ifndef INPUTVALIDATOR_H
#define INPUTVALIDATOR_H
#include <string>

class InputValidator {
public:

    static bool isPositiveInt(const std::string& str);
    static bool isNonNegativeInt(const std::string& str);   
    static bool isValidChoice(char choice, char minChoice, char maxChoice);
    static bool isNonEmptyString(const std::string& str);
};
#endif
