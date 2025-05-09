#include "ThreadPool.hpp"
#include "chrono"
void test_func(int id){
    std::cout << "Thread " << std::this_thread::get_id()<< " is working on task: " << id << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // 模拟工作 100太快改为200
    }

    int main(){
        ThreadPool pool(10,20);
        // std::this_thread::sleep_for(std::chrono::milliseconds(200)); // 等线程初始化完 没有卵用啊
        for(auto i=0;i<30;i++){
            int task_id=i;
            pool.AddTask(Task([task_id](){test_func(task_id);}));
        }
        std::this_thread::sleep_for(std::chrono::seconds(10));
        std::cout << "Busy threads: " << pool.GetBusyNum() << std::endl;
        std::cout << "Alive threads: " << pool.GetAliveNum() << std::endl;
        return 0;
    }