#include"TaskQueue.hpp"
/* TaskQueue::TaskQueue(){
    // pthread_mutex_init(&queue_mutex,NULL);//cpp mutex无需初始化
}
TaskQueue::~TaskQueue(){
    // pthread_mutex_destroy(&queue_mutex); cpp同样也无需销毁
}
void TaskQueue::AddTask(Task& Task){
std::lock_guard<std::mutex> lock(queue_mutex);
m_queue.push(Task);
}
void TaskQueue::AddTask(callback func,void* args){
    std::lock_guard<std::mutex> lock(queue_mutex);
    Task task(func,args);
    m_queue.push(task);
}TaskQueue::TaskQueue(){
    // pthread_mutex_init(&queue_mutex,NULL);//cpp mutex无需初始化
}
TaskQueue::~TaskQueue(){
    // pthread_mutex_destroy(&queue_mutex); cpp同样也无需销毁
}
void TaskQueue::AddTask(Task& Task){
std::lock_guard<std::mutex> lock(queue_mutex);
m_queue.push(Task);
}
void TaskQueue::AddTask(callback func,void* args){
    std::lock_guard<std::mutex> lock(queue_mutex);
    Task task(func,args);
    m_queue.push(task);
}
Task TaskQueue::TakeTask(){
Task task;
std::lock_guard<std::mutex> lock(queue_mutex);
if(m_queue.size()>0){
    task=m_queue.front();
    m_queue.pop();
}
return task;
}
Task TaskQueue::TakeTask(){
Task task;
std::lock_guard<std::mutex> lock(queue_mutex);
if(m_queue.size()>0){
    task=m_queue.front();
    m_queue.pop();
}
return task;
} */
Task::Task(std::function<void()> func){
    m_func=func;
}
void Task::operator()()const{
    if(m_func);
    m_func();
}
TaskQueue::TaskQueue()=default;
TaskQueue::~TaskQueue()=default;
void TaskQueue::AddTask(const Task& task){
    std::lock_guard<std::mutex> locker(queue_mutex);
    m_queue.push(task);
}
void TaskQueue::AddTask(std::function<void()> func){
    Task task(func);
    std::lock_guard<std::mutex> locker(queue_mutex);
    m_queue.push(task);
}
Task TaskQueue::TakeTask(){
    Task task;
    std::unique_lock<std::mutex> locker(queue_mutex);
    if(m_queue.size()>0){
        task=m_queue.front();
        m_queue.pop();
    }
    return task;
}
// inline int TaskQueue::GetTaskNum(){
//     std::lock_guard<std::mutex> locker(queue_mutex);
//     return m_queue.size();
// }