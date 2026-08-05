#include "QuestionBank.h"
#include <iostream>

using namespace std;

QuestionBank::QuestionBank()
{
    count = 0;

    for (int i = 0; i < 100; i++)
        questions[i] = nullptr;
}

QuestionBank::~QuestionBank()
{
    for (int i = 0; i < count; i++)
    {
        delete questions[i];
    }
}

bool QuestionBank::addQuestion(Question* q)
{
    if (count >= 100 || q == nullptr)
        return false;

    if (findById(q->getId()) != nullptr)
        return false;

    questions[count++] = q;

    return true;
}

bool QuestionBank::removeQuestion(int id)
{
    for (int i = 0; i < count; i++)
    {
        if (questions[i]->getId() == id)
        {
            delete questions[i];

            for (int j = i; j < count - 1; j++)
            {
                questions[j] = questions[j + 1];
            }

            questions[count - 1] = nullptr;
            count--;

            return true;
        }
    }

    return false;
}

bool QuestionBank::updateQuestion(int id, Question* newQuestion)
{
    if (newQuestion == nullptr)
        return false;

    for (int i = 0; i < count; i++)
    {
        if (questions[i]->getId() == id)
        {
            delete questions[i];
            questions[i] = newQuestion;
            return true;
        }
    }

    return false;
}

Question* QuestionBank::findById(int id) const
{
    for (int i = 0; i < count; i++)
    {
        if (questions[i]->getId() == id)
            return questions[i];
    }

    return nullptr;
}

void QuestionBank::displayByPrompt(const string& keyword) const
{
    bool found = false;

    for (int i = 0; i < count; i++)
    {
        if (questions[i]->getPrompt().find(keyword) != string::npos)
        {
            questions[i]->display();
            cout << endl;
            found = true;
        }
    }

    if (!found)
        cout << "No matching questions found.\n";
}

void QuestionBank::displayAll() const
{
    if (count == 0)
    {
        cout << "Question bank is empty.\n";
        return;
    }

    for (int i = 0; i < count; i++)
    {
        questions[i]->display();
        cout << endl;
    }
}

int QuestionBank::getCount() const
{
    return count;
}