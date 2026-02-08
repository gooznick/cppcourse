#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <iostream>


// sudo apt install libgtest-dev libgmock-dev pkg-config cmake

// ==========================================
// 1. The Interface (Dependency) 🔌
// ==========================================
class IDatabase {
public:
    virtual ~IDatabase() = default;
    virtual bool Login(std::string user, int port) = 0;
    virtual int Query(std::string sql) = 0;
};

// ==========================================
// 2. The Service (System Under Test) 🏗️
// ==========================================
class MyService {
    IDatabase* db_;
public:
    MyService(IDatabase* db) : db_(db) {}

    int ProcessData(std::string username) {
        // Business Logic:
        // 1. Try to Login
        if (!db_->Login(username, 8080)) {
            return -1; // Login failed ❌
        }

        // 2. Query Data
        int val = db_->Query("SELECT * FROM users");
        
        // 3. Process Result
        return val * 2; 
    }
};

// ==========================================
// 3. The Mock (Fake Implementation) 🎭
// ==========================================
class MockDatabase : public IDatabase {
public:
    // MOCK_METHOD(ReturnType, MethodName, (Args...), (Specs));
    MOCK_METHOD(bool, Login, (std::string, int), (override));
    MOCK_METHOD(int, Query, (std::string), (override));
};

// ==========================================
// 4. The Tests 🧪
// ==========================================
using ::testing::_;        // Match anything
using ::testing::Return;   // Force return value
using ::testing::Gt;       // Greater Than matcher

TEST(MyServiceTest, SuccessPath) {
    MockDatabase mock_db;
    MyService service(&mock_db);

    // script the expected behavior:
    // "Expect Login called with 'admin' and 8080 exactly once. Return true."
    EXPECT_CALL(mock_db, Login("admin", 8080))
        .Times(1)
        .WillOnce(Return(true));

    // "Expect Query called with ANY string. Return 21."
    EXPECT_CALL(mock_db, Query(_))
        .WillOnce(Return(21));

    // ACT
    int result = service.ProcessData("admin");

    // ASSERT
    EXPECT_EQ(result, 42); // 21 * 2 = 42
}

TEST(MyServiceTest, LoginFailure) {
    MockDatabase mock_db;
    MyService service(&mock_db);

    // "Expect Login called. Force it to return false."
    EXPECT_CALL(mock_db, Login(_, _))
        .WillOnce(Return(false));

    // Notice: We do NOT expect Query() to be called.
    // If the code calls Query(), the test will FAIL automatically!

    int result = service.ProcessData("hacker");
    EXPECT_EQ(result, -1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}