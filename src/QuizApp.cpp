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
    DataFileManager::loadQuizzes(manager, bank, QUIZZES_FILE);
}

void QuizApp::run() {
    int choice;
    bool exitApp = false;

    while (!exitApp) {
        showMainMenu();
        string input;
        getline(cin, input);

        if (!InputValidator::isPositiveInt(input) || input.size() > 1) {
            cout << "Lua chon khong hop le, vui long nhap lai.\n";
            continue;
        }
        choice = stoi(input);

        switch (choice) {
            case 1: questionMenu(); break;
            case 2: quizMenu(); break;
            case 3: takeQuizMenu(); break;
            case 4:
                DataFileManager::saveQuestions(bank, QUESTIONS_FILE);
                DataFileManager::saveQuizzes(manager, QUIZZES_FILE);
                cout << "Da luu du lieu. Tam biet!\n";
                exitApp = true;
                break;
            default:
                cout << "Lua chon ngoai pham vi, vui long nhap lai.\n";
        }
    }
}

void QuizApp::showMainMenu() {
    cout << "\n===== QUIZ SYSTEM =====\n";
    cout << "1. Question Bank\n";
    cout << "2. Quiz Management\n";
    cout << "3. Take Quiz\n";
    cout << "4. Save & Exit\n";
    cout << "Chon: ";
}

void QuizApp::questionMenu() {
    bool back = false;
    while (!back) {
        cout << "\n--- QUESTION BANK ---\n";
        cout << "1. Them cau hoi MCQ\n";
        cout << "2. Them cau hoi TF\n";
        cout << "3. Xoa cau hoi\n";
        cout << "4. Tim theo ID\n";
        cout << "5. Tim theo tu khoa prompt\n";
        cout << "6. Hien thi tat ca\n";
        cout << "0. Quay lai\n";
        cout << "Chon: ";

        string input;
        getline(cin, input);
        if (!InputValidator::isPositiveInt(input)) {
            cout << "Lua chon khong hop le.\n";
            continue;
        }
        int choice = stoi(input);

        switch (choice) {
            case 1: {
                cout << "Nhap id (so nguyen duong): ";
                string idStr; getline(cin, idStr);
                if (!InputValidator::isPositiveInt(idStr)) { cout << "ID khong hop le.\n"; break; }

                cout << "Nhap points (so nguyen duong): ";
                string ptStr; getline(cin, ptStr);
                if (!InputValidator::isPositiveInt(ptStr)) { cout << "Points khong hop le.\n"; break; }

                cout << "Nhap prompt: ";
                string prompt; getline(cin, prompt);
                if (!InputValidator::isNonEmptyString(prompt)) { cout << "Prompt khong duoc rong.\n"; break; }

                string options[4];
                bool validOptions = true;
                for (int i = 0; i < 4; i++) {
                    cout << "Nhap lua chon " << (char)('A' + i) << ": ";
                    getline(cin, options[i]);
                    if (!InputValidator::isNonEmptyString(options[i])) { validOptions = false; }
                }
                if (!validOptions) { cout << "Lua chon khong duoc rong.\n"; break; }

                cout << "Nhap dap an dung (A-D): ";
                string correctStr; getline(cin, correctStr);
                if (correctStr.empty() || !InputValidator::isValidChoice(correctStr[0], 'A', 'D')) {
                    cout << "Dap an phai la A-D.\n"; break;
                }

                Question* q = new MCQ(stoi(idStr), prompt, stoi(ptStr), options, toupper(correctStr[0]));
                if (!bank.addQuestion(q)) { cout << "Them that bai (ID trung).\n"; delete q; }
                else cout << "Them cau hoi thanh cong.\n";
                break;
            }
            case 2: {
                cout << "Nhap id (so nguyen duong): ";
                string idStr; getline(cin, idStr);
                if (!InputValidator::isPositiveInt(idStr)) { cout << "ID khong hop le.\n"; break; }

                cout << "Nhap points (so nguyen duong): ";
                string ptStr; getline(cin, ptStr);
                if (!InputValidator::isPositiveInt(ptStr)) { cout << "Points khong hop le.\n"; break; }

                cout << "Nhap prompt: ";
                string prompt; getline(cin, prompt);
                if (!InputValidator::isNonEmptyString(prompt)) { cout << "Prompt khong duoc rong.\n"; break; }

                cout << "Dap an dung (true/false): ";
                string ansStr; getline(cin, ansStr);
                bool correctAnswer;
                if (ansStr == "true") correctAnswer = true;
                else if (ansStr == "false") correctAnswer = false;
                else { cout << "Chi nhap true hoac false.\n"; break; }

                Question* q = new TF(stoi(idStr), prompt, stoi(ptStr), correctAnswer);
                if (!bank.addQuestion(q)) { cout << "Them that bai (ID trung).\n"; delete q; }
                else cout << "Them cau hoi thanh cong.\n";
                break;
            }
            case 3: {
                cout << "Nhap id can xoa: ";
                string idStr; getline(cin, idStr);
                if (!InputValidator::isPositiveInt(idStr)) { cout << "ID khong hop le.\n"; break; }
                int id = stoi(idStr);
                if (bank.removeQuestion(id)) {
                    manager.removeQuestionFromAllQuiz(id);
                    cout << "Da xoa cau hoi khoi Question Bank va cac Quiz lien quan.\n";
                } else {
                    cout << "Khong tim thay ID nay.\n";
                }
                break;
            }
            case 4: {
                cout << "Nhap id can tim: ";
                string idStr; getline(cin, idStr);
                if (!InputValidator::isPositiveInt(idStr)) { cout << "ID khong hop le.\n"; break; }
                Question* q = bank.findById(stoi(idStr));
                if (q) q->display();
                else cout << "Khong tim thay.\n";
                break;
            }
            case 5: {
                cout << "Nhap tu khoa: ";
                string keyword; getline(cin, keyword);
                bank.displayByPrompt(keyword);
                break;
            }
            case 6:
                bank.displayAll();
                break;
            case 0:
                back = true;
                break;
            default:
                cout << "Lua chon khong hop le.\n";
        }
    }
}

