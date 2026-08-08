// Build (from project root):
//   g++ -std=c++11 -I include src/Question.cpp src/MCQ.cpp src/TF.cpp tests/test_question.cpp -o tests/test_question
// Dynamic analysis tool:
// valgrind --leak-check=full ./test_question
// Static analysis tool:
// cppcheck --enable=all --inconclusive include/ src/
// Run:
//   ./tests/test_question  

#include "../include/Question.h"
#include "../include/MCQ.h"
#include "../include/TF.h"

#include <iostream>
#include <string>

// ---------------------------------------------------------------
// Mini test runner – no framework used, just simple macros.
// ---------------------------------------------------------------
static int g_pass = 0;
static int g_fail = 0;

#define CHECK(desc, expr)                                           \
    do {                                                            \
        if (expr) {                                                 \
            std::cout << "  [PASS] " << (desc) << "\n";            \
            ++g_pass;                                               \
        } else {                                                    \
            std::cout << "  [FAIL] " << (desc) << "\n";            \
            ++g_fail;                                               \
        }                                                           \
    } while (0)

// ---------------------------------------------------------------
// Helper: create dummy MCQ for reuse in multiple tests.
// Question: "2 + 2 = ?" - correct answer is B (4).
// ---------------------------------------------------------------
static MCQ makeSampleMCQ() {
    std::string opts[4] = { "3", "4", "5", "6" };
    return MCQ(101, "2 + 2 = ?", 2, opts, 'B');
}

// Helper: create dummy TF.
// Question: "C++ supports inheritance" - correct answer is true.
static TF makeSampleTF() {
    return TF(102, "C++ supports inheritance", 1, true);
}

// ---------------------------------------------------------------
// Test Question base (getter/setter) qua MCQ instance.
// ---------------------------------------------------------------
void test_question_base() {
    std::cout << "\n=== Question base (getter/setter) ===\n";
    MCQ q = makeSampleMCQ();

    CHECK("getId() returns correctly", q.getId() == 101);
    CHECK("getPrompt() returns correctly", q.getPrompt() == "2 + 2 = ?");
    CHECK("getPoints() returns correctly", q.getPoints() == 2);
    CHECK("getType() returns MCQ", q.getType() == "MCQ");

    // Valid setPrompt
    CHECK("Valid setPrompt returns true", q.setPrompt("3 + 3 = ?") == true);
    CHECK("getPrompt() updates correctly", q.getPrompt() == "3 + 3 = ?");

    // Empty setPrompt must be rejected
    CHECK("Empty setPrompt returns false", q.setPrompt("") == false);
    CHECK("prompt remains unchanged after rejection", q.getPrompt() == "3 + 3 = ?");

    // Valid setPoints
    CHECK("setPoints(5) returns true", q.setPoints(5) == true);
    CHECK("getPoints() updates correctly", q.getPoints() == 5);

    // Invalid setPoints
    CHECK("setPoints(0) returns false", q.setPoints(0) == false);
    CHECK("setPoints(-1) returns false", q.setPoints(-1) == false);
    CHECK("points remain unchanged after rejection", q.getPoints() == 5);
}

// ---------------------------------------------------------------
// Test MCQ::checkAnswer – TC-09
// ---------------------------------------------------------------
void test_mcq_checkAnswer() {
    std::cout << "\n=== MCQ::checkAnswer (TC-09) ===\n";
    MCQ q = makeSampleMCQ(); // correct answer: B

    CHECK("Input 'B' (uppercase) -> correct", q.checkAnswer("B") == true);
    CHECK("Input 'b' (lowercase) -> correct", q.checkAnswer("b") == true);
    CHECK("Input 'A' -> incorrect", q.checkAnswer("A") == false);
    CHECK("Input 'C' -> incorrect", q.checkAnswer("C") == false);
    CHECK("Input 'D' -> incorrect", q.checkAnswer("D") == false);
    CHECK("Input long string 'AB' -> incorrect", q.checkAnswer("AB") == false);
    CHECK("Input empty -> incorrect", q.checkAnswer("") == false);
}

