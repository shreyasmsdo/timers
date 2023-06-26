# timers
void set_time_out(std::future<T>& fut, int  timeout, auto onTimeout = []{}, auto onSuccess = []{}) // timeout in seconds uses std::thread 
std::future set_time_out(std::future<T>& fut, int  timeout, auto onTimeout = []{}, auto onSuccess = []{}) // timeout in seconds uses std::aysnc
