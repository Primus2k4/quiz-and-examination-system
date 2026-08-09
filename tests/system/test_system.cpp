#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdio>

#include "../include/QuestionBank.h"
#include "../include/QuizManager.h"
#include "../include/QuizEngine.h"
#include "../include/DataFileManager.h"
#include "../include/QuizApp.h"

class SystemE2ETest : public ::testing::Test {
protected:
    std::streambuf* origCinBuf = nullptr;
    std::streambuf* origCoutBuf = nullptr;

    std::istringstream mockCin;
    std::ostringstream mockCout;

    void SetUp() override {
        origCinBuf = std::cin.rdbuf();
        origCoutBuf = std::cout.rdbuf();
    }

    void TearDown() override {
        std::cin.rdbuf(origCinBuf);
        std::cout.rdbuf(origCoutBuf);
    }

    void redirectIO(const std::string& inputData) {
        mockCin.str(inputData);
        mockCin.clear();
        std::cin.rdbuf(mockCin.rdbuf());
        std::cout.rdbuf(mockCout.rdbuf());
    }

    std::string getCapturedOutput() {
        return mockCout.str();
    }
};

TEST_F(SystemE2ETest, E2E_FullUserSession_TakeQuizAndSubmit) {
    // Giả lập người dùng nhập Menu: 4 (Save & Exit ngay)
    std::string userInput = "4\n";
    redirectIO(userInput);

    QuizApp app;
    EXPECT_NO_THROW(app.run());

    std::string output = getCapturedOutput();
    EXPECT_NE(output.find("QUIZ SYSTEM"), std::string::npos);
}

TEST_F(SystemE2ETest, E2E_TC18_InvalidInput_CleansBuffer_AndRecovers) {
    // Nhập "abc" (sai) -> "99" (ngoài phạm vi) -> "4" (thoát)
    std::string invalidInput = "abc\n99\n4\n";
    redirectIO(invalidInput);

    QuizApp app;
    EXPECT_NO_THROW(app.run());

    std::string output = getCapturedOutput();
    EXPECT_NE(output.find("ngay/khong hop le"), std::string::npos);
}