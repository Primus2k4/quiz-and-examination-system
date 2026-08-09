#include "test_QB.h"

#include "../include/QuestionBank.h"
#include "../include/MCQ.h"
#include "../include/TF.h"

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

static int passed = 0;
static int failed = 0;

static void test(const string& name, bool result)
{
    if (result) {
        cout << "[PASS] " << name << '\n';
        ++passed;
    } else {
        cout << "[FAIL] " << name << '\n';
        ++failed;
    }
}

static MCQ* makeMCQ(int id, const string& prompt = "2 + 2 = ?", int points = 2, char correct = 'B')
{
    string options[4] = { "3", "4", "5", "6" };
    return new MCQ(id, prompt, points, options, correct);
}

static bool contains(const string& text, const string& part)
{
    return text.find(part) != string::npos;
}

static void testQuestionBaseAndMCQ()
{
    cout << "\n========== QUESTION / MCQ TESTS ==========" << '\n';

    string mcqOptions[4] = { "3", "4", "5", "6" };
    MCQ mcq(1, "2 + 2 = ?", 2, mcqOptions, 'b');

    test("MCQ keeps ID", mcq.getId() == 1);
    test("MCQ keeps prompt", mcq.getPrompt() == "2 + 2 = ?");
    test("MCQ keeps points", mcq.getPoints() == 2);
    test("MCQ getType()", mcq.getType() == "MCQ");
    test("MCQ isValid()", mcq.isValid());
    test("MCQ correct option normalized to uppercase", mcq.getCorrectOption() == 'B');

    Question* base = &mcq;
    test("MCQ works through Question pointer", base->getType() == "MCQ");
    test("Runtime polymorphism: MCQ correct answer", base->checkAnswer("B"));
    test("Runtime polymorphism: MCQ lowercase answer", base->checkAnswer("b"));
    test("MCQ wrong answer", !base->checkAnswer("A"));
    test("MCQ rejects empty answer", !base->checkAnswer(""));
    test("MCQ rejects multi-character answer", !base->checkAnswer("BB"));

    test("MCQ getOption(0)", mcq.getOption(0) == "3");
    test("MCQ getOption(3)", mcq.getOption(3) == "6");
    test("MCQ rejects getOption(-1)", mcq.getOption(-1).empty());
    test("MCQ rejects getOption(4)", mcq.getOption(4).empty());

    test("MCQ setOption valid", mcq.setOption(0, "2"));
    test("MCQ option actually changed", mcq.getOption(0) == "2");
    test("MCQ rejects empty option", !mcq.setOption(0, ""));
    test("MCQ rejects negative option index", !mcq.setOption(-1, "X"));
    test("MCQ rejects option index 4", !mcq.setOption(4, "X"));

    test("MCQ setCorrectOption lowercase", mcq.setCorrectOption('d') && mcq.getCorrectOption() == 'D');
    test("MCQ rejects invalid correct option", !mcq.setCorrectOption('E'));
    test("MCQ correct answer updated", mcq.checkAnswer("d"));

    test("Question setPrompt valid", mcq.setPrompt("Updated prompt") && mcq.getPrompt() == "Updated prompt");
    test("Question rejects empty prompt", !mcq.setPrompt(""));
    test("Question setPoints valid", mcq.setPoints(10) && mcq.getPoints() == 10);
    test("Question rejects zero points", !mcq.setPoints(0));
    test("Question rejects negative points", !mcq.setPoints(-5));

    string invalidOptions[4] = { "A", "", "C", "D" };
    MCQ invalid(2, "Prompt", 1, invalidOptions, 'A');
    test("MCQ invalid when an option is empty", !invalid.isValid());

    string validOptions[4] = { "A", "B", "C", "D" };
    MCQ invalidCorrect(3, "Prompt", 1, validOptions, 'E');
    test("MCQ invalid when correct option is outside A-D", !invalidCorrect.isValid());

    string options2[4] = { "A", "B", "C", "D" };
    MCQ invalidId(0, "Prompt", 1, options2, 'A');
    test("MCQ invalid when ID is not positive", !invalidId.isValid());

    MCQ invalidPoints(4, "Prompt", 0, options2, 'A');
    test("MCQ invalid when points are not positive", !invalidPoints.isValid());

    MCQ invalidPrompt(5, "", 1, options2, 'A');
    test("MCQ invalid when prompt is empty", !invalidPrompt.isValid());
}

