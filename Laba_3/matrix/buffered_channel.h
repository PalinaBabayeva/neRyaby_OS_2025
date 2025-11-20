#ifndef BUFFERED_CHANNEL_H_
#define BUFFERED_CHANNEL_H_

#include <queue>
#include <mutex>
#include <condition_variable>
#include <stdexcept>
#include <atomic>

template<class T>
class BufferedChannel {
public:
    explicit BufferedChannel(size_t size) : buffer_size(size), closed(false) {}

    void Send(T value) {
        std::unique_lock<std::mutex> lock(mtx);
        
        if (closed) {
            throw std::runtime_error("Cannot send to closed channel");
        }
        
        send_cv.wait(lock, [this]() { 
            return queue.size() < buffer_size || closed; 
        });
        
        if (closed) {
            throw std::runtime_error("Cannot send to closed channel");
        }
        
        queue.push(std::move(value));
        recv_cv.notify_one();
    }

    std::pair<T, bool> Recv() {
        std::unique_lock<std::mutex> lock(mtx);
        
        recv_cv.wait(lock, [this]() { 
            return !queue.empty() || closed; 
        });
        
        if (!queue.empty()) {
            T value = std::move(queue.front());
            queue.pop();
            send_cv.notify_one();
            return {std::move(value), true};
        }
        
        return {T(), false};
    }

    void Close() {
        std::unique_lock<std::mutex> lock(mtx);
        closed = true;
        send_cv.notify_all();
        recv_cv.notify_all();
    }

    size_t Size() const {
        std::unique_lock<std::mutex> lock(mtx);
        return queue.size();
    }

    bool IsClosed() const {
        std::unique_lock<std::mutex> lock(mtx);
        return closed && queue.empty();
    }

private:
    std::queue<T> queue;
    const size_t buffer_size;
    bool closed;
    
    mutable std::mutex mtx;
    std::condition_variable send_cv;
    std::condition_variable recv_cv;
};

#endif // BUFFERED_CHANNEL_H_