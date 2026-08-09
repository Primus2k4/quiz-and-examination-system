#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <cstdio>

#include "../include/QuestionBank.h"
#include "../include/QuizManager.h"
#include "../include/QuizEngine.h"
#include "../include/DataFileManager.h"
#include "../include/MCQ.h"
#include "../include/TF.h"

class IntegrationTest : public ::testing::Test {
protected:
    const std::string kQuestionsPath = "gtest_int_questions.txt";
    const std::string kQuizzesPath = "gtest_int_quizzes.txt";

    std::string opts[4] = { "A", "B", "C", "D" };

    void SetUp() override { removeTempFiles(); }
    void TearDown() override { removeTempFiles(); }

    void removeTempFiles() {
        std::remove(kQuestionsPath.c_str());
        std::remove(kQuizzesPath.c_str());
    }

    static void writeFile(const std::string& path, const std::string& content) {
        std::ofstream f(path);
        f << content;
    }
};

// Scenario 1: Cascade Delete (TC-05)
TEST_F(IntegrationTest, Scenario1_CascadeDelete_TC05) {
    QuestionBank bank;
    QuizManager manager;

    bank.addQuestion(new MCQ(101, "MCQ 1", 2, opts, 'A'));
    bank.addQuestion(new TF(102, "TF 1", 1, true));

    manager.createQuiz(201, "Quiz One");
    manager.addQuestionToQuiz(201, 101, bank);
    manager.addQuestionToQuiz(201, 102, bank);

    manager.createQuiz(202, "Quiz Two");
    manager.addQuestionToQuiz(202, 101, bank);

    EXPECT_TRUE(bank.removeQuestion(101));
    manager.removeQuestionFromAllQuiz(101); // Khớp hàm thực tế trong QuizManager

    EXPECT_EQ(manager.findQuiz(201)->getQuestionCount(), 1);
    EXPECT_EQ(manager.findQuiz(201)->getQuestionId(0), 102); // Khớp hàm thực tế getQuestionId
    EXPECT_EQ(manager.findQuiz(202)->getQuestionCount(), 0);
}

// Scenario 2: Full Lifecycle (TC-06, TC-11)
TEST_F(IntegrationTest, Scenario2_FullLifecycle_LoadToSubmit_TC06_TC11) {
    writeFile(kQuestionsPath,
        "MCQ|101|2|What is 2 + 2?|3;4;5;6|B\n"
        "TF|102|1|C++ supports inheritance|true\n"
    );
    writeFile(kQuizzesPath, "201|C++ Basics|101,102\n");

    QuestionBank bank;
    QuizManager manager;

    ASSERT_TRUE(DataFileManager::loadQuestions(bank, kQuestionsPath));
    ASSERT_TRUE(DataFileManager::loadQuizzes(manager, bank, kQuizzesPath));

    QuizEngine engine;
    ASSERT_TRUE(engine.start(manager.findQuiz(201), &bank));

    engine.answer("B");
    engine.next();
    engine.answer("false");

    ASSERT_TRUE(engine.submit());
    EXPECT_EQ(engine.getScore(), 2);
    EXPECT_EQ(engine.getState(), SUBMITTED); // Dùng enum SUBMITTED trực tiếp
}

// Scenario 3: Persistence Sync (TC-15)
TEST_F(IntegrationTest, Scenario3_ModifyData_SaveAndReload_TC15) {
    QuestionBank bank;
    QuizManager manager;

    bank.addQuestion(new MCQ(101, "Original Prompt", 2, opts, 'A'));
    manager.createQuiz(201, "Original Title");
    manager.addQuestionToQuiz(201, 101, bank);

    MCQ* updated = new MCQ(101, "Updated Prompt", 5, opts, 'C');
    EXPECT_TRUE(bank.updateQuestion(101, updated));

    EXPECT_TRUE(DataFileManager::saveQuestions(bank, kQuestionsPath));
    EXPECT_TRUE(DataFileManager::saveQuizzes(manager, kQuizzesPath));

    QuestionBank reloadedBank;
    QuizManager reloadedManager;
    ASSERT_TRUE(DataFileManager::loadQuestions(reloadedBank, kQuestionsPath));
    ASSERT_TRUE(DataFileManager::loadQuizzes(reloadedManager, reloadedBank, kQuizzesPath));

    Question* q = reloadedBank.findById(101);
    ASSERT_NE(q, nullptr);
    EXPECT_EQ(q->getPrompt(), "Updated Prompt");
    EXPECT_EQ(q->getPoints(), 5);
}

// Scenario 4: Dynamic Bank State
TEST_F(IntegrationTest, Scenario4_Engine_ReflectsUpdatedBankPoints) {
    QuestionBank bank;
    QuizManager manager;

    bank.addQuestion(new MCQ(101, "Prompt", 2, opts, 'A'));
    manager.createQuiz(201, "Dynamic Quiz");
    manager.addQuestionToQuiz(201, 101, bank);

    MCQ* updated = new MCQ(101, "Prompt", 10, opts, 'A');
    bank.updateQuestion(101, updated);

    QuizEngine engine;
    ASSERT_TRUE(engine.start(manager.findQuiz(201), &bank));
    engine.answer("A");
    engine.submit();

    EXPECT_EQ(engine.getScore(), 10);
}

// Scenario 5: Fault-Tolerant Load (TC-01)
TEST_F(IntegrationTest, Scenario5_FaultTolerantLoad_EngineSessionUnbroken_TC01) {
    writeFile(kQuestionsPath,
        "MCQ|101|2|Valid Question|A;B;C;D|A\n"
        "CORRUPTED_LINE_WITHOUT_PIPES\n"
    );
    writeFile(kQuizzesPath, "201|Robust Quiz|101,999\n");

    QuestionBank bank;
    QuizManager manager;

    EXPECT_NO_THROW(DataFileManager::loadQuestions(bank, kQuestionsPath));
    EXPECT_NO_THROW(DataFileManager::loadQuizzes(manager, bank, kQuizzesPath));

    QuizEngine engine;
    EXPECT_TRUE(engine.start(manager.findQuiz(201), &bank));
    engine.answer("A");
    EXPECT_TRUE(engine.submit());
    EXPECT_EQ(engine.getScore(), 2);
}