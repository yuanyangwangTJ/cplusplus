#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

// 定义一个接口类
class Database {
public:
    virtual ~Database() {}
    virtual bool connect(const std::string& host) = 0;
    virtual bool disconnect() = 0;
    virtual int query(const std::string& sql) = 0;
};

// Mock类
class MockDatabase : public Database {
public:
    MOCK_METHOD(bool, connect, (const std::string& host), (override));
    MOCK_METHOD(bool, disconnect, (), (override));
    MOCK_METHOD(int, query, (const std::string& sql), (override));
};

// 被测试的类
class UserManager {
private:
    Database* db;
public:
    UserManager(Database* database) : db(database) {}
    
    bool initConnection(const std::string& host) {
        return db->connect(host);
    }
    
    int getUserCount() {
        return db->query("SELECT COUNT(*) FROM users");
    }
};

// 测试用例
TEST(UserManagerTest, InitConnectionTest) {
    MockDatabase mockDb;
    UserManager userManager(&mockDb);
    
    // 设置期望
    EXPECT_CALL(mockDb, connect("localhost"))
        .Times(1)
        .WillOnce(testing::Return(true));
        
    // 执行测试
    bool result = userManager.initConnection("localhost");
    
    // 验证结果
    EXPECT_TRUE(result);
}

TEST(UserManagerTest, GetUserCountTest) {
    MockDatabase mockDb;
    UserManager userManager(&mockDb);
    
    // 设置期望
    EXPECT_CALL(mockDb, query(testing::_))
        .Times(1)
        .WillOnce(testing::Return(42));
        
    // 执行测试
    int count = userManager.getUserCount();
    
    // 验证结果
    EXPECT_EQ(count, 42);
}

int main(int argc, char** argv) {
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
