#include "Quiz.h"

Quiz::Quiz()
{
    id = 0;
    title = "";
    questionCount = 0;
}

Quiz::Quiz(int id, const string& title)
{
    this->id = id;
    this->title = title;
    questionCount = 0;
}

//========================
// Getter
//========================

int Quiz::getId() const
{
    return id;
}

string Quiz::getTitle() const
{
    return title;
}

int Quiz::getQuestionCount() const
{
    return questionCount;
}

int Quiz::getQuestionId(int index) const
{
    if (index < 0 || index >= questionCount)
        return -1;

    return questionIds[index];
}

//========================
// Setter
//========================

void Quiz::setTitle(const string& title)
{
    this->title = title;
}

//========================
// Add Question
//========================

bool Quiz::addQuestion(int questionId)
{
    if (questionCount >= MAX_QUESTIONS)
        return false;

    if (containsQuestion(questionId))
        return false;

    questionIds[questionCount++] = questionId;

    return true;
}

//========================
// Remove Question
//========================

bool Quiz::removeQuestion(int questionId)
{
    int index = -1;

    for (int i = 0; i < questionCount; i++)
    {
        if (questionIds[i] == questionId)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
        return false;

    for (int i = index; i < questionCount - 1; i++)
    {
        questionIds[i] = questionIds[i + 1];
    }

    questionCount--;

    return true;
}

//========================
// Contains Question
//========================

bool Quiz::containsQuestion(int questionId) const
{
    for (int i = 0; i < questionCount; i++)
    {
        if (questionIds[i] == questionId)
            return true;
    }

    return false;
}

//========================
// Display
//========================

void Quiz::display() const
{
    cout << "Quiz ID: " << id << endl;
    cout << "Title: " << title << endl;

    cout << "Questions: ";

    if (questionCount == 0)
    {
        cout << "(Empty)";
    }
    else
    {
        for (int i = 0; i < questionCount; i++)
        {
            cout << questionIds[i];

            if (i != questionCount - 1)
                cout << ", ";
        }
    }

    cout << endl;
}