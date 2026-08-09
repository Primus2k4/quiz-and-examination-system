# Test Cases – Question / MCQ / TF / QuestionBank

## Summary

| ID | Test | Expected result |
|---|---|---|
| Q-01 | Create valid MCQ | Object is valid |
| Q-02 | MCQ through `Question*` | Base pointer calls MCQ behavior |
| Q-03 | MCQ `getType()` | Returns `MCQ` |
| Q-04 | MCQ correct answer uppercase | Accepted |
| Q-05 | MCQ correct answer lowercase | Accepted |
| Q-06 | MCQ wrong answer | Rejected |
| Q-07 | MCQ empty answer | Rejected |
| Q-08 | MCQ multi-character answer | Rejected |
| Q-09 | MCQ get first option | Correct value returned |
| Q-10 | MCQ get last option | Correct value returned |
| Q-11 | MCQ get option with -1 | Rejected |
| Q-12 | MCQ get option with 4 | Rejected |
| Q-13 | MCQ set valid option | Updated |
| Q-14 | MCQ set empty option | Rejected |
| Q-15 | MCQ set invalid option index | Rejected |
| Q-16 | MCQ set correct option lowercase | Accepted and normalized |
| Q-17 | MCQ set correct option E | Rejected |
| Q-18 | Question set valid prompt | Updated |
| Q-19 | Question set empty prompt | Rejected |
| Q-20 | Question set valid points | Updated |
| Q-21 | Question set zero points | Rejected |
| Q-22 | Question set negative points | Rejected |
| Q-23 | MCQ empty option validation | Invalid |
| Q-24 | MCQ invalid correct option validation | Invalid |
| Q-25 | MCQ non-positive ID validation | Invalid |
| Q-26 | MCQ non-positive points validation | Invalid |
| Q-27 | MCQ empty prompt validation | Invalid |
| TF-01 | Create valid TF | Object is valid |
| TF-02 | TF `getType()` | Returns `TF` |
| TF-03 | TF true | Accepted when correct answer is true |
| TF-04 | TF TRUE | Accepted case-insensitively |
| TF-05 | TF True | Accepted case-insensitively |
| TF-06 | TF false when true is correct | Rejected |
| TF-07 | TF invalid answer | Rejected |
| TF-08 | TF empty answer | Rejected |
| TF-09 | Change TF correct answer | Stored correctly |
| TF-10 | TF false after setter | Accepted |
| TF-11 | TF invalid ID | Invalid |
| TF-12 | TF empty prompt | Invalid |
| TF-13 | TF invalid points | Invalid |
| QB-01 | New QuestionBank | Count is 0 |
| QB-02 | Add nullptr | Rejected |
| QB-03 | Add valid MCQ | Accepted |
| QB-04 | Add valid TF | Accepted |
| QB-05 | Count after add | Correct count |
| QB-06 | Find MCQ by ID | Returns MCQ |
| QB-07 | Find TF by ID | Returns TF |
| QB-08 | `getAt(0)` | Returns first question |
| QB-09 | `getAt(1)` | Returns second question |
| QB-10 | Out-of-range `getAt()` | Returns nullptr |
| QB-11 | Duplicate ID | Rejected; count unchanged |
| QB-12 | Empty prompt | Rejected; count unchanged |
| QB-13 | Zero points | Rejected; count unchanged |
| QB-14 | Empty MCQ option | Rejected; count unchanged |
| QB-15 | Invalid MCQ correct option | Rejected; count unchanged |
| QB-16 | Find unknown ID | Returns nullptr |
| QB-17 | Remove unknown ID | Rejected; count unchanged |
| QB-18 | Update same ID/type | Accepted |
| QB-19 | Updated prompt | Stored |
| QB-20 | Updated points | Stored |
| QB-21 | Updated MCQ answer | Stored |
| QB-22 | Update with different ID | Rejected; original remains |
| QB-23 | Update with different type | Rejected; original remains |
| QB-24 | Update with invalid replacement | Rejected; original remains |
| QB-25 | Update unknown ID | Rejected |
| QB-26 | Search partial prompt | Matching questions displayed |
| QB-27 | Search uppercase keyword | Same result as lowercase |
| QB-28 | Search TF prompt | TF is found |
| QB-29 | Search with no match | Clear no-result message |
| QB-30 | Display all | Both MCQ and TF displayed |
| QB-31 | Display empty bank | Clear empty-bank message |
| QB-32 | Remove middle question | Removed |
| QB-33 | Remove shifts remaining questions | Order preserved |
| QB-34 | Remove first question | Remaining question moves to index 0 |
| QB-35 | Remove last question | Bank becomes empty |
| QB-36 | Remove from empty bank | Rejected |
| QB-37 | Add 100 questions | All accepted |
| QB-38 | Add 101st question | Rejected |
| QB-39 | Last valid index 99 | Valid |
| QB-40 | Index 100 | Invalid |

## Required assignment coverage

The project specification requires validation of duplicate/invalid questions, partial case-insensitive prompt search, MCQ/TF answer checking and runtime polymorphism through `Question*`. The tests above cover these areas for the Question module.

## Actual Result

Run `QuestionModuleTests` and record the console result here after the current branch is built.
