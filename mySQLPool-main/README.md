# 数据库连接池

项目中 [MySQLConn](./MySQLPool/MySQLConn.h) 是对 `MySQLConn` 的封装，使用 [config](./config.json) 进行配置，对 `json` 的解析用到了之前实现的 [Json](https://github.com/Arthur940621/myJson) 解析器。数据库连接池 [ConnectPool](./MySQLPool/ConnectPool.h) 使用单例模式，使用队列保存创建的 [MySQLConn](./MySQLPool/MySQLConn.h) 连接，实现对数据库连接资源的复用。

# 使用方法

```cpp
#include "MySQLPool/ConnectPool.h" // 包含头文件

ConnectPool* pool = ConnectPool::get_connect_pool(); // 初始化连接池

std::shared_ptr<MySQLConn> conn = pool->get_connection(); // 从连接池中取出一个连接

string sql = "insert into user_tb values(null, 'xxx', 'xxx')";
conn->update(sql); // 对连接进行操作
```
