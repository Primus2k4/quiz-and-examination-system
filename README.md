# Quiz and Examination System

Ứng dụng console dùng để quản lý ngân hàng câu hỏi, quản lý nhiều bài kiểm tra và mô phỏng phiên làm bài. 

## Tính năng đã hoàn thành

- Question Bank: nạp/lưu file, thêm, sửa, xóa, hiển thị và tìm theo ID hoặc một phần prompt không phân biệt hoa/thường.
- Xác thực câu hỏi: từ chối ID trùng, prompt rỗng, điểm không hợp lệ, lựa chọn MCQ rỗng và đáp án sai định dạng.
- Quiz: tạo, đổi tên, xóa, hiển thị; thêm/xóa Question ID và giữ thứ tự câu hỏi.
- Cascade delete: xóa câu hỏi khỏi Question Bank và các Quiz liên quan.
- Take Quiz: Start, nhập/sửa đáp án, Next/Previous, Submit và hiển thị kết quả.
- Chấm MCQ/TF qua `Question` pointer/reference bằng runtime polymorphism.
- Persistence: đọc/ghi `questions.txt`, `quizzes.txt`, bỏ qua dòng lỗi và lưu khi `Save & Exit`.
- Automated tests bằng Google Test Framework ở ba lớp: unit, integration và system/E2E.

## Yêu cầu môi trường

- CMake 3.14 trở lên.
- Trình biên dịch hỗ trợ C++11 trở lên: MSVC, GCC hoặc Clang.
- Kết nối mạng ở lần cấu hình đầu tiên để CMake tải Google Test v1.10.0 qua `FetchContent`.

Kiểm tra CMake:

```powershell
cmake --version
```

## Build dự án

Chạy các lệnh tại thư mục gốc, nơi chứa `CMakeLists.txt`.

### Windows - Visual Studio/MSVC

```CMD
mkdir build
cd build
cmake ..
cmake --build .
```

Các executable được tạo trong `build/Debug/`:

```text
QuizApp.exe
run_unit_tests.exe
run_integration_tests.exe
run_system_tests.exe
```

### Chạy app chính từ terminal - Windows Debug

```powershell
.\build\Debug\QuizApp.exe
```

### Chạy bằng cách double-click `QuizApp.exe`

Có thể double-click `build/Debug/QuizApp.exe`. Từ vị trí này, ứng dụng tự đi lên hai cấp để tìm và sử dụng chung `data/` ở project root.

Menu chính:

```text
1. Question Bank
2. Quiz Management
3. Take Quiz
4. Save & Exit
```

Luồng demo đề xuất:

1. Mở `Question Bank` để xem hoặc thêm câu hỏi.
2. Mở `Quiz Management`, tạo Quiz và thêm các Question ID.
3. Chọn `Take Quiz`, nhập Quiz ID, trả lời và Submit.
4. Chọn `Save & Exit` để lưu dữ liệu trước khi thoát.

## Định dạng dữ liệu

### `data/questions.txt`

```text
MCQ|id|points|prompt|optionA;optionB;optionC;optionD|correctOption
TF|id|points|prompt|correctAnswer
```

Ví dụ:

```text
MCQ|101|2|2 + 2 = ?|3;4;5;6|B
TF|102|1|C++ supports inheritance|true
```

### `data/quizzes.txt`

```text
quizId|title|questionId1,questionId2,...
```

Ví dụ:

```text
201|C++ Basics|101,102
```

Không dùng các ký tự phân tách `|`, `;` hoặc `,` trong prompt, lựa chọn hoặc title của Quiz.

## Chạy automated tests

Windows Debug, chạy từ thư mục gốc:

```powershell
.\build\Debug\run_unit_tests.exe --gtest_color=no
.\build\Debug\run_integration_tests.exe --gtest_color=no
.\build\Debug\run_system_tests.exe --gtest_color=no
```

Chạy toàn bộ test bằng CTest:

```powershell
ctest --test-dir build -C Debug --output-on-failure
```

Chạy một test hoặc test suite:

```powershell
.\build\Debug\run_unit_tests.exe --gtest_filter=*TC01*
.\build\Debug\run_unit_tests.exe --gtest_filter=QuizEngineTest.*
```

## Cấu trúc thư mục

```text
quiz-and-examination-system/
|-- include/                 # Header/interface
|   |-- Question.h           # Abstract base class
|   |-- MCQ.h, TF.h          # Hai loại câu hỏi
|   |-- QuestionBank.h       # Quản lý và sở hữu câu hỏi
|   |-- Quiz.h               # Một Quiz và thứ tự Question ID
|   |-- QuizManager.h        # Quản lý nhiều Quiz
|   |-- QuizEngine.h         # Phiên làm bài và chấm điểm
|   |-- DataFileManager.h    # Persistence file text
|   |-- InputValidator.h     # Validation console input
|   `-- QuizApp.h            # Menu và điều phối use case
|-- src/                     # Implementation của các class
|-- data/
|   |-- questions.txt        # Dữ liệu mẫu Question Bank
|   `-- quizzes.txt          # Dữ liệu mẫu Quiz
|-- tests/
|   |-- unit/                # Unit tests
|   |-- integration/         # Integration tests
|   |-- system/              # System/E2E tests
|   `-- test_cases.md        # Mô tả test case
|-- main.cpp                 # Khởi tạo và chạy QuizApp
|-- CMakeLists.txt           # Cấu hình build app và tests
|-- README.md                # Tài liệu hướng dẫn cách build, chạy dự án cùng các thông tin tổng quan
`-- build/                   # Build artifacts
```

## Kiến trúc và trách nhiệm

| Thành phần | Trách nhiệm |
|---|---|
| `Question`, `MCQ`, `TF` | Domain model và hành vi đa hình `display()`/`checkAnswer()` |
| `QuestionBank` | Collection câu hỏi, validation, tìm kiếm và CRUD |
| `Quiz`, `QuizManager` | Quản lý Quiz và thứ tự Question ID |
| `QuizEngine` | Trạng thái phiên làm bài, đáp án và tính điểm |
| `DataFileManager` | Chuyển đổi giữa file text và object |
| `InputValidator` | Kiểm tra dữ liệu nhập |
| `QuizApp` | Menu và điều phối các module |
| `main.cpp` | Chỉ khởi tạo `QuizApp` và bắt đầu ứng dụng |

```text
Console -> QuizApp -> QuestionBank / QuizManager / QuizEngine
                       ^                         |
                       |                         v
                  DataFileManager <------- Question objects
```

## Giới hạn dự án đã chọn

- Tối đa 100 câu hỏi trong Question Bank.
- Tối đa 10 Quiz.
- Tối đa 30 câu hỏi trong mỗi Quiz.
- Chỉ hỗ trợ hai loại câu hỏi bắt buộc: MCQ và TF.
- MCQ có đúng 4 lựa chọn A-D; TF dùng `true` hoặc `false`.
- ID và points phải là số nguyên dương.
- Quiz chỉ lưu Question ID, không lưu con trỏ trực tiếp tới Question.
- Không lưu phiên làm bài, đáp án hoặc kết quả sau khi thoát.
- Không có GUI, database, networking, tài khoản, timer hoặc multithreading.