static void testTF()
{
    cout << "\n========== TF TESTS ==========" << '\n';

    TF tf(10, "C++ supports inheritance", 1, true);
    Question* base = &tf;

    test("TF keeps ID", tf.getId() == 10);
    test("TF getType()", tf.getType() == "TF");
    test("TF isValid()", tf.isValid());
    test("TF correct answer getter", tf.getCorrectAnswer());
    test("TF true is correct", base->checkAnswer("true"));
    test("TF TRUE is correct", base->checkAnswer("TRUE"));
    test("TF True is correct", base->checkAnswer("True"));
    test("TF false is wrong", !base->checkAnswer("false"));
    test("TF rejects invalid answer", !base->checkAnswer("yes"));
    test("TF rejects empty answer", !base->checkAnswer(""));

    tf.setCorrectAnswer(false);
    test("TF setter changes answer", !tf.getCorrectAnswer());
    test("TF false becomes correct", tf.checkAnswer("false"));
    test("TF true becomes wrong", !tf.checkAnswer("true"));

    TF invalidId(0, "Prompt", 1, true);
    test("TF invalid when ID is not positive", !invalidId.isValid());

    TF invalidPrompt(11, "", 1, true);
    test("TF invalid when prompt is empty", !invalidPrompt.isValid());

    TF invalidPoints(12, "Prompt", -1, true);
    test("TF invalid when points are not positive", !invalidPoints.isValid());
}

static void testQuestionBankBasic()
{
    cout << "\n========== QUESTION BANK BASIC TESTS ==========" << '\n';

    QuestionBank bank;
    string options[4] = { "A", "B", "C", "D" };

    test("New QuestionBank starts empty", bank.getCount() == 0);
    test("getAt(-1) returns nullptr", bank.getAt(-1) == nullptr);
    test("getAt(0) on empty bank returns nullptr", bank.getAt(0) == nullptr);
    test("findById on empty bank returns nullptr", bank.findById(1) == nullptr);
    test("Reject nullptr", !bank.addQuestion(nullptr));

    test("Add valid MCQ", bank.addQuestion(new MCQ(101, "First MCQ", 2, options, 'A')));
    test("Add valid TF", bank.addQuestion(new TF(102, "First TF", 1, true)));
    test("Count after two valid additions", bank.getCount() == 2);

    Question* q101 = bank.findById(101);
    Question* q102 = bank.findById(102);
    test("findById returns MCQ", q101 != nullptr && q101->getType() == "MCQ");
    test("findById returns TF", q102 != nullptr && q102->getType() == "TF");
    test("getAt(0) returns first question", bank.getAt(0) == q101);
    test("getAt(1) returns second question", bank.getAt(1) == q102);
    test("getAt(2) out of range returns nullptr", bank.getAt(2) == nullptr);

    MCQ* duplicate = new MCQ(101, "Duplicate", 2, options, 'B');
    test("Reject duplicate ID", !bank.addQuestion(duplicate) && bank.getCount() == 2);
    delete duplicate;

    MCQ* badPrompt = new MCQ(103, "", 2, options, 'A');
    test("Reject empty prompt", !bank.addQuestion(badPrompt) && bank.getCount() == 2);
    delete badPrompt;

    MCQ* badPoints = new MCQ(103, "Bad points", 0, options, 'A');
    test("Reject zero points", !bank.addQuestion(badPoints) && bank.getCount() == 2);
    delete badPoints;

    string badOptions[4] = { "A", "", "C", "D" };
    MCQ* badOption = new MCQ(103, "Bad option", 2, badOptions, 'A');
    test("Reject empty MCQ option", !bank.addQuestion(badOption) && bank.getCount() == 2);
    delete badOption;

    MCQ* badCorrect = new MCQ(103, "Bad correct", 2, options, 'Z');
    test("Reject invalid MCQ correct option", !bank.addQuestion(badCorrect) && bank.getCount() == 2);
    delete badCorrect;

    test("findById non-existing returns nullptr", bank.findById(999) == nullptr);
    test("remove non-existing ID fails", !bank.removeQuestion(999) && bank.getCount() == 2);
}

