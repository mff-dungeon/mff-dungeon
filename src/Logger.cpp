#include "Logger.hpp"
#include <algorithm>

namespace Dungeon {
    Logger::Logger() : ostream(NULL) {
        // link logger with log stream
        ostream::rdbuf(&buffer);
        
        // output all messages to stdout
        this->linkStream(cout);
        
        // generate log file name: yyyy-mm-dd_hh-mm-ss_stdout.log
        string logname = this->currentTime("%Y-%m-%d_%H-%M-%S") + "_stdout.log";
        
        // open file stream with "append at the end" flag and make it copy stdout
        logfile.open(logname, fstream::out | fstream::app | fstream::ate);
        this->linkStream(logfile);
    }
    
    Logger::~Logger() {
        // write new line into log and flush
        *this << endl;
        this->flush();
        
        // close the log file
        logfile.close();
    }
    
    void Logger::linkStream(std::ostream& stream) {
        stream.flush();
        buffer.addBuffer(stream.rdbuf());
    }
    
    void Logger::LogBuffer::addBuffer(streambuf* buf) {
        bufs.push_back(buf);
    }
    
    int Logger::LogBuffer::overflow(int c) {
        for_each(bufs.begin(), bufs.end(), bind2nd(mem_fun(&streambuf::sputc), c));
        return c;
    }
    
    void Logger::beginMessage(string source, Severity severity) {
        // atomic operation
        mutex.lock();
        
        // print timestamp
        *this << "[" << (*this).getTimestamp() << "] ";
        
        // print severity level if needed
        switch (severity) {
            case Severity::Warning:
                *this << "WARNING: ";
                break;
                
            case Severity::Error:
                *this << "ERROR: ";
                break;
                
            case Severity::Fatal:
                *this << "FATAL: ";
                break;
                
            default:
                break;
        }
        
        // print message source if needed
        if (source != "") {
            *this << source << ": ";
        }
    }
    
    void Logger::endMessage() {
        // print new line and flush
        *this << endl;
        this->flush();
        
        // file streams need to be flushed separately
        logfile.flush();
        
        // end atomic operation
        mutex.unlock();
    }
    
    string Logger::getTimestamp() {
        return this->currentTime("%d/%m %H:%M:%S");
    }
    
    string Logger::currentTime(string format) {
        std::time_t t = std::time(NULL);
        char mbstr[100];
        std::strftime(mbstr, sizeof(mbstr), format.c_str(), std::localtime(&t));
        
        return std::string(mbstr);
    }
    
    void Logger::setHeadline(string title) {
        // headline is actually just a fancy message
        LOG("") << "------------------------------------------------=[ " << title << " ]" << LOGF;
    }
}