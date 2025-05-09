#include"ThreadPool.hpp"
// ThreadPool::ThreadPool(int min_num,int max_num){
//     m_taskQ=new TaskQueue;
//     do
//     {   m_max_num=max_num;
//         m_min_num=min_num;
//         m_busy_num=0;
//         m_alive_num=min_num;
//         /* m_work_thread.resize(m_max_num); */ //vector动态扩展 不需要这样做
//         //创建work线程
//         for(auto i=0;i<m_min_num;++i){
//             m_work_thread.emplace_back(&ThreadPool::worker,this);
//             std::cout<<"创建了一个工作线程ID为:"<<m_work_thread[i-1].get_id()<<std::endl;
//         }
//         //创建管理者线程
//         m_manager_thread=std::thread(&ThreadPool::manager,this);
//             std::cout<<"创建了一个管理者线程ID为："<<m_manager_thread.get_id();
//     } while (0);
    
// }
// ThreadPool::~ThreadPool(){
//     m_shutdown=true;
//     not_empty.notify_all();
//     for(auto& t:m_work_thread){
//             if(t.joinable()){
//                 t.join();
//             }
//     }
//     if(m_manager_thread.joinable()){
//         m_manager_thread.join();
//     }
//     delete m_taskQ;
//     std::cout<<"线程池已被销毁"<<std::endl;
// }
// void ThreadPool::add_task(Task task){
//     if (m_shutdown)
//     {
//         return;
//     }
//     std::lock_guard<std::mutex> locker(m_mutex);
//     m_taskQ->AddTask(task);
//     not_empty.notify_one();
// }
// void ThreadPool::worker(){
// while(true){
//     std::unique_lock<std::mutex> locker(m_mutex);
//     while(this->m_taskQ->get_task_num()==0&&!m_shutdown){
//         not_empty.wait(locker);
//         if(m_exit_num>0){
//             --m_exit_num;
//             if(m_alive_num>m_min_num){
//                 --m_alive_num;
//                 return;
//             }
//         }
//     }
//     if(m_shutdown){
//         return;
//     }
//     Task task=m_taskQ->TakeTask();
//     ++m_busy_num;
//     locker.unlock();
//     if(task.function){
//        task.function(nullptr);
//     }
//     locker.lock();
//     --m_busy_num;
//     }
// }
// int ThreadPool::get_busy_num(){
//     std::lock_guard<std::mutex> locker(m_mutex);
//     return m_busy_num;
// }
// int ThreadPool::get_alive_num(){
//     std::lock_guard<std::mutex> locker(m_mutex);
//     return m_alive_num;
// }
ThreadPool::ThreadPool(int min,int max)
:m_min_num(min),m_max_num(max),m_exit_num(0),m_busy_num(0),m_alive_num(min),m_taskQ(std::make_unique<TaskQueue>())
{
    m_shutdown=false; //false为默认开启状态
    for(auto i=0;i<m_min_num;++i){
        work_thread.emplace_back(&ThreadPool::work,this);
    }
    manager_thread=std::thread(&ThreadPool::manage,this);
}
ThreadPool::~ThreadPool(){
    m_shutdown=true;
    not_empty.notify_all();
    for(auto& i :work_thread){
        if(i.joinable()){
            i.join();
        }
    }
    if(manager_thread.joinable()){
        manager_thread.join();
    }
}
void ThreadPool::work(){
    Task task;
    while(true){
        std::unique_lock<std::mutex> locker(m_mutex);
        not_empty.wait(locker,[this](){return m_shutdown || m_taskQ->GetTaskNum()>0||m_exit_num>0;});

        if(m_exit_num>0){
            m_exit_num--;
            if(m_alive_num>m_min_num){
                m_alive_num--;
                return;  //线程退出
            }
        }
        if(m_shutdown&&m_taskQ->GetTaskNum()==0){
            break;
        }
        if(m_taskQ->GetTaskNum()>0){
            task=m_taskQ->TakeTask();
            m_busy_num++;
        }else{
            continue;
        }
        task(); //运行 ()运算符已重载
        m_busy_num--;
    }
    m_alive_num--;
}
void ThreadPool::manage(){
while(!m_shutdown){
    std::this_thread::sleep_for(std::chrono::seconds(5)); //每5s检测一次
    int queue_size=m_taskQ->GetTaskNum();
    int alive=m_alive_num;
    int busy=m_busy_num;

    //扩容
    if(queue_size>alive&&alive<m_max_num){
        int add_count=0;
        const int add_num=2;

        std::lock_guard<std::mutex> lock(m_mutex);
        for(auto i=0;i<m_max_num&&add_count<add_num&&m_alive_num<m_max_num;++i){
            work_thread.emplace_back(&ThreadPool::work,this);
            m_alive_num++;
            add_count++;
        }
     }
     //缩容
     if(m_busy_num*2<m_alive_num&&alive>m_min_num){
        m_exit_num=2;
        for(auto i=0;i<m_exit_num;++i){
            not_empty.notify_one();
        }
      }
    }
}
int ThreadPool::GetBusyNum(){
    return m_busy_num;
}
int ThreadPool::GetAliveNum(){
    return m_alive_num;
}
void ThreadPool::AddTask(const Task& task){
    m_taskQ->AddTask(task);
    not_empty.notify_one();
}