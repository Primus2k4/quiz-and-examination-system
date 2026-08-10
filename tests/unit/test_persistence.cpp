#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <sstream>
#include <sys/stat.h>

#include "../include/DataFileManager.h"
#include "../include/QuestionBank.h"
#include "../include/QuizManager.h"
#include "../include/MCQ.h"
#include "../include/TF.h"

// ================================================================
// Fixture: creates a temp directory and manages test file lifetimes.
// ================================================================
class PersistenceTest : public ::testing::Test {
protected:
    const std::string kQuestionsPath = "gtest_questions.txt";
    const std::string kQuizzesPath = "gtest_quizzes.txt";
    const std::string kReadOnlyPath = "gtest_readonly.txt";
    const std::string kWriteOnlyPath = "gtest_writeonly.txt";

    void SetUp() override {
        removeIfExists(kQuestionsPath);
        removeIfExists(kQuizzesPath);
        removeIfExists(kReadOnlyPath);
        removeIfExists(kWriteOnlyPath);
    }

    void TearDown() override {
        removeIfExists(kQuestionsPath);
        removeIfExists(kQuizzesPath);
        removeIfExists(kReadOnlyPath);
        removeIfExists(kWriteOnlyPath);
    }

    static void removeIfExists(const std::string& path) {
        std::remove(path.c_str());
    }

    static void writeFile(const std::string& path, const std::string& content) {
        std::ofstream f(path);
        f << content;
    }

    static bool fileExists(const std::string& path) {
        std::ifstream f(path);
        return f.good();
    }

    static std::string readFile(const std::string& path) {
        std::ifstream f(path);
        std::ostringstream buf;
        buf << f.rdbuf();
        return buf.str();
    }

    static void buildSampleBank(QuestionBank& bank) {
        std::string opts[4] = { "3", "4", "5", "6" };
        bank.addQuestion(new MCQ(101, "2 + 2 = ?", 2, opts, 'B'));
        bank.addQuestion(new TF(102, "C++ supports inheritance", 1, true));
    }

    static void buildSampleManager(QuizManager& manager, QuestionBank& bank) {
        manager.createQuiz(201, "C++ Basics");
        manager.addQuestionToQuiz(201, 101, bank);
        manager.addQuestionToQuiz(201, 102, bank);
    }
};

// ================================================================
// TC-16: File does not exist
// ================================================================

TEST_F(PersistenceTest, LoadQuestions_FileNotExist_ReturnsEmptyBank_TC16) {
    QuestionBank bank;
    bool ok = DataFileManager::loadQuestions(bank, kQuestionsPath);

    EXPECT_EQ(bank.getCount(), 0);
    (void)ok;
}

TEST_F(PersistenceTest, LoadQuizzes_FileNotExist_ReturnsEmptyManager_TC16) {
    QuestionBank bank;
    QuizManager  manager;
    buildSampleBank(bank);

    DataFileManager::loadQuizzes(manager, bank, kQuizzesPath);
    EXPECT_EQ(manager.getCount(), 0);
}

TEST_F(PersistenceTest, LoadMissing_DoesNotCrash_TC16) {
    QuestionBank bank;
    QuizManager  manager;
    EXPECT_NO_THROW({
        DataFileManager::loadQuestions(bank, kQuestionsPath);
        DataFileManager::loadQuizzes(manager, bank, kQuizzesPath);
        });
}

// ================================================================
// TC-01: File with mixed valid and malformed lines
// ================================================================

TEST_F(PersistenceTest, LoadQuestions_MalformedLines_SkippedNoCrash_TC01) {
    writeFile(kQuestionsPath,
        "MCQ|101|2|2 + 2 = ?|3;4;5;6|B\n"
        "TF|102|1|C++ supports inheritance|true\n"
        "MCQ|103|2|Question 3|A;B;C;D|A\n"
        "TF|104|1|Is water wet?|false\n"
        "MCQ|105|2|Question 5|A;B;C;D|C\n"
        "INVALID_LINE_WITH_NO_PIPES\n"
    );

    QuestionBank bank;
    EXPECT_NO_THROW(DataFileManager::loadQuestions(bank, kQuestionsPath));
    EXPECT_EQ(bank.getCount(), 5);
}

TEST_F(PersistenceTest, LoadQuestions_UnknownType_SkippedNoCrash) {
    writeFile(kQuestionsPath,
        "MCQ|101|2|Valid question|A;B;C;D|A\n"
        "XYZ|102|1|Unknown type question|maybe\n"
    );

    QuestionBank bank;
    EXPECT_NO_THROW(DataFileManager::loadQuestions(bank, kQuestionsPath));
    EXPECT_EQ(bank.getCount(), 1);
}

