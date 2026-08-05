#ifndef QUIZMANAGER_H
#define QUIZMANAGER_H

#include "Quiz.h"
#include "QuestionBank.h"

class QuizManager
{
private:
    static const int MAX_QUIZZES = 10;

    Quiz quizzes[MAX_QUIZZES];
    int count;

public:
    QuizManager();

    // Getter
    int getCount() const;

    // CRUD Quiz
    bool createQuiz(int id, const string& title);
    bool deleteQuiz(int id);
    bool renameQuiz(int id, const string& title);

    // Search
    Quiz* findQuiz(int id);
    const Quiz* findQuiz(int id) const;

    // Manage Question
    bool addQuestionToQuiz(int quizId,
        int questionId,
        const QuestionBank& bank);

    bool removeQuestionFromQuiz(int quizId,
        int questionId);

    void removeQuestionFromAllQuiz(int questionId);

    // Display
    void displayAll() const;
};

#endif