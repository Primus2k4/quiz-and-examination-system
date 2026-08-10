#include "QuizEngine.h"

using namespace std;

// Constructor

QuizEngine::QuizEngine()
{
    currentQuiz = nullptr;
    questionBank = nullptr;

    currentIndex = 0;
    score = 0;

    state = NOT_STARTED;

    for (int i = 0; i < 30; i++)
    {
        answers[i] = "";
    }
}

// Start Quiz

bool QuizEngine::start(Quiz* quiz, QuestionBank* bank)
{
    // Kiểm tra dữ liệu đầu vào
    if (quiz == nullptr || bank == nullptr)
        return false;
        
    // Không cho start lại khi đang có phiên làm bài dở dang
    if (state == IN_PROGRESS)
        return false;

    // Không thể bắt đầu nếu quiz không có câu hỏi
    if (quiz->getQuestionCount() == 0)
        return false;

    // Lưu quiz và question bank
    currentQuiz = quiz;
    questionBank = bank;

    // Khởi tạo lại trạng thái
    currentIndex = 0;
    score = 0;

    // Xóa toàn bộ đáp án cũ
    for (int i = 0; i < 30; i++)
    {
        answers[i] = "";
    }

    // Chuyển sang trạng thái đang làm bài
    state = IN_PROGRESS;

    return true;
}

// Display Current Question

void QuizEngine::displayCurrentQuestion() const
{
    // Chỉ hiển thị khi đang làm bài
    if (state != IN_PROGRESS)
    {
        cout << "Quiz is not in progress." << endl;
        return;
    }

    // Lấy ID câu hỏi hiện tại
    int questionId = currentQuiz->getQuestionId(currentIndex);

    // Tìm câu hỏi trong QuestionBank
    Question* q = questionBank->findById(questionId);

    if (q == nullptr)
    {
        cout << "Question not found." << endl;
        return;
    }

    cout << "==========================================" << endl;
    cout << "Question "
         << currentIndex + 1
         << " / "
         << currentQuiz->getQuestionCount()
         << endl;

    // Runtime Polymorphism
    // MCQ sẽ gọi MCQ::display()
    // TF sẽ gọi TF::display()
    q->display();

    cout << endl;

    // Hiển thị đáp án hiện tại (nếu đã trả lời)
    if (answers[currentIndex] == "")
    {
        cout << "Current Answer : (None)" << endl;
    }
    else
    {
        cout << "Current Answer : "
             << answers[currentIndex]
             << endl;
    }

    cout << "==========================================" << endl;
}

// Answer Question

bool QuizEngine::answer(const string& ans)
{
    // Chỉ được trả lời khi đang làm bài
    if (state != IN_PROGRESS)
        return false;

    // Kiểm tra Quiz hợp lệ
    if (currentQuiz == nullptr)
        return false;

    // Lấy câu hỏi hiện tại
    int questionId = currentQuiz->getQuestionId(currentIndex);

    Question* q = questionBank->findById(questionId);

    if (q == nullptr)
        return false;

    // Không cho phép nhập rỗng
    if (ans.empty())
        return false;

    // Lưu đáp án
    answers[currentIndex] = ans;

    return true;
}

// Next Question

bool QuizEngine::next()
{
    // Chỉ được chuyển khi đang làm bài
    if (state != IN_PROGRESS)
        return false;

    // Đã ở câu cuối
    if (currentIndex >= currentQuiz->getQuestionCount() - 1)
    {
        cout << "Already at the last question." << endl;
        return false;
    }

    currentIndex++;

    return true;
}

// Previous Question

bool QuizEngine::previous()
{
    // Chỉ được chuyển khi đang làm bài
    if (state != IN_PROGRESS)
        return false;

    // Đã ở câu đầu
    if (currentIndex <= 0)
    {
        cout << "Already at the first question." << endl;
        return false;
    }

    currentIndex--;

    return true;
}

// Submit Quiz

bool QuizEngine::submit()
{
    // Chỉ được submit khi đang làm bài
    if (state != IN_PROGRESS)
        return false;

    // Tính điểm
    score = calculateScore();

    // Chuyển trạng thái
    state = SUBMITTED;

    cout << "\n==========================================\n";
    cout << "Quiz submitted successfully!" << endl;
    cout << "Your Score: " << score << endl;
    cout << "==========================================\n";

    return true;
}

// Calculate Score

int QuizEngine::calculateScore()
{
    int totalScore = 0;

    if (currentQuiz == nullptr || questionBank == nullptr)
        return 0;

    for (int i = 0; i < currentQuiz->getQuestionCount(); i++)
    {
        int questionId = currentQuiz->getQuestionId(i);

        Question* q = questionBank->findById(questionId);

        if (q == nullptr)
            continue;

        // Runtime Polymorphism
        if (q->checkAnswer(answers[i]))
        {
            totalScore += q->getPoints();
        }
    }

    return totalScore;
}

// Show Result

void QuizEngine::showResult() const
{
    if (state != SUBMITTED)
    {
        cout << "Quiz has not been submitted." << endl;
        return;
    }

    cout << "\n==========================================\n";
    cout << "               QUIZ RESULT                 " << endl;
    cout << "==========================================\n";

    int totalPoints = 0;

    for (int i = 0; i < currentQuiz->getQuestionCount(); i++)
    {
        int questionId = currentQuiz->getQuestionId(i);

        Question* q = questionBank->findById(questionId);

        if (q == nullptr)
            continue;

        totalPoints += q->getPoints();

        cout << "Question " << i + 1 << " : ";

        if (q->checkAnswer(answers[i]))
        {
            cout << "Correct";
        }
        else
        {
            cout << "Wrong";
        }

        cout << endl;

        cout << "Your Answer : ";

        if (answers[i] == "")
            cout << "(No Answer)";
        else
            cout << answers[i];

        cout << endl;

        cout << "Points : ";

        if (q->checkAnswer(answers[i]))
            cout << q->getPoints();
        else
            cout << 0;

        cout << "\n------------------------------------------\n";
    }

    cout << "Final Score : "
         << score
         << " / "
         << totalPoints
         << endl;

    cout << "==========================================\n";
}

// Getter

AttemptState QuizEngine::getState() const
{
    return state;
}

int QuizEngine::getCurrentIndex() const
{
    return currentIndex;
}

int QuizEngine::getScore() const
{
    return score;
}

string QuizEngine::getCurrentAnswer() const
{
    if (currentQuiz == nullptr)
        return "";

    return answers[currentIndex];
}