TEST_F(PersistenceTest, LoadQuestions_InvalidPoints_SkippedNoCrash) {
    writeFile(kQuestionsPath,
        "TF|101|0|Zero points question|true\n"
        "TF|102|1|Valid TF question|true\n"
    );

    QuestionBank bank;
    EXPECT_NO_THROW(DataFileManager::loadQuestions(bank, kQuestionsPath));
    EXPECT_EQ(bank.getCount(), 1);
}

TEST_F(PersistenceTest, LoadQuestions_EmptyPrompt_SkippedNoCrash) {
    writeFile(kQuestionsPath,
        "TF|101|1||true\n"
        "TF|102|1|Valid prompt|true\n"
    );

    QuestionBank bank;
    EXPECT_NO_THROW(DataFileManager::loadQuestions(bank, kQuestionsPath));
    EXPECT_EQ(bank.getCount(), 1);
}

TEST_F(PersistenceTest, LoadQuestions_MCQWrongOptionCount_SkippedNoCrash) {
    writeFile(kQuestionsPath,
        "MCQ|101|2|Question|A;B;C|A\n"
        "TF|102|1|Valid TF|true\n"
    );

    QuestionBank bank;
    EXPECT_NO_THROW(DataFileManager::loadQuestions(bank, kQuestionsPath));
    EXPECT_EQ(bank.getCount(), 1);
}

TEST_F(PersistenceTest, LoadQuestions_MCQInvalidCorrectOption_SkippedNoCrash) {
    writeFile(kQuestionsPath,
        "MCQ|101|2|Question|A;B;C;D|Z\n"
        "TF|102|1|Valid TF|true\n"
    );

    QuestionBank bank;
    EXPECT_NO_THROW(DataFileManager::loadQuestions(bank, kQuestionsPath));
    EXPECT_EQ(bank.getCount(), 1);
}

TEST_F(PersistenceTest, LoadQuestions_EmptyFile_NoCrash) {
    writeFile(kQuestionsPath, "");
    QuestionBank bank;
    EXPECT_NO_THROW(DataFileManager::loadQuestions(bank, kQuestionsPath));
    EXPECT_EQ(bank.getCount(), 0);
}

TEST_F(PersistenceTest, LoadQuizzes_QuestionIdNotInBank_LineSkipped) {
    writeFile(kQuizzesPath,
        "201|Valid Quiz|101,102\n"
        "202|Ghost Quiz|999\n"
    );

    QuestionBank bank;
    buildSampleBank(bank);

    QuizManager manager;
    EXPECT_NO_THROW(DataFileManager::loadQuizzes(manager, bank, kQuizzesPath));
    EXPECT_EQ(manager.getCount(), 1);
}

// ================================================================
// Round-trip: save then reload (TC-15)
// ================================================================

TEST_F(PersistenceTest, SaveAndReloadQuestions_DataMatchesOriginal_TC15) {
    QuestionBank original;
    buildSampleBank(original);

    EXPECT_TRUE(DataFileManager::saveQuestions(original, kQuestionsPath));

    QuestionBank reloaded;
    DataFileManager::loadQuestions(reloaded, kQuestionsPath);

    ASSERT_EQ(reloaded.getCount(), original.getCount());

    for (int i = 0; i < original.getCount(); ++i) {
        Question* orig = original.getAt(i);
        Question* rel = reloaded.findById(orig->getId());
        ASSERT_NE(rel, nullptr);
        EXPECT_EQ(rel->getType(), orig->getType());
        EXPECT_EQ(rel->getPrompt(), orig->getPrompt());
        EXPECT_EQ(rel->getPoints(), orig->getPoints());
    }
}

TEST_F(PersistenceTest, SaveAndReloadQuizzes_DataMatchesOriginal_TC15) {
    QuestionBank bank;
    buildSampleBank(bank);

    QuizManager original;
    buildSampleManager(original, bank);

    EXPECT_TRUE(DataFileManager::saveQuestions(bank, kQuestionsPath));
    EXPECT_TRUE(DataFileManager::saveQuizzes(original, kQuizzesPath));

    QuestionBank reloadedBank;
    DataFileManager::loadQuestions(reloadedBank, kQuestionsPath);

    QuizManager reloaded;
    DataFileManager::loadQuizzes(reloaded, reloadedBank, kQuizzesPath);

    ASSERT_EQ(reloaded.getCount(), original.getCount());

    Quiz* origQuiz = original.findQuiz(201);
    Quiz* reloadedQuiz = reloaded.findQuiz(201);
    ASSERT_NE(origQuiz, nullptr);
    ASSERT_NE(reloadedQuiz, nullptr);
    EXPECT_EQ(reloadedQuiz->getTitle(), origQuiz->getTitle());
    EXPECT_EQ(reloadedQuiz->getQuestionCount(), origQuiz->getQuestionCount());
}

