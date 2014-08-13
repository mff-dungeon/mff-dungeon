#include "Logger.hpp"
#include <algorithm>

namespace Dungeon {
    Logger::Logger() : ostream(NULL) {
        mutex.lock();

        // link logger with log stream
        ostream::rdbuf(&buffer);
        
        // output all messages to stdout
        this->linkStream(cout, Severity::Info);
        // open file stream with "append at the end" flag and make it copy stdout
        stdoutFile.open(this->currentTime("%Y-%m-%d_%H-%M-%S") + "_stdout.log", fstream::out | fstream::app | fstream::ate);
        this->linkStream(stdoutFile, Severity::Info);
        
        // open file stream with "append at the end" flag and make it receive all messages
        verboseFile.open(this->currentTime("%Y-%m-%d_%H-%M-%S") + "_verbose.log", fstream::out | fstream::app | fstream::ate);
        this->linkStream(verboseFile, Severity::Verbose);
        
        // open file stream with "append at the end" flag and make it receive warnings or worse
        warningsFile.open(this->currentTime("%Y-%m-%d_%H-%M-%S") + "_warnings.log", fstream::out | fstream::app | fstream::ate);
        this->linkStream(warningsFile, Severity::Warning);
        
        mutex.unlock();
    }
    
    Logger::~Logger() {
        // write new line into log and flush
        *this << endl;
        this->flush();
        
        // close the log file
        stdoutFile.close();
        verboseFile.close();
        warningsFile.close();
    }
    
    void Logger::linkStream(std::ostream& stream, Severity minSeverity) {
        stream.flush();
        
        stream << "[" << this->getTimestamp() << "] Logging began with minimal severity level: ";
        switch (minSeverity) {
            case Severity::Verbose:
                stream << "verbose";
                break;
                
            case Severity::Info:
                stream << "info";
                break;
                
            case Severity::Warning:
                stream << "warning";
                break;
                
            case Severity::Error:
                stream << "error";
                break;
                
            case Severity::Fatal:
                stream << "fatal";
                break;
                
            default:
                stream << "unknown";
                break;
        }
        stream << endl << endl;
        stream.flush();
        
        buffer.addBuffer(stream.rdbuf(), minSeverity);
    }
    
    void Logger::LogBuffer::addBuffer(streambuf* buf, Severity minSeverity) {
        bufs.insert(pair<streambuf*, Severity>(buf, minSeverity));
    }
    
    int Logger::LogBuffer::overflow(int c) {
        for (map<streambuf*, Severity>::iterator it = bufs.begin(); it != bufs.end(); ++it) {
            Severity minSeverity = it->second;
            
            if (minSeverity <= currentSeverity) {
                it->first->sputc(c);
            }
        }
        
        return c;
    }
    
    void Logger::beginMessage(string source, Severity severity) {
        // atomic operation
        mutex.lock();
        this->flush();
        
        buffer.currentSeverity = severity;
        
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
        stdoutFile.flush();
        verboseFile.flush();
        warningsFile.flush();
        
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