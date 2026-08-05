#include "../include/Question.h"

Question::Question(int id, const std::string& prompt, int points)
    : m_id(id), m_prompt(prompt), m_points(points)
{
}

int Question::getId() const {
    return m_id;
}

std::string Question::getPrompt() const {
    return m_prompt;
}

int Question::getPoints() const {
    return m_points;
}

bool Question::setPrompt(const std::string& prompt) {
    if (prompt.empty()) return false;
    m_prompt = prompt;
    return true;
}

bool Question::setPoints(int points) {
    if (points <= 0) return false;
    m_points = points;
    return true;
}