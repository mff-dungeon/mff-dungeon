#include "Logger.hpp"

namespace Dungeon {
    Logger::Logger() : ostream(NULL) {
        ostream::rdbuf(&buffer);
        this->linkStream(cout);
        
        time_t t = time(nullptr);
        tm tm = *localtime(&t);
        
        stringstream ss;
        ss << put_time(&tm, "%Y-%m-%d_%H-%M-%S") << "_stdout.log";
        string logname = ss.str();
        
        logfile = ofstream(logname, fstream::out | fstream::app | fstream::ate);
        this->linkStream(logfile);
    }
    
    Logger::~Logger() {
        *this << endl;
        this->flush();
        
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
        mutex.lock();
        
        *this << "[" << (*this).getTimestamp() << "] ";
        
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
        
        if (source != "") {
            *this << source << ": ";
        }
    }
    
    void Logger::endMessage() {
        *this << endl;
        this->flush();
        
        mutex.unlock();
    }
    
    string Logger::getTimestamp() {
        time_t t = time(nullptr);
        tm tm = *localtime(&t);
        
        stringstream ss;
        ss << put_time(&tm, "%d/%m %H:%M:%S");
        return ss.str();
    }
    
    void Logger::setHeadline(string title) {
        LOG("") << "------------------------------------------------=[ " << title << " ]" << LOGF;
    }
}