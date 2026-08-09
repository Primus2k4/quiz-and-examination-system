#ifndef QUIZAPP_H
#define QUIZAPP_H

#include "QuestionBank.h"
#include "QuizManager.h"
#include "QuizEngine.h"

class QuizApp {
private:
    QuestionBank bank;
    QuizManager manager;
    QuizEngine engine;

    void showMainMenu();
    void questionMenu();
    void quizMenu();
    void takeQuizMenu();

public:
    QuizApp();
    void run();
};

#endif
