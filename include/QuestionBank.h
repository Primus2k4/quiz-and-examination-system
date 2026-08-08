#ifndef QUESTIONBANK_H
#define QUESTIONBANK_H

#include "Question.h"
#include <string>

using namespace std;

class QuestionBank
{
private:
    Question* questions[100];
    int count;

public:
    QuestionBank();
    ~QuestionBank();

    bool addQuestion(Question* q);
    bool removeQuestion(int id);
    bool updateQuestion(int id, Question* newQuestion);

    Question* findById(int id) const;
    Question* getAt(int index) const;

    void displayByPrompt(const string& keyword) const;
    void displayAll() const;

    int getCount() const;
};

#endif