#include "DataFileManager.h"
#include "MCQ.h"
#include "TF.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cctype>
using namespace std;

// Tach chuoi thu cong theo 1 ky tu phan cach
static int splitByChar(const string& str, char delim, string result[], int maxParts) {
    int count = 0;
    string current = "";
    for (int i = 0; i < (int)str.size(); i++) {
        if (str[i] == delim) {
            if (count < maxParts - 1) {
                result[count++] = current;
                current = "";
            } else {
                current += str[i]; // phan cuoi giu nguyen, tranh cat nham du lieu co dau |
            }
        } else {
            current += str[i];
        }
    }
    result[count++] = current;
    return count;
}

static bool isAllDigits(const string& s) {
    if (s.empty()) return false;
    for (int i = 0; i < (int)s.size(); i++)
        if (!isdigit((unsigned char)s[i])) return false;
    return true;
}

static bool toBool(const string& s, bool& out) {
    if (s == "true" || s == "True" || s == "TRUE") { out = true; return true; }
    if (s == "false" || s == "False" || s == "FALSE") { out = false; return true; }
    return false;
}

bool DataFileManager::loadQuestions(QuestionBank& bank, const string& filepath) {
    ifstream fin(filepath.c_str());
    if (!fin.is_open()) {
        cout << "Khong tim thay file " << filepath << ", bat dau voi Question Bank rong.\n";
        return false;
    }

    string line;
    int lineNo = 0;
    while (getline(fin, line)) {
        lineNo++;
        if (line.empty()) continue;

        // Doc truoc token dau tien de biet loai
        string typeParts[2];
        int firstSplit = splitByChar(line, '|', typeParts, 2);
        if (firstSplit < 2) {
            cout << "Dong " << lineNo << ": sai dinh dang, bo qua.\n";
            continue;
        }

        if (typeParts[0] == "MCQ") {
            string parts[6];
            int n = splitByChar(line, '|', parts, 6);
            if (n != 6 || !isAllDigits(parts[1]) || !isAllDigits(parts[2]) || parts[3].empty() || parts[5].empty()) {
                cout << "Dong " << lineNo << ": sai dinh dang MCQ, bo qua.\n";
                continue;
            }
            string options[4];
            int optCount = splitByChar(parts[4], ';', options, 4);
            if (optCount != 4 || options[0].empty() || options[1].empty() || options[2].empty() || options[3].empty()) {
                cout << "Dong " << lineNo << ": MCQ thieu 4 lua chon, bo qua.\n";
                continue;
            }
            char correctOpt = toupper(parts[5][0]);
            if (correctOpt < 'A' || correctOpt > 'D') {
                cout << "Dong " << lineNo << ": correctOption phai la A-D, bo qua.\n";
                continue;
            }
            int id = stoi(parts[1]);
            int points = stoi(parts[2]);
            if (points <= 0) {
                cout << "Dong " << lineNo << ": points phai > 0, bo qua.\n";
                continue;
            }
            Question* q = new MCQ(id, parts[3], points, options, correctOpt);
            if (!bank.addQuestion(q)) {
                cout << "Dong " << lineNo << ": id " << id << " bi trung, bo qua.\n";
                delete q;
            }
        }
        else if (typeParts[0] == "TF") {
            string parts[5];
            int n = splitByChar(line, '|', parts, 5);
            if (n != 5 || !isAllDigits(parts[1]) || !isAllDigits(parts[2]) || parts[3].empty()) {
                cout << "Dong " << lineNo << ": sai dinh dang TF, bo qua.\n";
                continue;
            }
            bool correctAnswer;
            if (!toBool(parts[4], correctAnswer)) {
                cout << "Dong " << lineNo << ": correctAnswer phai la true/false, bo qua.\n";
                continue;
            }
            int id = stoi(parts[1]);
            int points = stoi(parts[2]);
            if (points <= 0) {
                cout << "Dong " << lineNo << ": points phai > 0, bo qua.\n";
                continue;
            }
            Question* q = new TF(id, parts[3], points, correctAnswer);
            if (!bank.addQuestion(q)) {
                cout << "Dong " << lineNo << ": id " << id << " bi trung, bo qua.\n";
                delete q;
            }
        }
        else {
            cout << "Dong " << lineNo << ": loai cau hoi khong hop le, bo qua.\n";
        }
    }
    fin.close();
    return true;
}

bool DataFileManager::saveQuestions(const QuestionBank& bank, const string& filepath) {
    ofstream fout(filepath.c_str());
    if (!fout.is_open()) {
        cout << "Khong the ghi file " << filepath << "\n";
        return false;
    }
    // TODO: can QuestionBank cung cap ham duyet toan bo cau hoi (VD getAt(index))
    // va Question can co ham lay lai du lieu tho (getType/getId/getPrompt/getPoints)
    // de ghi dung dinh dang MCQ|.../TF|...
    fout.close();
    return true;
}

bool DataFileManager::loadQuizzes(QuizManager& manager, const string& filepath) {
    ifstream fin(filepath.c_str());
    if (!fin.is_open()) {
        cout << "Khong tim thay file " << filepath << ", bat dau voi Quiz rong.\n";
        return false;
    }

    string line;
    int lineNo = 0;
    while (getline(fin, line)) {
        lineNo++;
        if (line.empty()) continue;

        string parts[3];
        int n = splitByChar(line, '|', parts, 3);
        if (n != 3 || !isAllDigits(parts[0]) || parts[1].empty()) {
            cout << "Dong " << lineNo << ": sai dinh dang quiz, bo qua.\n";
            continue;
        }
        int quizId = stoi(parts[0]);
        string title = parts[1];
        if (!manager.createQuiz(quizId, title)) {
            cout << "Dong " << lineNo << ": quiz id " << quizId << " bi trung, bo qua.\n";
            continue;
        }
        // parts[2] la danh sach questionId cach nhau boi dau phay - can bank de add
        // Se hoan thien khi co tham chieu QuestionBank truyen vao ham nay
    }
    fin.close();
    return true;
}

bool DataFileManager::saveQuizzes(const QuizManager& manager, const string& filepath) {
    ofstream fout(filepath.c_str());
    if (!fout.is_open()) {
        cout << "Khong the ghi file " << filepath << "\n";
        return false;
    }
    // TODO: can QuizManager cung cap ham duyet toan bo Quiz (VD getAt(index))
    // va Quiz can co getQuestionCount()/getQuestionId(index) de ghi dung dinh dang
    fout.close();
    return true;
}
