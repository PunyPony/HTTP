#pragma once
#include <mutex>
#include <iostream>
#include <fstream>
class SynchronizedFile {
public:
    SynchronizedFile (const std::string& path) 
     :path_(path)
    {
        // Open file for writing...
        std::ofstream os_(path);
        os_.open(path_);
    }

    int write (const std::string& dataToWrite) {
        // Ensure that only one thread can execute at a time
        std::lock_guard<std::mutex> lock(writerMutex_);
        // Write to the file...
        os_.open(path_, std::ios::out | std::ios::app);
        os_ << dataToWrite;
        os_.close();
        return 0;
    }

    ~SynchronizedFile()
    {
    } 

private:
    std::string path_;
    std::ofstream os_;
    std::mutex writerMutex_;
};
