#include <gtest/gtest.h>
#include <string>

#include "../include/QuestionBank.h"
#include "../include/QuizManager.h"
#include "../include/QuizEngine.h"
#include "../include/MCQ.h"
#include "../include/TF.h"

class QuizEngineTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::string opts[4] = { "Java", "Python", "C++", "HTML" };
        bank.addQuestion(new MCQ(101, "Which language is used in this project?", 2, opts, 'C'));
        bank.addQuestion(new TF(102, "C++ supports inheritance.", 1, true));

        quizManager.createQuiz(1, "C++ Basic Quiz");
        quizManager.addQuestionToQuiz(1, 101, bank);
        quizManager.addQuestionToQuiz(1, 102, bank);

        quizManager.createQuiz(2, "Empty Quiz");
    }

    QuestionBank bank;
    QuizManager  quizManager;
};

// TC-07: Start with empty quiz
TEST_F(QuizEngineTest, Start_EmptyQuiz_Rejected_StateRemainsNotStarted) {
    QuizEngine engine;
    Quiz* empty = quizManager.findQuiz(2);
    ASSERT_NE(empty, nullptr);

    EXPECT_FALSE(engine.start(empty, &bank));
    EXPECT_EQ(engine.getState(), NOT_STARTED);
}

// TC-06: Start with valid quiz
TEST_F(QuizEngineTest, Start_ValidQuiz_TransitionsToInProgress) {
    QuizEngine engine;
    Quiz* quiz = quizManager.findQuiz(1);
    ASSERT_NE(quiz, nullptr);

    EXPECT_TRUE(engine.start(quiz, &bank));
    EXPECT_EQ(engine.getState(), IN_PROGRESS);
}

TEST_F(QuizEngineTest, Start_ValidQuiz_FirstQuestionBecomeCurrent) {
    QuizEngine engine;
    engine.start(quizManager.findQuiz(1), &bank);

    EXPECT_EQ(engine.getCurrentIndex(), 0);
}

// TC-08: answer() before start
TEST_F(QuizEngineTest, Answer_BeforeStart_Rejected_NoStateChange) {
    QuizEngine engine;
    EXPECT_FALSE(engine.answer("C"));
    EXPECT_EQ(engine.getState(), NOT_STARTED);
}

// TC-12: Navigation
TEST_F(QuizEngineTest, Next_MovesToSecondQuestion) {
    QuizEngine engine;
    engine.start(quizManager.findQuiz(1), &bank);

    EXPECT_TRUE(engine.next());
    EXPECT_EQ(engine.getCurrentIndex(), 1);
}

TEST_F(QuizEngineTest, Next_AtLastQuestion_ReturnsFalse) {
    QuizEngine engine;
    engine.start(quizManager.findQuiz(1), &bank);
    engine.next();

    EXPECT_FALSE(engine.next());
}

TEST_F(QuizEngineTest, Previous_MovesBackToPreviousQuestion) {
    QuizEngine engine;
    engine.start(quizManager.findQuiz(1), &bank);
    engine.next();
    EXPECT_TRUE(engine.previous());
    EXPECT_EQ(engine.getCurrentIndex(), 0);
}

TEST_F(QuizEngineTest, Previous_AtFirstQuestion_ReturnsFalse) {
    QuizEngine engine;
    engine.start(quizManager.findQuiz(1), &bank);

    EXPECT_FALSE(engine.previous());
}

TEST_F(QuizEngineTest, Answer_SavedAndRetrievableAfterNavigation_TC12) {
    QuizEngine engine;
    engine.start(quizManager.findQuiz(1), &bank);

    engine.answer("C");
    engine.next();
    engine.previous();

    EXPECT_EQ(engine.getCurrentAnswer(), "C");
}

TEST_F(QuizEngineTest, Answer_CanBeChangedWhileInProgress_TC12) {
    QuizEngine engine;
    engine.start(quizManager.findQuiz(1), &bank);

    engine.answer("A");
    EXPECT_EQ(engine.getCurrentAnswer(), "A");

    engine.answer("C");
    EXPECT_EQ(engine.getCurrentAnswer(), "C");
}

// TC-11: Score calculation
TEST_F(QuizEngineTest, Submit_CorrectMCQ_WrongTF_ScoreIs2of3_TC11) {
    QuizEngine engine;
    engine.start(quizManager.findQuiz(1), &bank);

    engine.answer("C");
    engine.next();
    engine.answer("false");

    EXPECT_TRUE(engine.submit());
    EXPECT_EQ(engine.getScore(), 2);
}

