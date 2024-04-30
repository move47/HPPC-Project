#include <mutex>
#include <condition_variable>

class RWLock {
public:
    RWLock() : readers(0), writer(false) {}

    void readLock() {
        std::unique_lock<std::mutex> lock(mutex_);
        while (writer) {
            readerCV.wait(lock);
        }
        ++readers;
    }

    void readUnlock() {
        std::unique_lock<std::mutex> lock(mutex_);
        --readers;
        if (readers == 0) {
            writerCV.notify_one();
        }
    }

    void writeLock() {
        std::unique_lock<std::mutex> lock(mutex_);
        while (writer || readers > 0) {
            writerCV.wait(lock);
        }
        writer = true;
    }

    void writeUnlock() {
        std::unique_lock<std::mutex> lock(mutex_);
        writer = false;
        readerCV.notify_all();
        writerCV.notify_one();
    }

private:
    std::mutex mutex_;
    std::condition_variable readerCV;
    std::condition_variable writerCV;
    int readers;
    bool writer;
};
