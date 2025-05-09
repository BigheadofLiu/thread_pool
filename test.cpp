#include "ThreadPool.hpp"
#include "chrono"
void test_func(int id){
    std::cout << "Thread " << std::this_thread::get_id()
    << " is working on task " << id << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 模拟工作
    }

    int main(){
        ThreadPool pool(2,6);
        for(auto i=0;i<20;i++){
            int task_id=i;
            pool.AddTask(Task([task_id](){test_func(task_id);}));
        }
        std::this_thread::sleep_for(std::chrono::seconds(10));
        std::cout << "Busy threads: " << pool.GetBusyNum() << std::endl;
        std::cout << "Alive threads: " << pool.GetAliveNum() << std::endl;
        return 0;
    }