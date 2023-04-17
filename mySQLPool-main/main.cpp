#include <iostream>
#include <thread>
#include "MySQLPool/ConnectPool.h"

using namespace std;
using namespace chrono;


// 单次数据库插入和删除测试
void query() {
    MySQLConn conn;
    bool flag_connection = conn.connect("root", "admin", "test_mySQLPool", "localhost", 3306);
    cout << "connection flag value : " << flag_connection << endl;
    
    string sql = "insert into user(id, age, sex, name) values(7, 25, 'man', 'tom')";
    bool flag_insert = conn.update(sql);
    cout << "insert flag value :" << flag_insert << endl;

    string sql2 = "select * from user";
    conn.query(sql2);
    while (conn.next()) {
        cout << conn.value(0) << ", "
            << conn.value(1) << ", "
            << conn.value(2) << ", "
            << conn.value(3) << endl;
    }
}

// 批量数据库插入和查询测试
void op1(int begin, int end) {
    
    for (int i = begin; i <= end; ++i) {
        MySQLConn conn;
        conn.connect("root", "admin", "test_mySQLPool", "localhost", 3306);

        char sql[1024] = {0};
        sprintf(sql, "insert into user(id, age, sex, name) values(%d, 25, 'man', 'tom')", i);
        conn.update(sql);
    }
}

// 批量使用数据库连接池进行数据库的插入和查询测试
void op2(ConnectPool* pool, int begin, int end) {
    for (int i = begin; i != end; ++i) {
        std::shared_ptr<MySQLConn> conn = pool->get_connection();
        char sql[1024] = {0};
        sprintf(sql, "insert into user(id, age, sex, name) values(%d, 25, 'man', 'tom')", i);        conn->update(sql);
    }
}

// 单线程：使用/不使用连接池
void test1() {
// 不使用连接池
#if 0   // 1089 ms
    steady_clock::time_point begin = steady_clock::now();
    op1(0, 5000);
    steady_clock::time_point end = steady_clock::now();
    auto length = end - begin;
    cout << "It takes " << (length.count() / 1000000) << " ms for a single thread not to use the connection pool" << endl;
// 使用连接池
#else   // 6 ms
    ConnectPool* pool = ConnectPool::get_connect_pool();
    steady_clock::time_point begin = steady_clock::now();
    op2(pool, 0, 5000);
    steady_clock::time_point end = steady_clock::now();
    auto length = end - begin;
    cout << "It takes " << (length.count() / 1000000) << " ms for a single thread to use the connection pool" << endl;
#endif
}

// 多线程：使用/不使用连接池
void test2() {
#if 0   // 613 ms
    steady_clock::time_point begin = steady_clock::now();
    thread t1(op1, 0, 1000);
    thread t2(op1, 1000, 2000);
    thread t3(op1, 2000, 3000);
    thread t4(op1, 3000, 4000);
    thread t5(op1, 4000, 5000);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    steady_clock::time_point end = steady_clock::now();
    auto length = end - begin;
    cout << "It takes " << (length.count() / 1000000) << " ms for a multiple thread not to use the connection pool" << endl;

#else   // 11 ms
    ConnectPool* pool = ConnectPool::get_connect_pool();
    steady_clock::time_point begin = steady_clock::now();
    thread t1(op2, pool, 0, 1000);
    thread t2(op2, pool, 1000, 2000);
    thread t3(op2, pool, 2000, 3000);
    thread t4(op2, pool, 3000, 4000);
    thread t5(op2, pool, 4000, 5000);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    steady_clock::time_point end = steady_clock::now();
    auto length = end - begin;
    cout << "It takes " << (length.count() / 1000000) << " ms for a multiple thread to use the connection pool" << endl;
#endif
}

int main() {
    // test1();
    test2();
    return 0;
}