static void testQuestionBankUpdate()
{
    cout << "\n========== QUESTION BANK UPDATE TESTS ==========" << '\n';

    QuestionBank bank;
    string options[4] = { "A", "B", "C", "D" };
    bank.addQuestion(new MCQ(201, "Original", 2, options, 'A'));
    bank.addQuestion(new TF(202, "TF Original", 1, true));

    MCQ* updated = new MCQ(201, "Updated", 5, options, 'C');
    test("Update same ID and same type succeeds", bank.updateQuestion(201, updated));
    Question* q = bank.findById(201);
    test("Updated prompt is stored", q && q->getPrompt() == "Updated");
    test("Updated points are stored", q && q->getPoints() == 5);
    test("Updated MCQ answer is stored", q && q->checkAnswer("c"));

    MCQ* wrongId = new MCQ(999, "Wrong ID", 2, options, 'A');
    test("Reject update with different ID", !bank.updateQuestion(201, wrongId));
    delete wrongId;
    test("Original question remains after rejected ID update",
         bank.findById(201) && bank.findById(201)->getPrompt() == "Updated");

    TF* wrongType = new TF(201, "Wrong type", 2, true);
    test("Reject update with different type", !bank.updateQuestion(201, wrongType));
    delete wrongType;
    test("Original type remains after rejected type update",
         bank.findById(201) && bank.findById(201)->getType() == "MCQ");

    MCQ* badUpdate = new MCQ(201, "", 2, options, 'A');
    test("Reject invalid replacement question", !bank.updateQuestion(201, badUpdate));
    delete badUpdate;

    MCQ* missing = makeMCQ(999);
    test("Update non-existing ID fails", !bank.updateQuestion(999, missing));
    delete missing;
}

static void testQuestionBankSearchAndDisplay()
{
    cout << "\n========== SEARCH / DISPLAY TESTS ==========" << '\n';

    QuestionBank bank;
    string options[4] = { "A", "B", "C", "D" };
    bank.addQuestion(new MCQ(301, "What is Object Oriented Programming?", 2, options, 'A'));
    bank.addQuestion(new TF(302, "C++ supports inheritance", 1, true));
    bank.addQuestion(new TF(303, "Pointers can store addresses", 1, true));

    ostringstream out;
    streambuf* old = cout.rdbuf(out.rdbuf());
    bank.displayByPrompt("object oriented");
    cout.rdbuf(old);
    string result = out.str();
    test("Search finds partial prompt", contains(result, "What is Object Oriented Programming?"));

    out.str("");
    out.clear();
    old = cout.rdbuf(out.rdbuf());
    bank.displayByPrompt("OBJECT ORIENTED");
    cout.rdbuf(old);
    result = out.str();
    test("Search is case-insensitive", contains(result, "What is Object Oriented Programming?"));

    out.str("");
    out.clear();
    old = cout.rdbuf(out.rdbuf());
    bank.displayByPrompt("inheritance");
    cout.rdbuf(old);
    result = out.str();
    test("Search can find TF prompt", contains(result, "C++ supports inheritance"));

    out.str("");
    out.clear();
    old = cout.rdbuf(out.rdbuf());
    bank.displayByPrompt("does-not-exist");
    cout.rdbuf(old);
    result = out.str();
    test("Search reports no result", contains(result, "No matching questions found."));

    out.str("");
    out.clear();
    old = cout.rdbuf(out.rdbuf());
    bank.displayAll();
    cout.rdbuf(old);
    result = out.str();
    test("displayAll displays MCQ", contains(result, "[MCQ]"));
    test("displayAll displays TF", contains(result, "[TF]"));
    test("displayAll displays all prompts", contains(result, "Object Oriented") &&
         contains(result, "inheritance") && contains(result, "Pointers"));

    QuestionBank empty;
    out.str("");
    out.clear();
    old = cout.rdbuf(out.rdbuf());
    empty.displayAll();
    cout.rdbuf(old);
    test("displayAll handles empty bank", contains(out.str(), "Question bank is empty."));
}

