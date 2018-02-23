#pragma once
#include <mutex>
#include <iostream>
#include <fstream>
class SynchronizedFile {
public:
    SynchronizedFile (const std::string& path) 
    {
        // Open file for writing...
        log_file_.open(path);
    }

    int write (const std::string& dataToWrite) {
        // Ensure that only one thread can execute at a time
        std::lock_guard<std::mutex> lock(writerMutex_);
        // Write to the file...
        log_file_ << dataToWrite;
    }

    ~SynchronizedFile()
    {
        log_file_.close();
    } 

private:
    std::ofstream log_file_;
    std::mutex writerMutex_;
};