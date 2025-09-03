#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <chrono>
#include <thread>
#include <vector>

// 共享内存结构体
struct SharedData {
    int values[5];
    pid_t last_writer;
};

// 创建并初始化共享内存
SharedData* create_shared_memory() {
    // 创建共享内存
    SharedData* shared_data = (SharedData*)mmap(
        NULL, sizeof(SharedData), 
        PROT_READ | PROT_WRITE, 
        MAP_SHARED | MAP_ANONYMOUS, 
        -1, 0);
    
    if (shared_data == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }
    
    // 初始化共享内存
    for (int i = 0; i < 5; i++) {
        shared_data->values[i] = 0;
    }
    shared_data->last_writer = 0;
    
    return shared_data;
}

// 这是我们将在其前后设置断点的函数
void create_processes(int num_processes) {
    std::cout << "准备创建 " << num_processes << " 个子进程..." << std::endl;
    
    // 创建共享内存
    SharedData* shared_data = create_shared_memory();
    
    // 存储子进程ID
    std::vector<pid_t> child_pids;
    
    // 创建管道用于父子进程通信
    int pipes[num_processes][2];
    for (int i = 0; i < num_processes; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe failed");
            exit(1);
        }
    }
    
    // 创建多个子进程
    for (int i = 0; i < num_processes; i++) {
        pid_t pid = fork();
        
        if (pid < 0) {
            // 创建失败
            perror("fork failed");
            exit(1);
        } else if (pid == 0) {
            // 子进程代码
            close(pipes[i][0]); // 关闭读端
            
            // 子进程ID (不是真正的PID，只是一个编号)
            int process_id = i;
            
            std::cout << "子进程 " << process_id << " 启动，实际PID: " << getpid() << std::endl;
            
            // 每个子进程执行一些工作
            for (int j = 0; j < 5; j++) {
                // 更新共享内存
                shared_data->values[process_id] += 10;
                shared_data->last_writer = getpid();
                
                // 向父进程发送消息
                char message[100];
                sprintf(message, "子进程 %d (PID: %d) 完成了第 %d 轮工作", 
                    process_id, getpid(), j + 1);
                
                write(pipes[i][1], message, strlen(message) + 1);
                
                // 模拟工作
                std::this_thread::sleep_for(std::chrono::seconds(2));
            }
            
            std::cout << "子进程 " << process_id << " (PID: " << getpid() << ") 即将退出" << std::endl;
            close(pipes[i][1]); // 关闭写端
            
            // 解除共享内存映射
            munmap(shared_data, sizeof(SharedData));
            
            // 子进程结束
            exit(0);
        } else {
            // 父进程代码
            close(pipes[i][1]); // 关闭写端
            child_pids.push_back(pid); // 存储子进程ID
        }
    }
    
    std::cout << "已创建 " << num_processes << " 个子进程，它们正在后台运行" << std::endl;
    
    // 父进程继续处理一些事情
    std::cout << "父进程继续执行..." << std::endl;
    
    // 从管道读取子进程发送的消息
    fd_set read_fds;
    char buffer[100];
    
    // 跟踪活跃管道数量
    int active_pipes = num_processes;
    bool pipe_active[num_processes];
    for (int i = 0; i < num_processes; i++) {
        pipe_active[i] = true;
    }
    
    // 读取所有子进程的消息
    while (active_pipes > 0) {
        // 准备select调用
        FD_ZERO(&read_fds);
        int max_fd = 0;
        
        for (int i = 0; i < num_processes; i++) {
            if (pipe_active[i]) {
                FD_SET(pipes[i][0], &read_fds);
                if (pipes[i][0] > max_fd) {
                    max_fd = pipes[i][0];
                }
            }
        }
        
        // 等待任何管道有数据可读
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        
        int ready = select(max_fd + 1, &read_fds, NULL, NULL, &timeout);
        
        if (ready == -1) {
            perror("select failed");
            break;
        } else if (ready == 0) {
            // 超时，继续循环
            continue;
        }
        
        // 检查哪些管道有数据
        for (int i = 0; i < num_processes; i++) {
            if (pipe_active[i] && FD_ISSET(pipes[i][0], &read_fds)) {
                // 从管道读取数据
                int bytes_read = read(pipes[i][0], buffer, sizeof(buffer) - 1);
                
                if (bytes_read <= 0) {
                    // 管道已关闭或出错
                    pipe_active[i] = false;
                    active_pipes--;
                    close(pipes[i][0]);
                } else {
                    // 处理收到的消息
                    buffer[bytes_read] = '\0';
                    std::cout << "父进程收到消息: " << buffer << std::endl;
                    
                    // 显示共享内存状态
                    std::cout << "共享内存状态 - 最后写入者: " << shared_data->last_writer << std::endl;
                    std::cout << "共享内存值: ";
                    for (int j = 0; j < 5; j++) {
                        std::cout << shared_data->values[j] << " ";
                    }
                    std::cout << std::endl;
                }
            }
        }
    }
    
    // 等待所有子进程结束
    for (pid_t child_pid : child_pids) {
        int status;
        std::cout << "等待子进程 " << child_pid << " 结束..." << std::endl;
        waitpid(child_pid, &status, 0);
        if (WIFEXITED(status)) {
            std::cout << "子进程 " << child_pid << " 已正常结束，退出码: " << WEXITSTATUS(status) << std::endl;
        } else {
            std::cout << "子进程 " << child_pid << " 异常终止" << std::endl;
        }
    }
    
    // 解除共享内存映射
    munmap(shared_data, sizeof(SharedData));
    
    std::cout << "所有子进程已结束" << std::endl;
}

int main() {
    std::cout << "程序启动，主进程PID: " << getpid() << std::endl;
    
    // 调用创建进程的函数
    create_processes(3);
    
    std::cout << "程序即将结束" << std::endl;
    return 0;
} 