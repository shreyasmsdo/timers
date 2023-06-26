#include <iostream>
#include <future>
#include <chrono>
#include<thread>
#include<mutex>

std::mutex cin_mutex;

template<typename T>
void set_time_out(std::future<T>& fut, int  timeout, auto onTimeout = []{}, auto onSuccess = []{})
{
  std::thread(
      [&](int timeout)
      {
        auto start = std::chrono::steady_clock::now();
        if (fut.wait_for(std::chrono::seconds(timeout)) == std::future_status::timeout)
        {
          onTimeout();
        }
        else
        {
          onSuccess();
        }
      }, timeout)
      .detach();
}
// thread can be detaced so the thread implementation is faster
template<typename T>
auto set_time_out2(std::future<T>& fut, int  timeout, auto onTimeout = []{}, auto onSuccess = []{})
{
  return std::async(
      [&](int timeout)
      {
        auto start = std::chrono::steady_clock::now();
        if (fut.wait_for(std::chrono::seconds(timeout)) == std::future_status::timeout)
        {
          onTimeout();
        }
        else
        {
          onSuccess();
        }
      },
      timeout);
}

int main() {
    std::promise<void> promise;
    std::future<void> fut = promise.get_future();
    auto onTimeOut = []{
      std::cout << " timeout for "<<  std::this_thread::get_id() <<" \n";
    };
    auto onSuccess = []{
      std::cout << " success for "<<  std::this_thread::get_id() <<" \n";
    };
    set_time_out(fut, 2, onTimeOut, onSuccess);
    std::this_thread::sleep_for(std::chrono::seconds(1)); 
    promise.set_value();


    std::promise<void> promise2;
    std::future<void> fut2 = promise2.get_future();
    set_time_out(fut2, 2, onTimeOut, onSuccess);
    std::this_thread::sleep_for(std::chrono::seconds(3)); 
    promise2.set_value();

    std::cout << " EXITING MAIN\n";
    return 0;
}

