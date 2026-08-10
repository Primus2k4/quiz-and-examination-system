#include <gtest/gtest.h>
#include "Question.h"
#include "MCQ.h"
#include "TF.h"

// ================================================================
// Fixture helper
// ================================================================
static MCQ makeMCQ() {
    std::string opts[4] = { "3", "4", "5", "6" };
    return MCQ(101, "2 + 2 = ?", 2, opts, 'B');
}

static TF makeTF_True() {
    return TF(102, "C++ supports inheritance", 1, true);
}

static TF makeTF_False() {
    return TF(103, "Java is C++", 1, false);
}

// ================================================================
// UT-BASE: Question base class – getter / setter (FR-01)
// ================================================================

TEST(UnitTest_QuestionBase, Getters_ReturnCorrectValues) {
    MCQ q = makeMCQ();
    EXPECT_EQ(q.getId(), 101);
    EXPECT_EQ(q.getPrompt(), "2 + 2 = ?");
    EXPECT_EQ(q.getPoints(), 2);
    EXPECT_EQ(q.getType(), "MCQ");
}

TEST(UnitTest_QuestionBase, SetPrompt_Valid_UpdatesPrompt) {
    MCQ q = makeMCQ();
    EXPECT_TRUE(q.setPrompt("3 + 3 = ?"));
    EXPECT_EQ(q.getPrompt(), "3 + 3 = ?");
}

TEST(UnitTest_QuestionBase, SetPrompt_Empty_Rejected) {
    MCQ q = makeMCQ();
    EXPECT_FALSE(q.setPrompt(""));
    EXPECT_EQ(q.getPrompt(), "2 + 2 = ?"); 
}

TEST(UnitTest_QuestionBase, SetPoints_Valid_UpdatesPoints) {
    MCQ q = makeMCQ();
    EXPECT_TRUE(q.setPoints(5));
    EXPECT_EQ(q.getPoints(), 5);
}

TEST(UnitTest_QuestionBase, SetPoints_Zero_Rejected) {
    MCQ q = makeMCQ();
    EXPECT_FALSE(q.setPoints(0));
    EXPECT_EQ(q.getPoints(), 2); 
}

TEST(UnitTest_QuestionBase, SetPoints_Negative_Rejected) {
    MCQ q = makeMCQ();
    EXPECT_FALSE(q.setPoints(-1));
    EXPECT_EQ(q.getPoints(), 2); 
}

// ================================================================
// UT-MCQ: MCQ::checkAnswer – TC-09
// ================================================================

// TC-09: 
TEST(UnitTest_MCQ, CheckAnswer_UpperCase_ReturnsTrue_TC09) {
    MCQ q = makeMCQ(); 
    EXPECT_TRUE(q.checkAnswer("B"));
}

TEST(UnitTest_MCQ, CheckAnswer_LowerCase_ReturnsTrue_TC09) {
    MCQ q = makeMCQ();
    EXPECT_TRUE(q.checkAnswer("b"));
}

TEST(UnitTest_MCQ, CheckAnswer_WrongOption_ReturnsFalse) {
    MCQ q = makeMCQ();
    EXPECT_FALSE(q.checkAnswer("A"));
    EXPECT_FALSE(q.checkAnswer("C"));
    EXPECT_FALSE(q.checkAnswer("D"));
}

// FR-04: 
TEST(UnitTest_MCQ, CheckAnswer_InvalidInput_ReturnsFalse) {
    MCQ q = makeMCQ();
    EXPECT_FALSE(q.checkAnswer(""));    
    EXPECT_FALSE(q.checkAnswer("AB"));  
    EXPECT_FALSE(q.checkAnswer("1"));   
    EXPECT_FALSE(q.checkAnswer(" B"));  
}

//
TEST(UnitTest_MCQ, Init_LowercaseCorrectOption_NormalizedToUpper) {
    std::string opts[4] = { "x", "y", "z", "w" };
    MCQ q(200, "test", 1, opts, 'b');
    EXPECT_EQ(q.getCorrectOption(), 'B');
}

// ================================================================
// UT-MCQ: getter / setter MCQ (FR-01)
// ================================================================

TEST(UnitTest_MCQ, GetOption_ReturnsCorrectText) {
    MCQ q = makeMCQ();
    EXPECT_EQ(q.getOption(0), "3");
    EXPECT_EQ(q.getOption(1), "4");
    EXPECT_EQ(q.getOption(2), "5");
    EXPECT_EQ(q.getOption(3), "6");
}

TEST(UnitTest_MCQ, SetOption_Valid_UpdatesText) {
    MCQ q = makeMCQ();
    EXPECT_TRUE(q.setOption(0, "seven"));
    EXPECT_EQ(q.getOption(0), "seven");
}

TEST(UnitTest_MCQ, SetOption_Empty_Rejected) {
    MCQ q = makeMCQ();
    EXPECT_FALSE(q.setOption(0, ""));
    EXPECT_EQ(q.getOption(0), "3"); 
}

