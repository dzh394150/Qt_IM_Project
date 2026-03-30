# SingChat 即时通讯客户端

基于 Qt6 开发的桌面端即时通讯应用，支持私聊、群聊、文件传输等功能。

## 项目结构

```
20260329windows/
├── core/                    # 核心模块
│   ├── main.cpp             # 应用入口
│   ├── mainwindow.h/cpp/ui  # 主窗口
│
├── auth/                    # 认证模块
│   ├── login.h/cpp/ui       # 登录窗口
│   └── registerwidget.h/cpp/ui  # 注册窗口
│
├── chat/                    # 聊天模块
│   ├── chatmodel.h/cpp      # 聊天数据模型
│   ├── chatdelegate.h/cpp   # 聊天项渲染委托
│   ├── chatmessage.h        # 消息结构体定义
│   └── cacheutils.h/cpp     # 缓存工具
│
├── contacts/                # 联系人模块
│   ├── FriendListModel.h/cpp      # 好友列表模型
│   ├── FriendListDelegate.h/cpp   # 好友列表委托
│   ├── FriendRequestModel.h/cpp   # 好友请求模型
│   ├── FriendRequestDelegate.h/cpp # 好友请求委托
│   ├── GroupListModel.h/cpp       # 群聊列表模型
│   ├── GroupListDelegate.h/cpp    # 群聊列表委托
│   ├── AddFriends.h/cpp/ui        # 添加好友窗口
│   └── CreateGroup.h/cpp/ui       # 创建群聊窗口
│
├── storage/                 # 数据存储模块
│   └── LocalChatCache.h/cpp # SQLite本地缓存
│
└── filetransfer/            # 文件传输模块
    ├── FileDownloader.h/cpp       # 文件下载器
    ├── FileTransferWorker.h/cpp   # 文件传输工作线程
    ├── FileUtils.h/cpp            # 文件工具
    ├── FileAttachment.h           # 文件附件结构体
    ├── FileChunkInfo.h            # 分片信息结构体
    └── ImagePreviewDialog.h/cpp/ui # 图片预览弹窗

├── backend/                 # 后端服务（Go语言）
│   ├── server.go            # 主服务入口（WebSocket、HTTP服务）
│   ├── send_check.go        # 消息发送校验
│   ├── go.mod/go.sum        # Go模块依赖
│   ├── 总表.sql              # 数据库表结构
│   ├── config/              # 配置模块
│   │   └── config.go        # 全局配置（MySQL/Redis/Kafka）
│   ├── database/            # 数据库模块
│   │   └── db.go            # MySQL连接与数据操作
│   ├── cache/               # 缓存模块
│   │   └── cache.go         # Redis缓存操作（好友列表、聊天记录）
│   ├── messaging/           # 消息队列模块
│   │   └── kafka_client.go  # Kafka生产者/消费者
│   └── deploy/              # 部署配置
│       ├── Dockerfile           # Docker镜像构建
│       ├── docker-compose.yaml  # Docker编排配置
│       ├── nginx网关.txt         # Nginx反向代理配置
│       ├── algo_deploy.sh       # 阿里云部署脚本
│       └── onekey-deploy.sh     # 一键部署脚本
```

### 各模块详细说明

#### core/ - 核心模块
- **main.cpp**: 应用程序入口，创建 QApplication 并启动登录窗口
- **mainwindow.h/cpp**: 主窗口实现，包含：
  - 聊天界面布局与管理
  - WebSocket 连接管理（SSL + 自定义帧解析）
  - 好友列表、群聊列表展示
  - 消息收发处理
  - 文件传输协调
  - 心跳保活机制
  - 懒加载历史消息
- **mainwindow.ui**: 主窗口界面设计文件
- **ChatInputTextEdit 类**: 自定义文本输入框，支持拖拽/粘贴发送文件

#### auth/ - 认证模块
- **login.h/cpp**: 登录窗口实现
  - TCP + SSL 连接建立
  - WebSocket 握手（手动实现）
  - 登录消息发送（SHA256密码加密）
  - SSL 证书验证与错误处理
- **registerwidget.h/cpp**: 注册窗口实现
  - 头像选择与上传（HTTPS）
  - 手机号验证码注册
  - 注册消息发送
- **.ui 文件**: 登录/注册界面设计

#### chat/ - 聊天模块
- **chatmessage.h**: 消息结构体定义
  - 消息类型：文本、图片、文件、视频
  - 消息归属：自己、对方、系统
  - 时间戳、已读状态、同步状态
  - 文件附件信息
- **chatmodel.h/cpp**: 聊天数据模型（QAbstractListModel）
  - 消息列表管理
  - 角色定义（内容、发送者、头像、时间戳等）
  - 支持消息添加、批量插入、清空
- **chatdelegate.h/cpp**: 聊天项渲染委托
  - 自定义消息气泡绘制
  - 头像显示
  - 文件消息特殊渲染
  - 图片消息预览
- **cacheutils.h/cpp**: 缓存工具函数

#### contacts/ - 联系人模块
- **FriendListModel.h/cpp**: 好友列表数据模型
  - 好友数据结构（用户名、头像、未读数）
  - 好友增删查改
  - 未读消息数量管理
- **FriendListDelegate.h/cpp**: 好友列表渲染委托
  - 头像 + 用户名 + 未读红点展示
- **FriendRequestModel.h/cpp**: 好友请求模型
  - 请求数据结构（来源用户、头像、验证消息、状态）