// ---------------------------------------------------------------
// Test MCQ getter/setter riêng
// ---------------------------------------------------------------
void test_mcq_fields() {
    std::cout << "\n=== MCQ getter/setter ===\n";
    MCQ q = makeSampleMCQ(); // options: 3,4,5,6 | correct: B

    CHECK("getOption(0) == '3'", q.getOption(0) == "3");
    CHECK("getOption(1) == '4'", q.getOption(1) == "4");
    CHECK("getCorrectOption() == 'B'", q.getCorrectOption() == 'B');

    // correctOption is normalized to uppercase when initialized with 'b'
    std::string opts[4] = { "x","y","z","w" };
    MCQ q2(200, "test", 1, opts, 'b'); // pass in lowercase
    CHECK("correctOption is normalized to 'B' when init with 'b'",
        q2.getCorrectOption() == 'B');

    // Valid setCorrectOption
    CHECK("setCorrectOption('C') -> true", q.setCorrectOption('C') == true);
    CHECK("getCorrectOption() updates to 'C'", q.getCorrectOption() == 'C');
    CHECK("setCorrectOption('d') -> true", q.setCorrectOption('d') == true);
    CHECK("getCorrectOption() normalizes to 'D'", q.getCorrectOption() == 'D');

    // Invalid setCorrectOption
    CHECK("setCorrectOption('E') -> false", q.setCorrectOption('E') == false);
    CHECK("setCorrectOption('1') -> false", q.setCorrectOption('1') == false);

    // Valid setOption
    CHECK("setOption(0, 'new') -> true", q.setOption(0, "new") == true);
    CHECK("getOption(0) updates to 'new'", q.getOption(0) == "new");

    // Empty / out of bounds setOption
    CHECK("setOption(0, '') -> false", q.setOption(0, "") == false);
    CHECK("setOption(-1, 'x') -> false", q.setOption(-1, "x") == false);
    CHECK("setOption(4, 'x') -> false", q.setOption(4, "x") == false);
}

// ---------------------------------------------------------------
// Test TF::checkAnswer – TC-10
// ---------------------------------------------------------------
void test_tf_checkAnswer() {
    std::cout << "\n=== TF::checkAnswer (TC-10) ===\n";
    TF q = makeSampleTF(); // correct answer: true

    CHECK("Input 'true' -> correct", q.checkAnswer("true") == true);
    CHECK("Input 'True' -> correct", q.checkAnswer("True") == true);
    CHECK("Input 'TRUE' -> correct", q.checkAnswer("TRUE") == true);
    CHECK("Input 'false' -> incorrect", q.checkAnswer("false") == false);
    CHECK("Input 'False' -> incorrect", q.checkAnswer("False") == false);
    CHECK("Input unknown string -> incorrect", q.checkAnswer("yes") == false);
    CHECK("Input empty -> incorrect", q.checkAnswer("") == false);

    TF q2(103, "Java is C++", 1, false);
    CHECK("TF(false): input 'false' -> correct", q2.checkAnswer("false") == true);
    CHECK("TF(false): input 'true'  -> incorrect", q2.checkAnswer("true") == false);
}


void test_tf_fields() {
    std::cout << "\n=== TF getter/setter ===\n";
    TF q = makeSampleTF();

    CHECK("getType() == 'TF'", q.getType() == "TF");
    CHECK("getCorrectAnswer() == true", q.getCorrectAnswer() == true);

    q.setCorrectAnswer(false);
    CHECK("setCorrectAnswer(false) updates correctly", q.getCorrectAnswer() == false);
}

void test_polymorphism() {
    std::cout << "\n=== Runtime polymorphism via Question* (TC-11) ===\n";

    std::string opts[4] = { "3","4","5","6" };
    Question* q1 = new MCQ(101, "2 + 2 = ?", 2, opts, 'B');
    Question* q2 = new TF(102, "C++ supports inheritance", 1, true);

    std::cout << "  -- display MCQ --\n";
    q1->display();
    std::cout << "  -- display TF --\n";
    q2->display();

    int totalScore = 0;
    totalScore += q1->checkAnswer("B") ? q1->getPoints() : 0; // +2
    totalScore += q2->checkAnswer("false") ? q2->getPoints() : 0; // +0

    CHECK("Total score of correct MCQ + incorrect TF = 2/3", totalScore == 2);

    delete q1;
    delete q2;
}

void test_display() {
    std::cout << "\n=== display() visual check ===\n";
    MCQ mcq = makeSampleMCQ();
    TF  tf = makeSampleTF();

    std::cout << "  -- MCQ display --\n";
    mcq.display();
    std::cout << "  -- TF display --\n";
    tf.display();

    CHECK("MCQ::display() does not crash", true);
    CHECK("TF::display() does not crash", true);
}

// ---------------------------------------------------------------
// main
// ---------------------------------------------------------------
int main() {
    std::cout << "========================================\n";
    std::cout << "  Test: Question / MCQ / TF\n";
    std::cout << "========================================\n";

    test_question_base();
    test_mcq_checkAnswer();
    test_mcq_fields();
    test_tf_checkAnswer();
    test_tf_fields();
    test_polymorphism();
    test_display();

    std::cout << "\n========================================\n";
    std::cout << "  Result: "
        << g_pass << " PASS  |  "
        << g_fail << " FAIL\n";
    std::cout << "========================================\n";

    return g_fail == 0 ? 0 : 1; 
}