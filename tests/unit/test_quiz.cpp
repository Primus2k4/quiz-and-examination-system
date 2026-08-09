#include <gtest/gtest.h>
#include <string>

#include "../include/Quiz.h"
#include "../include/QuizManager.h"
#include "../include/QuestionBank.h"
#include "../include/MCQ.h"
#include "../include/TF.h"

static void populateBank(QuestionBank& bank)
{
    std::string opts[4] = { "A", "B", "C", "D" };
    bank.addQuestion(new MCQ(101, "MCQ question 1", 2, opts, 'A'));
    bank.addQuestion(new MCQ(102, "MCQ question 2", 2, opts, 'B'));
    bank.addQuestion(new TF(103, "TF question 1", 1, true));
    bank.addQuestion(new TF(104, "TF question 2", 1, false));
    bank.addQuestion(new MCQ(105, "MCQ question 3", 2, opts, 'C'));
}

class QuizManagerTest : public ::testing::Test {
protected:
    void SetUp() override { populateBank(bank); }

    QuestionBank bank;
    QuizManager  manager;
};

// Creating quizzes
TEST_F(QuizManagerTest, CreateQuiz_ValidIdAndTitle_Succeeds) {
    EXPECT_TRUE(manager.createQuiz(1, "Quiz One"));
    EXPECT_NE(manager.findQuiz(1), nullptr);
}

TEST_F(QuizManagerTest, CreateQuiz_CountIncreasesAfterCreate) {
    manager.createQuiz(1, "Quiz One");
    manager.createQuiz(2, "Quiz Two");
    EXPECT_EQ(manager.getCount(), 2);
}

TEST_F(QuizManagerTest, CreateQuiz_DuplicateId_Rejected) {
    manager.createQuiz(1, "Quiz One");
    EXPECT_FALSE(manager.createQuiz(1, "Quiz One Again"));
    EXPECT_EQ(manager.getCount(), 1);
}

TEST_F(QuizManagerTest, CreateQuiz_DuplicateTitle_Rejected) {
    manager.createQuiz(1, "Quiz One");
    EXPECT_FALSE(manager.createQuiz(2, "Quiz One"));
    EXPECT_EQ(manager.getCount(), 1);
}

TEST_F(QuizManagerTest, CreateQuiz_EmptyTitle_Rejected) {
    EXPECT_FALSE(manager.createQuiz(1, ""));
    EXPECT_EQ(manager.getCount(), 0);
}

TEST_F(QuizManagerTest, CreateQuiz_ZeroId_Rejected) {
    EXPECT_FALSE(manager.createQuiz(0, "Quiz Zero"));
    EXPECT_EQ(manager.getCount(), 0);
}

TEST_F(QuizManagerTest, CreateQuiz_NegativeId_Rejected) {
    EXPECT_FALSE(manager.createQuiz(-1, "Quiz Negative"));
    EXPECT_EQ(manager.getCount(), 0);
}

// Finding and listing quizzes
TEST_F(QuizManagerTest, FindQuiz_ExistingId_ReturnsCorrectQuiz) {
    manager.createQuiz(10, "Find Me");
    Quiz* q = manager.findQuiz(10);
    ASSERT_NE(q, nullptr);
    EXPECT_EQ(q->getTitle(), "Find Me");
}

TEST_F(QuizManagerTest, FindQuiz_NonExistingId_ReturnsNullptr) {
    EXPECT_EQ(manager.findQuiz(999), nullptr);
}

TEST_F(QuizManagerTest, FindQuiz_EmptyManager_ReturnsNullptr) {
    EXPECT_EQ(manager.findQuiz(1), nullptr);
}

// Renaming quizzes
TEST_F(QuizManagerTest, RenameQuiz_ValidNewTitle_Succeeds) {
    manager.createQuiz(1, "Old Title");
    EXPECT_TRUE(manager.renameQuiz(1, "New Title"));
    EXPECT_EQ(manager.findQuiz(1)->getTitle(), "New Title");
}

TEST_F(QuizManagerTest, RenameQuiz_EmptyTitle_Rejected) {
    manager.createQuiz(1, "Old Title");
    EXPECT_FALSE(manager.renameQuiz(1, ""));
    EXPECT_EQ(manager.findQuiz(1)->getTitle(), "Old Title");
}

