#ifndef QUIZENGINE_H
#define QUIZENGINE_H

#include <string>
#include <iostream>

#include "Quiz.h"
#include "QuestionBank.h"
#include "AttemptState.h"

using namespace std;

class QuizEngine
{
private:

    Quiz* currentQuiz;

    QuestionBank* questionBank;

    string answers[30];

    int currentIndex;

    int score;

    AttemptState state;

public:

    // Constructor
    QuizEngine();

    // Start Quiz
    bool start(Quiz* quiz, QuestionBank* bank);

    // Display
    void displayCurrentQuestion() const;

    // Answer
    bool answer(const string& ans);

    // Navigation
    bool next();

    bool previous();

    // Submit
    bool submit();

    // Calculate score
    int calculateScore();

    // Show result
    void showResult() const;

    // Getter
    AttemptState getState() const;

    int getCurrentIndex() const;

    int getScore() const;

    string getCurrentAnswer() const;
};

#endif
