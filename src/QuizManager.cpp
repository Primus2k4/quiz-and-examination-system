#include "QuizManager.h"
#include <iostream>

using namespace std;

QuizManager::QuizManager()
{
    count = 0;
}

int QuizManager::getCount() const
{
    return count;
}

Quiz* QuizManager::findQuiz(int id)
{
    for (int i = 0; i < count; i++)
    {
        if (quizzes[i].getId() == id)
            return &quizzes[i];
    }

    return nullptr;
}

const Quiz* QuizManager::findQuiz(int id) const
{
    for (int i = 0; i < count; i++)
    {
        if (quizzes[i].getId() == id)
            return &quizzes[i];
    }

    return nullptr;
}

const Quiz* QuizManager::getAt(int index) const {
    if (index < 0 || index >= count) return nullptr;
    return &quizzes[index];
}

bool QuizManager::createQuiz(int id, const string& title)
{
    if (count >= MAX_QUIZZES)
        return false;

    if (title.empty())
        return false;

    // Check duplicate ID
    if (findQuiz(id) != nullptr)
        return false;

    // Check duplicate title
    for (int i = 0; i < count; i++)
    {
        if (quizzes[i].getTitle() == title)
            return false;
    }

    quizzes[count] = Quiz(id, title);
    count++;

    return true;
}

bool QuizManager::deleteQuiz(int id)
{
    int index = -1;

    for (int i = 0; i < count; i++)
    {
        if (quizzes[i].getId() == id)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
        return false;

    for (int i = index; i < count - 1; i++)
    {
        quizzes[i] = quizzes[i + 1];
    }

    count--;

    return true;
}

bool QuizManager::renameQuiz(int id, const string& title)
{
    if (title.empty())
        return false;

    // Duplicate title
    for (int i = 0; i < count; i++)
    {
        if (quizzes[i].getTitle() == title &&
            quizzes[i].getId() != id)
        {
            return false;
        }
    }

    Quiz* quiz = findQuiz(id);

    if (quiz == nullptr)
        return false;

    quiz->setTitle(title);

    return true;
}

bool QuizManager::addQuestionToQuiz(
    int quizId,
    int questionId,
    const QuestionBank& bank)
{
    Quiz* quiz = findQuiz(quizId);

    if (quiz == nullptr)
        return false;

    // Question must exist
    if (bank.findById(questionId) == nullptr)
        return false;

    return quiz->addQuestion(questionId);
}

bool QuizManager::removeQuestionFromQuiz(
    int quizId,
    int questionId)
{
    Quiz* quiz = findQuiz(quizId);

    if (quiz == nullptr)
        return false;

    return quiz->removeQuestion(questionId);
}

void QuizManager::removeQuestionFromAllQuiz(int questionId)
{
    for (int i = 0; i < count; i++)
    {
        quizzes[i].removeQuestion(questionId);
    }
}

void QuizManager::displayAll() const
{
    if (count == 0)
    {
        cout << "No quizzes.\n";
        return;
    }

    for (int i = 0; i < count; i++)
    {
        quizzes[i].display();
        cout << endl;
    }
}
