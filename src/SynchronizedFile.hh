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
    }

    int write (const std::string& dataToWrite) {
        // Ensure that only one thread can execute at a time
        std::lock_guard<std::mutex> lock(writerMutex_);
        std::ofstream os;
        os.open(path_);

        // Write to the file...
        os << dataToWrite;
        os.close();

    }

    ~SynchronizedFile()
    {
    } 

private:
    std::string path_;
    std::mutex writerMutex_;
};
