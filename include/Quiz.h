#ifndef QUIZ_H
#define QUIZ_H

#include <string>
#include <iostream>

using namespace std;

class Quiz
{
private:
    int id;
    string title;

    static const int MAX_QUESTIONS = 30;
    int questionIds[MAX_QUESTIONS];
    int questionCount;

public:
    // Constructor
    Quiz();
    Quiz(int id, const string& title);

    // Getter
    int getId() const;
    string getTitle() const;
    int getQuestionCount() const;
    int getQuestionId(int index) const;

    // Setter
    void setTitle(const string& title);

    // Quiz functions
    bool addQuestion(int questionId);
    bool removeQuestion(int questionId);
    bool containsQuestion(int questionId) const;

    // Display
    void display() const;
};

#endif