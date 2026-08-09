#include <iostream>

#include "QuestionBank.h"
#include "MCQ.h"
#include "TF.h"
#include "Quiz.h"
#include "QuizManager.h"
#include "QuizEngine.h"

using namespace std;

int main()
{
    //==================================================
    // 1. QUESTION BANK
    //==================================================

    QuestionBank bank;

    //---------- MCQ ----------
    string options[4] =
    {
        "Java",
        "Python",
        "C++",
        "HTML"
    };

    MCQ* mcq = new MCQ(
        101,
        "Which language is used in this project?",
        2,
        options,
        'C'
    );

    if (!bank.addQuestion(mcq))
    {
        cout << "Cannot add MCQ.\n";
        delete mcq;
        return 1;
    }

    //---------- TF ----------
    TF* tf = new TF(
        102,
        "C++ supports inheritance.",
        1,
        true
    );

    if (!bank.addQuestion(tf))
    {
        cout << "Cannot add TF.\n";
        delete tf;
        return 1;
    }

    cout << "Question Bank created successfully.\n";


    //==================================================
    // 2. QUIZ MANAGER
    //==================================================

    QuizManager quizManager;

    if (!quizManager.createQuiz(
            1,
            "C++ Basic Quiz"))
    {
        cout << "Cannot create quiz.\n";
        return 1;
    }

    cout << "Quiz created successfully.\n";


    //==================================================
    // 3. ADD QUESTIONS TO QUIZ
    //==================================================

    if (!quizManager.addQuestionToQuiz(
            1,
            101,
            bank))
    {
        cout << "Cannot add question 101 to quiz.\n";
        return 1;
    }

    if (!quizManager.addQuestionToQuiz(
            1,
            102,
            bank))
    {
        cout << "Cannot add question 102 to quiz.\n";
        return 1;
    }

    cout << "Questions added to quiz successfully.\n";


    //==================================================
    // 4. FIND QUIZ
    //==================================================

    Quiz* quiz = quizManager.findQuiz(1);

    if (quiz == nullptr)
    {
        cout << "Quiz not found.\n";
        return 1;
    }


    //==================================================
    // 5. CREATE QUIZ ENGINE
    //==================================================

    QuizEngine engine;

    if (!engine.start(quiz, &bank))
    {
        cout << "Cannot start quiz.\n";
        return 1;
    }

    cout << "\nQuiz started successfully.\n";


    //==================================================
    // 6. QUESTION 1 - MCQ
    //==================================================

    cout << "\n========================================\n";
    cout << "QUESTION 1\n";
    cout << "========================================\n";

    engine.displayCurrentQuestion();

    cout << "\nUser answer: C\n";

    if (engine.answer("C"))
        cout << "Answer saved successfully.\n";
    else
        cout << "Failed to save answer.\n";


    //==================================================
    // 7. NEXT
    //==================================================

    if (engine.next())
        cout << "\nMoved to next question.\n";


    //==================================================
    // 8. QUESTION 2 - TF
    //==================================================

    cout << "\n========================================\n";
    cout << "QUESTION 2\n";
    cout << "========================================\n";

    engine.displayCurrentQuestion();

    cout << "\nUser answer: true\n";

    if (engine.answer("true"))
        cout << "Answer saved successfully.\n";
    else
        cout << "Failed to save answer.\n";


    //==================================================
    // 9. PREVIOUS TEST
    //==================================================

    if (engine.previous())
    {
        cout << "\nMoved back to Question 1.\n";

        cout << "Current answer: "
             << engine.getCurrentAnswer()
             << endl;
    }


    //==================================================
    // 10. NEXT AGAIN
    //==================================================

    engine.next();


    //==================================================
    // 11. SUBMIT
    //==================================================

    cout << "\n========================================\n";
    cout << "SUBMIT QUIZ\n";
    cout << "========================================\n";

    if (engine.submit())
        cout << "Quiz submitted successfully.\n";
    else
        cout << "Failed to submit quiz.\n";


    //==================================================
    // 12. SHOW RESULT
    //==================================================

    engine.showResult();


    //==================================================
    // 13. TEST CANNOT ANSWER AFTER SUBMIT
    //==================================================

    cout << "\n========================================\n";
    cout << "TEST AFTER SUBMIT\n";
    cout << "========================================\n";

    if (!engine.answer("A"))
        cout << "Correct: Cannot answer after submit.\n";
    else
        cout << "Error: Answer was accepted after submit.\n";


    //==================================================
    // END
    //==================================================

    return 0;
}
