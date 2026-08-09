#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "../include/QuestionBank.h"
#include "../include/MCQ.h"
#include "../include/TF.h"

// ================================================================
// Helpers
// ================================================================

// Creates a valid MCQ on the heap. Caller owns the pointer.
static MCQ* makeMCQ(int id,
    const std::string& prompt = "What is 2 + 2?",
    int points = 2,
    char correct = 'B')
{
    std::string opts[4] = { "3", "4", "5", "6" };
    return new MCQ(id, prompt, points, opts, correct);
}

// Returns a string captured from cout while func() runs.
template <typename Func>
static std::string captureStdout(Func func)
{
    std::ostringstream buf;
    std::streambuf* old = std::cout.rdbuf(buf.rdbuf());
    func();
    std::cout.rdbuf(old);
    return buf.str();
}

// ================================================================
// Fixture: fresh QuestionBank for each test
// ================================================================
class QuestionBankTest : public ::testing::Test {
protected:
    QuestionBank bank;
    std::string  opts[4] = { "A", "B", "C", "D" };
};

// ================================================================
// Basic state on an empty bank
// ================================================================

TEST_F(QuestionBankTest, EmptyBank_CountIsZero) {
    EXPECT_EQ(bank.getCount(), 0);
}

TEST_F(QuestionBankTest, EmptyBank_GetAtReturnsNullptr) {
    EXPECT_EQ(bank.getAt(-1), nullptr);
    EXPECT_EQ(bank.getAt(0), nullptr);
}

TEST_F(QuestionBankTest, EmptyBank_FindByIdReturnsNullptr) {
    EXPECT_EQ(bank.findById(1), nullptr);
}

TEST_F(QuestionBankTest, AddNullptr_Rejected) {
    EXPECT_FALSE(bank.addQuestion(nullptr));
}

// ================================================================
// Adding valid questions (FR-01)
// ================================================================

TEST_F(QuestionBankTest, AddValidMCQ_Succeeds) {
    EXPECT_TRUE(bank.addQuestion(new MCQ(101, "Prompt", 2, opts, 'A')));
    EXPECT_EQ(bank.getCount(), 1);
}

TEST_F(QuestionBankTest, AddValidTF_Succeeds) {
    EXPECT_TRUE(bank.addQuestion(new TF(102, "Prompt", 1, true)));
    EXPECT_EQ(bank.getCount(), 1);
}

TEST_F(QuestionBankTest, FindById_ReturnsCorrectType) {
    bank.addQuestion(new MCQ(101, "MCQ prompt", 2, opts, 'A'));
    bank.addQuestion(new TF(102, "TF prompt", 1, true));

    Question* q101 = bank.findById(101);
    Question* q102 = bank.findById(102);

    ASSERT_NE(q101, nullptr);
    ASSERT_NE(q102, nullptr);
    EXPECT_EQ(q101->getType(), "MCQ");
    EXPECT_EQ(q102->getType(), "TF");
}

TEST_F(QuestionBankTest, GetAt_ReturnsQuestionsInInsertionOrder) {
    bank.addQuestion(new MCQ(101, "First", 2, opts, 'A'));
    bank.addQuestion(new TF(102, "Second", 1, true));

    EXPECT_EQ(bank.getAt(0)->getId(), 101);
    EXPECT_EQ(bank.getAt(1)->getId(), 102);
    EXPECT_EQ(bank.getAt(2), nullptr);
}

// ================================================================
// Rejection of invalid additions (TC-02)
// ================================================================

TEST_F(QuestionBankTest, AddDuplicateId_Rejected) {
    bank.addQuestion(new MCQ(101, "Original", 2, opts, 'A'));
    MCQ* dup = new MCQ(101, "Duplicate", 2, opts, 'B');
    EXPECT_FALSE(bank.addQuestion(dup));
    EXPECT_EQ(bank.getCount(), 1);
    delete dup;
}

TEST_F(QuestionBankTest, AddEmptyPrompt_Rejected) {
    MCQ* q = new MCQ(103, "", 2, opts, 'A');
    EXPECT_FALSE(bank.addQuestion(q));
    EXPECT_EQ(bank.getCount(), 0);
    delete q;
}

TEST_F(QuestionBankTest, AddZeroPoints_Rejected) {
    MCQ* q = new MCQ(103, "Prompt", 0, opts, 'A');
    EXPECT_FALSE(bank.addQuestion(q));
    EXPECT_EQ(bank.getCount(), 0);
    delete q;
}

