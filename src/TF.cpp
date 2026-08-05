#include "../include/TF.h"
#include <iostream>
#include <algorithm>    
#include <cctype>       

static std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return s;
}

TF::TF(int id,
    const std::string& prompt,
    int points,
    bool correctAnswer)
    : Question(id, prompt, points)
    , m_correctAnswer(correctAnswer)
{
}

void TF::display() const {
    std::cout << "[TF] (ID: " << getId()
        << " | " << getPoints() << " pts)\n";
    std::cout << getPrompt() << "\n";
    std::cout << "  1. True\n";
    std::cout << "  2. False\n";
}

bool TF::checkAnswer(const std::string& answer) const {
    std::string lower = toLower(answer);
    if (lower == "true")  return m_correctAnswer == true;
    if (lower == "false") return m_correctAnswer == false;
    return false;
}

std::string TF::getType() const {
    return "TF";
}

bool TF::getCorrectAnswer() const {
    return m_correctAnswer;
}

void TF::setCorrectAnswer(bool correctAnswer) {
    m_correctAnswer = correctAnswer;
}