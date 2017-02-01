/* Implement a thread safe queue */

#ifndef ThreadSafeQueue_h
#define ThreadSafeQueue_h

#include <mutex>
#include <deque>

template<typename TDataType>
class ThreadSafeQueue {
public:
  void push(TDataType data)
  {
    std::lock_guard<std::mutex> lock(mut_access_lock);
    m_deque.push_back(data);
  }
  TDataType pop()
  {
    std::lock_guard<std::mutex> lock(mut_access_lock);
    TDataType tmp = m_deque.front();
    m_deque.pop_front();
    return tmp;
  }
  TDataType at(int i)
  {
    std::lock_guard<std::mutex> lock(mut_access_lock);
    return m_deque.at(i);
  }
  bool empty()
  {
    std::lock_guard<std::mutex> lock(mut_access_lock);
    return m_deque.empty();
  }
  size_t size()
  {
    std::lock_guard<std::mutex> lock(mut_access_lock);
    return m_deque.size();
  }
private:
  std::mutex mut_access_lock;
  std::deque<TDataType> m_deque;
};

#endif // ThreadSafeQueue_h