TEST_F(QuizManagerTest, RenameQuiz_DuplicateTitle_Rejected) {
    manager.createQuiz(1, "Quiz One");
    manager.createQuiz(2, "Quiz Two");
    EXPECT_FALSE(manager.renameQuiz(2, "Quiz One"));
    EXPECT_EQ(manager.findQuiz(2)->getTitle(), "Quiz Two");
}

TEST_F(QuizManagerTest, RenameQuiz_SameTitleAsItself_Succeeds) {
    manager.createQuiz(1, "Quiz One");
    EXPECT_TRUE(manager.renameQuiz(1, "Quiz One"));
}

TEST_F(QuizManagerTest, RenameQuiz_NonExistingId_Rejected) {
    EXPECT_FALSE(manager.renameQuiz(999, "Ghost Quiz"));
}

// Deleting quizzes
TEST_F(QuizManagerTest, DeleteQuiz_ExistingId_Succeeds) {
    manager.createQuiz(1, "Delete Me");
    EXPECT_TRUE(manager.deleteQuiz(1));
    EXPECT_EQ(manager.findQuiz(1), nullptr);
    EXPECT_EQ(manager.getCount(), 0);
}

TEST_F(QuizManagerTest, DeleteQuiz_NonExistingId_Rejected) {
    EXPECT_FALSE(manager.deleteQuiz(999));
}

TEST_F(QuizManagerTest, DeleteQuiz_EmptyManager_Rejected) {
    EXPECT_FALSE(manager.deleteQuiz(1));
}

TEST_F(QuizManagerTest, DeleteQuiz_CountDecreasesAfterDelete) {
    manager.createQuiz(1, "Q1");
    manager.createQuiz(2, "Q2");
    manager.deleteQuiz(1);
    EXPECT_EQ(manager.getCount(), 1);
    EXPECT_NE(manager.findQuiz(2), nullptr);
}

// Adding questions to a quiz
TEST_F(QuizManagerTest, AddQuestionToQuiz_ValidId_Succeeds) {
    manager.createQuiz(1, "Quiz One");
    EXPECT_TRUE(manager.addQuestionToQuiz(1, 101, bank));
    EXPECT_EQ(manager.findQuiz(1)->getQuestionCount(), 1);
}

TEST_F(QuizManagerTest, AddQuestionToQuiz_IdNotInBank_Rejected_TC04) {
    manager.createQuiz(1, "Quiz One");
    EXPECT_FALSE(manager.addQuestionToQuiz(1, 999, bank));
    EXPECT_EQ(manager.findQuiz(1)->getQuestionCount(), 0);
}

TEST_F(QuizManagerTest, AddQuestionToQuiz_DuplicateId_Rejected_TC04) {
    manager.createQuiz(1, "Quiz One");
    manager.addQuestionToQuiz(1, 101, bank);
    EXPECT_FALSE(manager.addQuestionToQuiz(1, 101, bank));
    EXPECT_EQ(manager.findQuiz(1)->getQuestionCount(), 1);
}

TEST_F(QuizManagerTest, AddQuestionToQuiz_NonExistingQuiz_Rejected) {
    EXPECT_FALSE(manager.addQuestionToQuiz(999, 101, bank));
}

TEST_F(QuizManagerTest, AddQuestionToQuiz_PreservesInsertionOrder) {
    manager.createQuiz(1, "Ordered Quiz");
    manager.addQuestionToQuiz(1, 103, bank);
    manager.addQuestionToQuiz(1, 101, bank);
    manager.addQuestionToQuiz(1, 105, bank);

    Quiz* q = manager.findQuiz(1);
    ASSERT_NE(q, nullptr);
    EXPECT_EQ(q->getQuestionId(0), 103);
    EXPECT_EQ(q->getQuestionId(1), 101);
    EXPECT_EQ(q->getQuestionId(2), 105);
}

