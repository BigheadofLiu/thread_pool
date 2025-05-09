#pragma once
#include<thread>
#include<vector>
#include<mutex>
#include<iostream>
#include<condition_variable>
#include<memory.h>
#include<atomic>
#include"TaskQueue.hpp"
//1.原版
// class ThreadPool
// {
// private:
//     /* data */
//     int m_min_num; //最小线程数
//     int m_max_num;  //最大线程数

//     int m_busy_num; //工作线程数
//     int m_alive_num; //存活线程数
//     int m_exit_num;  //需杀死线程数
//     bool m_shutdown=false; //线程池状态
//     // pthread_t* m_threadids; //用于指向线程数组？
//     // pthread_t* m_managerids; //用于指向管理者线程数组
//     std::thread m_manager_thread; //管理者线程 1个
//     std::vector<std::thread> m_work_thread; //工作线程 N个 使用vector存储
//     std::mutex m_mutex; //互斥量
//     std::condition_variable not_empty; //条件变量：不满
//     // std::condition_variable not_full;//条件变量：不空 暂时用不到
//     TaskQueue* m_taskQ; //任务队列
//     void worker(/* void* args */); //工作函数（消费者）
//     void manager(/* void* args */); //管理者函数
//     void thread_exit(); //线程退出函数
// public:
//     ThreadPool(int,int);
//     ~ThreadPool();
//     void add_task(Task task);
//     int get_alive_num();
//     int get_busy_num();

// };

//2.改cpp11版本
class ThreadPool{
    private:
    int m_min_num;
    int m_max_num;
    std::atomic<int> m_alive_num;
    std::atomic<int> m_busy_num;
    std::atomic<int> m_exit_num;
    // std::atomic<bool> m_shutdown; //是否为互斥量？
    bool m_shutdown;
    std::thread manager_thread;
    std::vector<std::thread> work_thread;
    std::mutex m_mutex;
    std::condition_variable not_empty;
    std::unique_ptr<TaskQueue> m_taskQ;  //使用智能指针管理 TaskQueue* m_taskQ裸指针
    void work();
    void manage();
    void thread_exit();

    public:
        ThreadPool();
        ThreadPool(int,int);
        ~ThreadPool();
        void AddTask(const Task&);
        int GetAliveNum();
        int GetBusyNum();
};