void QuizApp::quizMenu() {
    bool back = false;
    while (!back) {
        cout << "\n--- QUIZ MANAGEMENT ---\n";
        cout << "1. Tao Quiz\n";
        cout << "2. Doi ten Quiz\n";
        cout << "3. Xoa Quiz\n";
        cout << "4. Them cau hoi vao Quiz\n";
        cout << "5. Xoa cau hoi khoi Quiz\n";
        cout << "6. Hien thi tat ca Quiz\n";
        cout << "0. Quay lai\n";
        cout << "Chon: ";

        string input;
        getline(cin, input);
        if (!InputValidator::isPositiveInt(input)) { cout << "Lua chon khong hop le.\n"; continue; }
        int choice = stoi(input);

        switch (choice) {
            case 1: {
                cout << "Nhap Quiz ID: ";
                string idStr; getline(cin, idStr);
                if (!InputValidator::isPositiveInt(idStr)) { cout << "ID khong hop le.\n"; break; }
                cout << "Nhap title: ";
                string title; getline(cin, title);
                if (!InputValidator::isNonEmptyString(title)) { cout << "Title khong duoc rong.\n"; break; }
                if (manager.createQuiz(stoi(idStr), title)) cout << "Tao Quiz thanh cong.\n";
                else cout << "Tao that bai (ID trung hoac title trung/rong).\n";
                break;
            }
            case 2: {
                cout << "Nhap Quiz ID: ";
                string idStr; getline(cin, idStr);
                if (!InputValidator::isPositiveInt(idStr)) { cout << "ID khong hop le.\n"; break; }
                cout << "Nhap title moi: ";
                string title; getline(cin, title);
                if (manager.renameQuiz(stoi(idStr), title)) cout << "Doi ten thanh cong.\n";
                else cout << "Doi ten that bai.\n";
                break;
            }
            case 3: {
                cout << "Nhap Quiz ID can xoa: ";
                string idStr; getline(cin, idStr);
                if (!InputValidator::isPositiveInt(idStr)) { cout << "ID khong hop le.\n"; break; }
                if (manager.deleteQuiz(stoi(idStr))) cout << "Da xoa Quiz.\n";
                else cout << "Khong tim thay Quiz.\n";
                break;
            }
            case 4: {
                cout << "Nhap Quiz ID: ";
                string qzIdStr; getline(cin, qzIdStr);
                cout << "Nhap Question ID: ";
                string qIdStr; getline(cin, qIdStr);
                if (!InputValidator::isPositiveInt(qzIdStr) || !InputValidator::isPositiveInt(qIdStr)) {
                    cout << "ID khong hop le.\n"; break;
                }
                if (manager.addQuestionToQuiz(stoi(qzIdStr), stoi(qIdStr), bank))
                    cout << "Them cau hoi vao Quiz thanh cong.\n";
                else
                    cout << "That bai (ID khong ton tai hoac da co trong Quiz).\n";
                break;
            }
            case 5: {
                cout << "Nhap Quiz ID: ";
                string qzIdStr; getline(cin, qzIdStr);
                cout << "Nhap Question ID can xoa: ";
                string qIdStr; getline(cin, qIdStr);
                if (!InputValidator::isPositiveInt(qzIdStr) || !InputValidator::isPositiveInt(qIdStr)) {
                    cout << "ID khong hop le.\n"; break;
                }
                if (manager.removeQuestionFromQuiz(stoi(qzIdStr), stoi(qIdStr)))
                    cout << "Da xoa cau hoi khoi Quiz.\n";
                else
                    cout << "That bai.\n";
                break;
            }
            case 6:
                manager.displayAll();
                break;
            case 0:
                back = true;
                break;
            default:
                cout << "Lua chon khong hop le.\n";
        }
    }
}

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
