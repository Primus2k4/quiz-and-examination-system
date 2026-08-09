#include "../include/MCQ.h"
#include <iostream>
#include <cctype>   

MCQ::MCQ(int id,
    const std::string& prompt,
    int points,
    const std::string options[4],
    char correctOption)
    : Question(id, prompt, points)
    , m_correctOption(static_cast<char>(std::toupper(correctOption)))
{
    for (int i = 0; i < 4; ++i) {
        m_options[i] = options[i];
    }
}

void MCQ::display() const {
    std::cout << "[MCQ] (ID: " << getId()
        << " | " << getPoints() << " pts)\n";
    std::cout << getPrompt() << "\n";
    const char labels[] = { 'A', 'B', 'C', 'D' };
    for (int i = 0; i < 4; ++i) {
        std::cout << "  " << labels[i] << ". " << m_options[i] << "\n";
    }
}

bool MCQ::checkAnswer(const std::string& answer) const {
    if (answer.size() != 1) return false;
    char input = static_cast<char>(std::toupper(answer[0]));
    return input == m_correctOption;
}

std::string MCQ::getType() const {
    return "MCQ";
}

bool MCQ::isValid() const {
    if (getId() <= 0 || getPrompt().empty() || getPoints() <= 0) return false;
    if (m_correctOption < 'A' || m_correctOption > 'D') return false;
    for (int i = 0; i < 4; ++i) if (m_options[i].empty()) return false;
    return true;
}

std::string MCQ::getOption(int index) const {
    if (index < 0 || index > 3) return "";
    return m_options[index];
}

char MCQ::getCorrectOption() const {
    return m_correctOption;
}

bool MCQ::setOption(int index, const std::string& text) {
    if (index < 0 || index > 3) return false;
    if (text.empty()) return false;
    m_options[index] = text;
    return true;
}

bool MCQ::setCorrectOption(char option) {
    char upper = static_cast<char>(std::toupper(option));
    if (upper < 'A' || upper > 'D') return false;
    m_correctOption = upper;
    return true;
}