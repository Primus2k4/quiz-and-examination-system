#ifndef MCQ_H
#define MCQ_H

#include "Question.h"
#include <string>

class MCQ : public Question {
public:
    MCQ(int id,
        const std::string& prompt,
        int points,
        const std::string options[4],
        char correctOption);

    ~MCQ() override = default;
    void display() const override;

    bool checkAnswer(const std::string& answer) const override;
    std::string getType() const override;
    std::string getOption(int index) const; 
    char getCorrectOption() const;
    bool setOption(int index, const std::string& text); 
    bool setCorrectOption(char option);                 

private:
    std::string m_options[4];  
    char m_correctOption; 
};

#endif 