TEST(UnitTest_MCQ, SetOption_OutOfRange_Rejected) {
    MCQ q = makeMCQ();
    EXPECT_FALSE(q.setOption(-1, "x"));
    EXPECT_FALSE(q.setOption(4, "x"));
}

TEST(UnitTest_MCQ, SetCorrectOption_Valid_NormalizedToUpper) {
    MCQ q = makeMCQ();
    EXPECT_TRUE(q.setCorrectOption('c'));
    EXPECT_EQ(q.getCorrectOption(), 'C');
}

TEST(UnitTest_MCQ, SetCorrectOption_Invalid_Rejected) {
    MCQ q = makeMCQ();
    EXPECT_FALSE(q.setCorrectOption('E'));
    EXPECT_FALSE(q.setCorrectOption('1'));
    EXPECT_EQ(q.getCorrectOption(), 'B'); 
}

// ================================================================
// UT-TF: TF::checkAnswer – TC-10
// ================================================================

// TC-10: 
TEST(UnitTest_TF, CheckAnswer_CorrectIsTrue_TrueInputReturnsTrue_TC10) {
    TF q = makeTF_True();
    EXPECT_TRUE(q.checkAnswer("true"));
    EXPECT_TRUE(q.checkAnswer("True"));
    EXPECT_TRUE(q.checkAnswer("TRUE"));
}

TEST(UnitTest_TF, CheckAnswer_CorrectIsTrue_FalseInputReturnsFalse_TC10) {
    TF q = makeTF_True();
    EXPECT_FALSE(q.checkAnswer("false"));
    EXPECT_FALSE(q.checkAnswer("False"));
    EXPECT_FALSE(q.checkAnswer("FALSE"));
}

// 
TEST(UnitTest_TF, CheckAnswer_CorrectIsFalse_FalseInputReturnsTrue_TC10) {
    TF q = makeTF_False();
    EXPECT_TRUE(q.checkAnswer("false"));
    EXPECT_TRUE(q.checkAnswer("False"));
    EXPECT_TRUE(q.checkAnswer("FALSE"));
}

TEST(UnitTest_TF, CheckAnswer_CorrectIsFalse_TrueInputReturnsFalse_TC10) {
    TF q = makeTF_False();
    EXPECT_FALSE(q.checkAnswer("true"));
    EXPECT_FALSE(q.checkAnswer("True"));
}

// FR-04: 
TEST(UnitTest_TF, CheckAnswer_InvalidInput_ReturnsFalse) {
    TF q = makeTF_True();
    EXPECT_FALSE(q.checkAnswer(""));     
    EXPECT_FALSE(q.checkAnswer("yes"));  
    EXPECT_FALSE(q.checkAnswer("1"));    
    EXPECT_FALSE(q.checkAnswer("T"));    
}

// ================================================================
// UT-TF: getter / setter (FR-01)
// ================================================================

TEST(UnitTest_TF, GetType_ReturnsTF) {
    TF q = makeTF_True();
    EXPECT_EQ(q.getType(), "TF");
}

TEST(UnitTest_TF, SetCorrectAnswer_UpdatesValue) {
    TF q = makeTF_True();
    q.setCorrectAnswer(false);
    EXPECT_FALSE(q.getCorrectAnswer());
    EXPECT_TRUE(q.checkAnswer("false"));
    EXPECT_FALSE(q.checkAnswer("true"));
}

// ================================================================
// UT-POLY: Runtime polymorphism qua Question* – TC-11
// ================================================================

// TC-11:
TEST(UnitTest_Polymorphism, ScoreCalculation_ViaPtrNoIfSwitch_TC11) {
    std::string opts[4] = { "3", "4", "5", "6" };
    Question* q1 = new MCQ(101, "2 + 2 = ?", 2, opts, 'B');
    Question* q2 = new TF(102, "C++ supports inheritance", 1, true);

    int score = 0;
    score += q1->checkAnswer("B") ? q1->getPoints() : 0; 
    score += q2->checkAnswer("false") ? q2->getPoints() : 0; 

    EXPECT_EQ(score, 2);

    delete q1;
    delete q2;
}

// display()
TEST(UnitTest_Polymorphism, Display_DoesNotCrash) {
    std::string opts[4] = { "3", "4", "5", "6" };
    Question* q1 = new MCQ(101, "2 + 2 = ?", 2, opts, 'B');
    Question* q2 = new TF(102, "C++ supports inheritance", 1, true);

    EXPECT_NO_THROW(q1->display());
    EXPECT_NO_THROW(q2->display());

    delete q1;
    delete q2;
}

// Virtual destructor – delete through base pointer 
TEST(UnitTest_Polymorphism, VirtualDestructor_NoMemoryLeak) {
    std::string opts[4] = { "a", "b", "c", "d" };
    Question* q = new MCQ(999, "test", 1, opts, 'A');
    // If destructor not virtual
    EXPECT_NO_THROW(delete q);
}