TEST_F(PersistenceTest, SaveAndReload_MCQOptionsAndCorrectOption_Preserved_TC15) {
    std::string opts[4] = { "3", "4", "5", "6" };
    QuestionBank bank;
    bank.addQuestion(new MCQ(101, "2 + 2 = ?", 2, opts, 'B'));

    DataFileManager::saveQuestions(bank, kQuestionsPath);

    QuestionBank reloaded;
    DataFileManager::loadQuestions(reloaded, kQuestionsPath);

    MCQ* q = dynamic_cast<MCQ*>(reloaded.findById(101));
    ASSERT_NE(q, nullptr);
    EXPECT_EQ(q->getOption(0), "3");
    EXPECT_EQ(q->getOption(1), "4");
    EXPECT_EQ(q->getOption(2), "5");
    EXPECT_EQ(q->getOption(3), "6");
    EXPECT_EQ(q->getCorrectOption(), 'B');
}

TEST_F(PersistenceTest, SaveAndReload_TFCorrectAnswer_Preserved_TC15) {
    QuestionBank bank;
    bank.addQuestion(new TF(102, "C++ supports inheritance", 1, true));
    bank.addQuestion(new TF(103, "Java is C++", 1, false));

    DataFileManager::saveQuestions(bank, kQuestionsPath);

    QuestionBank reloaded;
    DataFileManager::loadQuestions(reloaded, kQuestionsPath);

    TF* tf_true = dynamic_cast<TF*>(reloaded.findById(102));
    TF* tf_false = dynamic_cast<TF*>(reloaded.findById(103));
    ASSERT_NE(tf_true, nullptr);
    ASSERT_NE(tf_false, nullptr);
    EXPECT_TRUE(tf_true->getCorrectAnswer());
    EXPECT_FALSE(tf_false->getCorrectAnswer());
}

TEST_F(PersistenceTest, SaveAndReload_QuizQuestionOrder_Preserved_TC15) {
    QuestionBank bank;
    buildSampleBank(bank);

    QuizManager manager;
    manager.createQuiz(201, "Order Test");
    manager.addQuestionToQuiz(201, 102, bank);
    manager.addQuestionToQuiz(201, 101, bank);

    DataFileManager::saveQuestions(bank, kQuestionsPath);
    DataFileManager::saveQuizzes(manager, kQuizzesPath);

    QuestionBank reloadedBank;
    DataFileManager::loadQuestions(reloadedBank, kQuestionsPath);
    QuizManager reloadedManager;
    DataFileManager::loadQuizzes(reloadedManager, reloadedBank, kQuizzesPath);

    Quiz* q = reloadedManager.findQuiz(201);
    ASSERT_NE(q, nullptr);
    EXPECT_EQ(q->getQuestionId(0), 102);
    EXPECT_EQ(q->getQuestionId(1), 101);
}

// ================================================================
// File format matching spec
// ================================================================

TEST_F(PersistenceTest, SaveQuestions_OutputFormat_MatchesSpec) {
    std::string opts[4] = { "3", "4", "5", "6" };
    QuestionBank bank;
    bank.addQuestion(new MCQ(101, "2 + 2 = ?", 2, opts, 'B'));
    bank.addQuestion(new TF(102, "C++ supports inheritance", 1, true));

    DataFileManager::saveQuestions(bank, kQuestionsPath);
    std::string content = readFile(kQuestionsPath);

    EXPECT_NE(content.find("MCQ|101|2|2 + 2 = ?|3;4;5;6|B"), std::string::npos);
    EXPECT_NE(content.find("TF|102|1|C++ supports inheritance|true"), std::string::npos);
}

TEST_F(PersistenceTest, SaveQuizzes_OutputFormat_MatchesSpec) {
    QuestionBank bank;
    buildSampleBank(bank);
    QuizManager manager;
    buildSampleManager(manager, bank);

    DataFileManager::saveQuizzes(manager, kQuizzesPath);
    std::string content = readFile(kQuizzesPath);

    EXPECT_NE(content.find("201|C++ Basics|101,102"), std::string::npos);
}

// ================================================================
// Saving empty structures
// ================================================================

TEST_F(PersistenceTest, SaveEmptyBank_CreatesEmptyFile) {
    QuestionBank bank;
    EXPECT_TRUE(DataFileManager::saveQuestions(bank, kQuestionsPath));
    EXPECT_TRUE(fileExists(kQuestionsPath));

    QuestionBank reloaded;
    DataFileManager::loadQuestions(reloaded, kQuestionsPath);
    EXPECT_EQ(reloaded.getCount(), 0);
}

TEST_F(PersistenceTest, SaveEmptyManager_CreatesEmptyFile) {
    QuizManager manager;
    EXPECT_TRUE(DataFileManager::saveQuizzes(manager, kQuizzesPath));
    EXPECT_TRUE(fileExists(kQuizzesPath));

    QuestionBank  bank;
    QuizManager   reloaded;
    DataFileManager::loadQuizzes(reloaded, bank, kQuizzesPath);
    EXPECT_EQ(reloaded.getCount(), 0);
}