static void testRemovalAndOrder()
{
    cout << "\n========== REMOVE / ORDER TESTS ==========" << '\n';

    QuestionBank bank;
    string options[4] = { "A", "B", "C", "D" };
    bank.addQuestion(new MCQ(401, "Q1", 1, options, 'A'));
    bank.addQuestion(new TF(402, "Q2", 1, true));
    bank.addQuestion(new MCQ(403, "Q3", 1, options, 'C'));

    test("Remove middle question", bank.removeQuestion(402));
    test("Count decreases after removal", bank.getCount() == 2);
    test("Removed question no longer found", bank.findById(402) == nullptr);
    test("First question remains", bank.getAt(0) && bank.getAt(0)->getId() == 401);
    test("Remaining questions shift left", bank.getAt(1) && bank.getAt(1)->getId() == 403);
    test("Out-of-range index after removal returns nullptr", bank.getAt(2) == nullptr);

    test("Remove first question", bank.removeQuestion(401));
    test("Remaining question becomes first", bank.getAt(0) && bank.getAt(0)->getId() == 403);

    test("Remove last question", bank.removeQuestion(403));
    test("Bank becomes empty", bank.getCount() == 0 && bank.getAt(0) == nullptr);
    test("Removing from empty bank fails", !bank.removeQuestion(403));
}

static void testCapacity()
{
    cout << "\n========== CAPACITY TESTS ==========" << '\n';

    QuestionBank bank;
    for (int i = 0; i < 100; ++i) {
        bool ok = bank.addQuestion(new TF(500 + i, "Question " + to_string(i), 1, true));
        if (!ok) {
            test("Add question within 100-question capacity", false);
            return;
        }
    }

    test("QuestionBank accepts exactly 100 questions", bank.getCount() == 100);
    test("First capacity question exists", bank.findById(500) != nullptr);
    test("Last capacity question exists", bank.findById(599) != nullptr);

    TF* overCapacity = new TF(600, "Over capacity", 1, true);
    bool result = bank.addQuestion(overCapacity);
    test("Reject 101st question", !result && bank.getCount() == 100);
    if (!result) delete overCapacity;

    test("Question at index 99 exists", bank.getAt(99) != nullptr && bank.getAt(99)->getId() == 599);
    test("Index 100 is invalid", bank.getAt(100) == nullptr);
}

static void runAllTests()
{
    passed = 0;
    failed = 0;

    testQuestionBaseAndMCQ();
    testTF();
    testQuestionBankBasic();
    testQuestionBankUpdate();
    testQuestionBankSearchAndDisplay();
    testRemovalAndOrder();
    testCapacity();

    cout << "\n==========================================\n";
    cout << "TOTAL PASSED: " << passed << '\n';
    cout << "TOTAL FAILED: " << failed << '\n';
    cout << "TOTAL TESTS : " << passed + failed << '\n';
    cout << "==========================================\n";
}

void runQuestionBankTests()
{
    runAllTests();
}

