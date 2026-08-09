#ifndef QUESTION_H
#define QUESTION_H

#include <string>

class Question {
private:
    int m_id;
    std::string m_prompt;
    int m_points;
public:
    Question(int id, const std::string& prompt, int points);

    virtual ~Question() = default;
    virtual void display() const = 0;
    virtual bool checkAnswer(const std::string& answer) const = 0;
    virtual std::string getType() const = 0;
    virtual bool isValid() const = 0;
    int getId() const;
    std::string getPrompt() const;
    int getPoints() const;
    bool setPrompt(const std::string& prompt);   
    bool setPoints(int points);                  

  
};

#endif