- **FriendRequestDelegate.h/cpp**: 好友请求渲染委托
- **GroupListModel.h/cpp**: 群聊列表模型
  - 群聊数据结构（群ID、名称、头像、成员、未读数）
- **GroupListDelegate.h/cpp**: 群聊列表渲染委托
- **AddFriends.h/cpp/ui**: 添加好友窗口
  - 搜索用户
  - 发送好友请求
- **CreateGroup.h/cpp/ui**: 创建群聊窗口
  - 群名称、头像设置
  - 成员选择

#### storage/ - 数据存储模块
- **LocalChatCache.h/cpp**: SQLite本地缓存管理（单例模式）
  - 数据库初始化与表创建
  - 聊天记录查询、保存、批量插入
  - 消息同步状态管理
  - 用户信息缓存（头像URL等）
  - 群聊信息缓存
  - 未读消息标记
  - 增量同步支持

#### filetransfer/ - 文件传输模块
- **FileAttachment.h**: 文件附件结构体
  - 文件名、大小、类型、URL
- **FileChunkInfo.h**: 文件分片信息结构体
  - 分片索引、数据、总片数
- **FileDownloader.h/cpp**: 文件下载器
  - 支持普通下载和分片并发下载
  - 2MB分片大小，最大8并发
  - Range请求支持
  - 断点续传
  - 下载进度反馈
- **FileTransferWorker.h/cpp**: 文件传输工作线程
  - 在独立线程中处理文件分片读取
  - WebSocket帧封装
  - 避免阻塞UI
- **FileUtils.h/cpp**: 文件工具函数
  - 文件大小格式化
  - 文件名处理
  - 文件类型判断
- **ImagePreviewDialog.h/cpp/ui**: 图片预览弹窗
  - 大图查看
  - 缩放、拖拽
  - 保存图片

#### backend/ - 后端服务（Go语言）
- **server.go**: 主服务入口（2400+行，核心逻辑）
  - WebSocket 服务（gorilla/websocket）
  - HTTP API 服务（头像上传、文件上传/下载）
  - 消息路由与转发（私聊、群聊）
  - 文件分片接收与重组
  - 用户连接管理（在线状态）
  - 心跳检测
- **send_check.go**: 消息发送校验
- **config/config.go**: 全局配置管理
  - MySQL DSN 配置
  - Redis 连接配置
  - Kafka 配置（Broker、Topic、ConsumerGroup）
  - 缓存过期时间配置
  - 支持环境变量读取
- **database/db.go**: 数据库操作
  - MySQL 连接初始化
  - 用户表操作（注册、登录、信息查询）
  - 好友关系操作（添加、查询、同意/拒绝）
  - 聊天记录存储与查询
  - 文件元数据管理
- **cache/cache.go**: Redis 缓存操作
  - 好友列表缓存（JSON序列化）
  - 聊天记录缓存（Sorted Set按时间戳排序）
  - 缓存穿透保护
  - 缓存过期策略
- **messaging/kafka_client.go**: 消息队列
  - Kafka 生产者初始化（SyncProducer）
  - 消费者组管理
  - 消息分发（普通聊天 vs 文件传输独立Topic）
- **deploy/**: 部署相关
  - Docker 容器化配置
  - Nginx 反向代理
  - 一键部署脚本

## 技术栈

- **框架**: Qt 6.10.1
- **编译器**: MinGW 64-bit
- **通信协议**: WebSocket over SSL (WSS)
- **本地存储**: SQLite
- **UI 框架**: Qt Widgets

## 核心功能

### 1. 用户认证
- 账号密码登录（SHA256加密）
- 手机号验证码注册
- 头像上传

### 2. 即时通讯
- 私聊（一对一聊天）
- 群聊（多人群组聊天）
- 文本消息
- 图片消息（支持粘贴/拖拽发送）
- 文件传输（支持大文件分片传输）

### 3. 好友管理
- 添加好友
- 好友列表展示
- 好友请求处理（同意/拒绝）
- 未读消息红点提示

### 4. 群聊管理
- 创建群聊
- 群成员管理
- 群聊列表展示

### 5. 本地缓存
- 聊天记录本地存储（SQLite）
- 懒加载历史消息
- 离线消息同步

### 6. 文件传输
- 支持图片、视频、压缩文件
- 分片上传/下载
- 并发分片传输优化
- 断点续传支持
- 图片预览功能

## 编译说明

### 环境要求
- Qt 6.10.1 或更高版本
- MinGW 64-bit 编译器
- OpenSSL（用于SSL连接）

### 编译步骤
1. 使用 Qt Creator 打开 `20260329windows.pro`
2. 执行 **Build → Run qmake**
3. 执行 **Build → Build All**

### 运行配置
- 服务器地址: `www.singchat.chat:443`
- SSL证书路径: `cert/singchat.chat_bundle.crt`

## 项目特点

- **模块化设计**: 按功能模块划分文件夹，代码结构清晰
- **自定义 WebSocket**: 手动实现 WebSocket 帧解析，支持 SSL 加密
- **异步文件传输**: 文件传输不阻塞 UI，支持边传边聊
- **本地缓存优化**: SQLite 缓存聊天记录，支持增量同步
- **并发下载**: 文件分片并发下载，充分利用带宽

## 最近更新

- 重构项目文件夹结构，按功能模块分类
- 优化文件传输性能，支持并发分片下载
- 添加本地聊天记录缓存
