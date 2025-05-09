#pragma once
#include<queue>
#include<mutex>
#include<functional>

//1.c与cpp混合版本、尼玛晦涩难懂
/* using callback=void (*)(void*);
class Task{
    //定义任务类 需要确定任务函数和任务参数
    private:
    // void (*function)(void* args);    //函数指针 尼玛孤儿写法 根本尼玛看不懂
    callback function;  //替换写法
    void *args;     //用于保存函数参数
    //默认构造
    public:
    Task(){
        function=nullptr;
        args=nullptr;
    }//有参构造
    Task(callback fun,void* args){
        this->function=fun;
        this->args=args;
    }

};

class TaskQueue{
private:
std::queue<Task> m_queue; //任务队列容器
std::mutex queue_mutex;   //互斥量
public:
TaskQueue();
~TaskQueue();
void AddTask(Task&);//添加任务
void AddTask(callback,void*);//重载
Task TakeTask();    //取出任务
inline int get_task_num(){ // 使用内联函数 加快编译运行速度
    return m_queue.size();  //获取当前任务数量
}
}; */

//2.cpp11版本
class Task{
    private:
    std::function<void()> m_func;  //
    public:
    Task()=default;
    Task(std::function<void()>);
    void operator()() const; //重载()
};
class TaskQueue{
    public:
    TaskQueue();
    ~TaskQueue();
    void AddTask(const Task&);
    void AddTask(std::function<void()>);
    Task TakeTask();
    inline int GetTaskNum(){
        std::lock_guard<std::mutex> locker(queue_mutex);
        return m_queue.size();
    }  //内联函数需要在声明里面实现
    
    private:
    std::queue<Task> m_queue;
    std::mutex queue_mutex;
    
};