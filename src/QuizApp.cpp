#include "QuizApp.h"
#include "DataFileManager.h"
#include "InputValidator.h"
#include "MCQ.h"
#include "TF.h"
#include <iostream>
using namespace std;

const string QUESTIONS_FILE = "data/questions.txt";
const string QUIZZES_FILE = "data/quizzes.txt";

QuizApp::QuizApp() {
    DataFileManager::loadQuestions(bank, QUESTIONS_FILE);
    DataFileManager::loadQuizzes(manager, QUIZZES_FILE);
}

// ... showMainMenu(), questionMenu(), quizMenu() giữ nguyên như bản trước ...

void QuizApp::takeQuizMenu() {
    cout << "\n--- TAKE QUIZ ---\n";
    manager.displayAll();
    cout << "Nhap Quiz ID muon lam: ";
    string idStr; getline(cin, idStr);
    if (!InputValidator::isPositiveInt(idStr)) { cout << "ID khong hop le.\n"; return; }

    Quiz* quiz = manager.findQuiz(stoi(idStr));
    if (!quiz) { cout << "Khong tim thay Quiz.\n"; return; }

    if (!engine.start(quiz, &bank)) {
        cout << "Khong the bat dau (Quiz rong hoac loi).\n";
        return;
    }

    bool doing = true;
    while (doing) {
        engine.displayCurrentQuestion();

        cout << "1. Tra loi\n2. Cau tiep\n3. Cau truoc\n4. Nop bai\n0. Thoat (chua nop)\nChon: ";
        string cmd; getline(cin, cmd);
        if (!InputValidator::isPositiveInt(cmd)) { cout << "Lua chon khong hop le.\n"; continue; }
        int c = stoi(cmd);

        switch (c) {
            case 1: {
                cout << "Nhap dap an: ";
                string ans; getline(cin, ans);
                if (!engine.answer(ans)) cout << "Khong the ghi nhan dap an.\n";
                break;
            }
            case 2:
                if (!engine.next()) cout << "Da o cau cuoi.\n";
                break;
            case 3:
                if (!engine.previous()) cout << "Da o cau dau.\n";
                break;
            case 4: {
                if (engine.submit()) {
                    engine.showResult();
                    cout << "Tong diem: " << engine.getScore() << "\n";
                    doing = false;
                } else {
                    cout << "Nop bai that bai.\n";
                }
                break;
            }
            case 0:
                doing = false;
                break;
            default:
                cout << "Lua chon khong hop le.\n";
        }
    }
}