TEST_F(QuestionBankTest, AddEmptyMCQOption_Rejected) {
    std::string badOpts[4] = { "A", "", "C", "D" };
    MCQ* q = new MCQ(103, "Prompt", 2, badOpts, 'A');
    EXPECT_FALSE(bank.addQuestion(q));
    EXPECT_EQ(bank.getCount(), 0);
    delete q;
}

TEST_F(QuestionBankTest, AddInvalidMCQCorrectOption_Rejected) {
    MCQ* q = new MCQ(103, "Prompt", 2, opts, 'Z');
    EXPECT_FALSE(bank.addQuestion(q));
    EXPECT_EQ(bank.getCount(), 0);
    delete q;
}

TEST_F(QuestionBankTest, FindById_NonExisting_ReturnsNullptr) {
    bank.addQuestion(new MCQ(101, "Prompt", 2, opts, 'A'));
    EXPECT_EQ(bank.findById(999), nullptr);
}

// ================================================================
// Update question (FR-01)
// ================================================================

TEST_F(QuestionBankTest, UpdateQuestion_ValidReplacement_Succeeds) {
    bank.addQuestion(new MCQ(201, "Original", 2, opts, 'A'));
    MCQ* updated = new MCQ(201, "Updated", 5, opts, 'C');

    EXPECT_TRUE(bank.updateQuestion(201, updated));
    Question* q = bank.findById(201);
    ASSERT_NE(q, nullptr);
    EXPECT_EQ(q->getPrompt(), "Updated");
    EXPECT_EQ(q->getPoints(), 5);
    EXPECT_TRUE(q->checkAnswer("c")); // case-insensitive
}

TEST_F(QuestionBankTest, UpdateQuestion_MismatchedId_Rejected) {
    bank.addQuestion(new MCQ(201, "Original", 2, opts, 'A'));
    MCQ* wrong = new MCQ(999, "Wrong ID", 2, opts, 'A');
    EXPECT_FALSE(bank.updateQuestion(201, wrong));
    EXPECT_EQ(bank.findById(201)->getPrompt(), "Original"); // unchanged
    delete wrong;
}

TEST_F(QuestionBankTest, UpdateQuestion_MismatchedType_Rejected) {
    bank.addQuestion(new MCQ(201, "Original", 2, opts, 'A'));
    TF* wrong = new TF(201, "Wrong type", 2, true);
    EXPECT_FALSE(bank.updateQuestion(201, wrong));
    EXPECT_EQ(bank.findById(201)->getType(), "MCQ"); // type unchanged
    delete wrong;
}

TEST_F(QuestionBankTest, UpdateQuestion_InvalidReplacement_Rejected) {
    bank.addQuestion(new MCQ(201, "Original", 2, opts, 'A'));
    MCQ* bad = new MCQ(201, "", 2, opts, 'A'); // empty prompt
    EXPECT_FALSE(bank.updateQuestion(201, bad));
    delete bad;
}

TEST_F(QuestionBankTest, UpdateQuestion_NonExistingId_Rejected) {
    MCQ* q = makeMCQ(999);
    EXPECT_FALSE(bank.updateQuestion(999, q));
    delete q;
}

// ================================================================
// Remove question (FR-01)
// ================================================================

TEST_F(QuestionBankTest, RemoveMiddleQuestion_ShiftsRemaining) {
    bank.addQuestion(new MCQ(401, "Q1", 1, opts, 'A'));
    bank.addQuestion(new TF(402, "Q2", 1, true));
    bank.addQuestion(new MCQ(403, "Q3", 1, opts, 'C'));

    EXPECT_TRUE(bank.removeQuestion(402));
    EXPECT_EQ(bank.getCount(), 2);
    EXPECT_EQ(bank.findById(402), nullptr);
    EXPECT_EQ(bank.getAt(0)->getId(), 401);
    EXPECT_EQ(bank.getAt(1)->getId(), 403);
    EXPECT_EQ(bank.getAt(2), nullptr);
}

TEST_F(QuestionBankTest, RemoveFirstQuestion_RemainingShiftsLeft) {
    bank.addQuestion(new MCQ(401, "Q1", 1, opts, 'A'));
    bank.addQuestion(new TF(402, "Q2", 1, true));

    EXPECT_TRUE(bank.removeQuestion(401));
    EXPECT_EQ(bank.getAt(0)->getId(), 402);
}

