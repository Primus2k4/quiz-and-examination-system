#ifndef DATAFILEMANAGER_H
#define DATAFILEMANAGER_H
#include <string>
#include "QuestionBank.h"
#include "QuizManager.h"

class DataFileManager {
public:
    static bool loadQuestions(QuestionBank& bank, const std::string& filepath);
    static bool saveQuestions(const QuestionBank& bank, const std::string& filepath);
    static bool loadQuizzes(QuizManager& manager, const std::string& filepath);
    static bool saveQuizzes(const QuizManager& manager, const std::string& filepath);
};
#endif