TEST_F(QuizEngineTest, Submit_BothCorrect_ScoreIsTotal) {
    QuizEngine engine;
    engine.start(quizManager.findQuiz(1), &bank);

    engine.answer("C");
    engine.next();
    engine.answer("true");

    engine.submit();
    EXPECT_EQ(engine.getScore(), 3);
}

// TC-13: Unanswered questions
TEST_F(QuizEngineTest, Submit_UnansweredQuestions_CountAsWrong_TC13) {
    QuizEngine engine;
    engine.start(quizManager.findQuiz(1), &bank);

    EXPECT_TRUE(engine.submit());
    EXPECT_EQ(engine.getScore(), 0);
}

TEST_F(QuizEngineTest, Submit_OneAnsweredOneNot_OnlyAnsweredCounted) {
    QuizEngine engine;
    engine.start(quizManager.findQuiz(1), &bank);

    engine.answer("C");

    engine.submit();
    EXPECT_EQ(engine.getScore(), 2);
}

TEST_F(QuizEngineTest, Submit_TransitionsToSubmitted_TC13) {
    QuizEngine engine;
    engine.start(quizManager.findQuiz(1), &bank);
    engine.submit();

    EXPECT_EQ(engine.getState(), SUBMITTED);
}

// TC-14: After SUBMITTED
TEST_F(QuizEngineTest, Answer_AfterSubmit_Rejected_TC14) {
    QuizEngine engine;
    engine.start(quizManager.findQuiz(1), &bank);
    engine.answer("C");
    engine.submit();

    EXPECT_FALSE(engine.answer("A"));
}

TEST_F(QuizEngineTest, AnswerAfterSubmit_DoesNotChangeScore_TC14) {
    QuizEngine engine;
    engine.start(quizManager.findQuiz(1), &bank);
    engine.answer("C");
    engine.submit();
    int scoreBefore = engine.getScore();

    engine.answer("C");
    EXPECT_EQ(engine.getScore(), scoreBefore);
}

TEST_F(QuizEngineTest, SubmitTwice_SecondCallRejected_TC14) {
    QuizEngine engine;
    engine.start(quizManager.findQuiz(1), &bank);
    engine.submit();

    EXPECT_FALSE(engine.submit());
}

// State transition
TEST_F(QuizEngineTest, StateTransition_FollowsCorrectOrder) {
    QuizEngine engine;
    EXPECT_EQ(engine.getState(), NOT_STARTED);

    engine.start(quizManager.findQuiz(1), &bank);
    EXPECT_EQ(engine.getState(), IN_PROGRESS);

    engine.submit();
    EXPECT_EQ(engine.getState(), SUBMITTED);
}

TEST_F(QuizEngineTest, ShowResult_AfterSubmit_DoesNotCrash) {
    QuizEngine engine;
    engine.start(quizManager.findQuiz(1), &bank);
    engine.answer("C");
    engine.next();
    engine.answer("true");
    engine.submit();

    EXPECT_NO_THROW(engine.showResult());
}

// Edge case A
TEST_F(QuizEngineTest, Next_BeforeStart_ReturnsFalse) {
    QuizEngine engine;
    EXPECT_FALSE(engine.next());
    EXPECT_EQ(engine.getState(), NOT_STARTED);
}

TEST_F(QuizEngineTest, Previous_BeforeStart_ReturnsFalse) {
    QuizEngine engine;
    EXPECT_FALSE(engine.previous());
    EXPECT_EQ(engine.getState(), NOT_STARTED);
}

TEST_F(QuizEngineTest, ShowResult_BeforeStart_DoesNotCrash) {
    QuizEngine engine;
    EXPECT_NO_THROW(engine.showResult());
}

// Edge case B
TEST_F(QuizEngineTest, Start_NullQuiz_Rejected_NoStateChange) {
    QuizEngine engine;
    EXPECT_FALSE(engine.start(nullptr, &bank));
    EXPECT_EQ(engine.getState(), NOT_STARTED);
}

TEST_F(QuizEngineTest, Start_NullBank_Rejected_NoStateChange) {
    QuizEngine engine;
    EXPECT_FALSE(engine.start(quizManager.findQuiz(1), nullptr));
    EXPECT_EQ(engine.getState(), NOT_STARTED);
}

// Edge case C
TEST_F(QuizEngineTest, Start_WhileInProgress_Rejected_SessionPreserved) {
    QuizEngine engine;
    engine.start(quizManager.findQuiz(1), &bank);
    engine.answer("C");

    EXPECT_FALSE(engine.start(quizManager.findQuiz(1), &bank));

    EXPECT_EQ(engine.getState(), IN_PROGRESS);
    EXPECT_EQ(engine.getCurrentAnswer(), "C");
}