TEST_F(QuestionBankTest, RemoveLastQuestion_BankBecomesEmpty) {
    bank.addQuestion(new TF(401, "Q1", 1, true));
    EXPECT_TRUE(bank.removeQuestion(401));
    EXPECT_EQ(bank.getCount(), 0);
    EXPECT_EQ(bank.getAt(0), nullptr);
}

TEST_F(QuestionBankTest, RemoveNonExisting_Fails_CountUnchanged) {
    bank.addQuestion(new TF(401, "Q1", 1, true));
    EXPECT_FALSE(bank.removeQuestion(999));
    EXPECT_EQ(bank.getCount(), 1);
}

TEST_F(QuestionBankTest, RemoveFromEmptyBank_Fails) {
    EXPECT_FALSE(bank.removeQuestion(1));
}

// ================================================================
// Search by prompt (TC-03)
// ================================================================

class QuestionBankSearchTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::string opts[4] = { "A", "B", "C", "D" };
        bank.addQuestion(new MCQ(301, "What is Object Oriented Programming?", 2, opts, 'A'));
        bank.addQuestion(new TF(302, "C++ supports inheritance", 1, true));
        bank.addQuestion(new TF(303, "Pointers can store addresses", 1, true));
    }
    QuestionBank bank;
};

TEST_F(QuestionBankSearchTest, SearchByPartialPrompt_FindsMatch) {
    std::string out = captureStdout([&] { bank.displayByPrompt("object oriented"); });
    EXPECT_NE(out.find("What is Object Oriented Programming?"), std::string::npos);
}

TEST_F(QuestionBankSearchTest, SearchIsCaseInsensitive_TC03) {
    std::string out = captureStdout([&] { bank.displayByPrompt("OBJECT ORIENTED"); });
    EXPECT_NE(out.find("What is Object Oriented Programming?"), std::string::npos);
}

TEST_F(QuestionBankSearchTest, SearchFindsMultipleMatches) {
    // "inheritance" matches only one, "supports" also one — test both exist
    std::string out = captureStdout([&] { bank.displayByPrompt("supports"); });
    EXPECT_NE(out.find("C++ supports inheritance"), std::string::npos);
}

TEST_F(QuestionBankSearchTest, SearchNoMatch_ReportsNotFound) {
    std::string out = captureStdout([&] { bank.displayByPrompt("does-not-exist"); });
    EXPECT_NE(out.find("No matching questions found."), std::string::npos);
}

// ================================================================
// Display all (FR-01)
// ================================================================

TEST_F(QuestionBankSearchTest, DisplayAll_ShowsAllQuestions) {
    std::string out = captureStdout([&] { bank.displayAll(); });
    EXPECT_NE(out.find("[MCQ]"), std::string::npos);
    EXPECT_NE(out.find("[TF]"), std::string::npos);
    EXPECT_NE(out.find("Object Oriented"), std::string::npos);
    EXPECT_NE(out.find("inheritance"), std::string::npos);
    EXPECT_NE(out.find("Pointers"), std::string::npos);
}

TEST_F(QuestionBankTest, DisplayAll_EmptyBank_ReportsEmpty) {
    std::string out = captureStdout([&] { bank.displayAll(); });
    EXPECT_NE(out.find("Question bank is empty."), std::string::npos);
}

// ================================================================
// Capacity limit (FR-01: max 100 questions)
// ================================================================

TEST_F(QuestionBankTest, AddExactly100Questions_AllSucceed) {
    for (int i = 0; i < 100; ++i) {
        EXPECT_TRUE(bank.addQuestion(
            new TF(500 + i, "Question " + std::to_string(i), 1, true)))
            << "Failed at question index " << i;
    }
    EXPECT_EQ(bank.getCount(), 100);
}

TEST_F(QuestionBankTest, Add101stQuestion_Rejected) {
    for (int i = 0; i < 100; ++i)
        bank.addQuestion(new TF(500 + i, "Q" + std::to_string(i), 1, true));

    TF* over = new TF(600, "Over capacity", 1, true);
    EXPECT_FALSE(bank.addQuestion(over));
    EXPECT_EQ(bank.getCount(), 100);
    delete over;
}

TEST_F(QuestionBankTest, At99_ValidAfterFull_AtIndex100_Invalid) {
    for (int i = 0; i < 100; ++i)
        bank.addQuestion(new TF(500 + i, "Q" + std::to_string(i), 1, true));

    EXPECT_NE(bank.getAt(99), nullptr);
    EXPECT_EQ(bank.getAt(100), nullptr);
}