// Removing questions from a quiz
TEST_F(QuizManagerTest, RemoveQuestionFromQuiz_ExistingId_Succeeds) {
    manager.createQuiz(1, "Quiz One");
    manager.addQuestionToQuiz(1, 101, bank);
    manager.addQuestionToQuiz(1, 102, bank);

    EXPECT_TRUE(manager.removeQuestionFromQuiz(1, 101));
    EXPECT_EQ(manager.findQuiz(1)->getQuestionCount(), 1);
    EXPECT_EQ(manager.findQuiz(1)->getQuestionId(0), 102);
}

TEST_F(QuizManagerTest, RemoveQuestionFromQuiz_IdNotInQuiz_Rejected) {
    manager.createQuiz(1, "Quiz One");
    manager.addQuestionToQuiz(1, 101, bank);

    EXPECT_FALSE(manager.removeQuestionFromQuiz(1, 102));
    EXPECT_EQ(manager.findQuiz(1)->getQuestionCount(), 1);
}

TEST_F(QuizManagerTest, RemoveQuestionFromQuiz_NonExistingQuiz_Rejected) {
    EXPECT_FALSE(manager.removeQuestionFromQuiz(999, 101));
}

TEST_F(QuizManagerTest, RemoveQuestionFromQuiz_LastQuestion_QuizBecomesEmpty) {
    manager.createQuiz(1, "Quiz One");
    manager.addQuestionToQuiz(1, 101, bank);

    EXPECT_TRUE(manager.removeQuestionFromQuiz(1, 101));
    EXPECT_EQ(manager.findQuiz(1)->getQuestionCount(), 0);
}

// Capacity limit
TEST_F(QuizManagerTest, AddExactly30Questions_AllSucceed) {
    QuestionBank bigBank;
    std::string opts[4] = { "A", "B", "C", "D" };
    for (int i = 0; i < 30; ++i)
        bigBank.addQuestion(new MCQ(200 + i, "Question " + std::to_string(i), 1, opts, 'A'));

    QuizManager mgr;
    mgr.createQuiz(1, "Big Quiz");
    for (int i = 0; i < 30; ++i) {
        EXPECT_TRUE(mgr.addQuestionToQuiz(1, 200 + i, bigBank))
            << "Failed at question index " << i;
    }
    EXPECT_EQ(mgr.findQuiz(1)->getQuestionCount(), 30);
}

TEST_F(QuizManagerTest, Add31stQuestion_Rejected) {
    QuestionBank bigBank;
    std::string opts[4] = { "A", "B", "C", "D" };
    for (int i = 0; i < 31; ++i)
        bigBank.addQuestion(new MCQ(200 + i, "Question " + std::to_string(i), 1, opts, 'A'));

    QuizManager mgr;
    mgr.createQuiz(1, "Big Quiz");
    for (int i = 0; i < 30; ++i)
        mgr.addQuestionToQuiz(1, 200 + i, bigBank);

    EXPECT_FALSE(mgr.addQuestionToQuiz(1, 230, bigBank));
    EXPECT_EQ(mgr.findQuiz(1)->getQuestionCount(), 30);
}

TEST_F(QuizManagerTest, CreateExactly10Quizzes_AllSucceed) {
    for (int i = 1; i <= 10; ++i)
        EXPECT_TRUE(manager.createQuiz(i, "Quiz " + std::to_string(i)));
    EXPECT_EQ(manager.getCount(), 10);
}

TEST_F(QuizManagerTest, Create11thQuiz_Rejected) {
    for (int i = 1; i <= 10; ++i)
        manager.createQuiz(i, "Quiz " + std::to_string(i));

    EXPECT_FALSE(manager.createQuiz(11, "Quiz Eleven"));
    EXPECT_EQ(manager.getCount(), 10);
}

// Edge cases
TEST_F(QuizManagerTest, GetQuestionId_OutOfRange_ReturnsInvalidSentinel) {
    manager.createQuiz(1, "Quiz One");
    manager.addQuestionToQuiz(1, 101, bank);

    Quiz* q = manager.findQuiz(1);
    EXPECT_EQ(q->getQuestionId(1), -1);
    EXPECT_EQ(q->getQuestionId(-1), -1);
}

TEST_F(QuizManagerTest, AddToQuiz_AfterQuizDeleted_Rejected) {
    manager.createQuiz(1, "Temp Quiz");
    manager.deleteQuiz(1);
    EXPECT_FALSE(manager.addQuestionToQuiz(1, 101, bank));
}