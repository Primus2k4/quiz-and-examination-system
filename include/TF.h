#ifndef TF_H
#define TF_H

#include "Question.h"
#include <string>

class TF : public Question {
private:
    bool m_correctAnswer;
public:
    TF(int id,
        const std::string& prompt,
        int points,
        bool correctAnswer);

    ~TF() override = default;
    void display() const override;

    bool checkAnswer(const std::string& answer) const override;
    std::string getType() const override;
    bool getCorrectAnswer() const;
    void setCorrectAnswer(bool correctAnswer);


};

#endif 