#ifndef LOGGER_HPP
#define	LOGGER_HPP

#include "common.hpp"
#include <iomanip>
#include <ctime>
#include <sstream>
#include <fstream>
#include <mutex>

using namespace std;

namespace Dungeon {
    class Logger : public ostream {
    public:
        enum Severity {
            Verbose = 1,
            Info = 2,
            Warning = 4,
            Error = 8,
            Fatal = 16
        };
        
        static Logger& getInstance() {
            static Logger instance;
            return instance;
        }
        
        void beginMessage(string source = "", Severity severity = Severity::Info);
        void endMessage();
        
        void setHeadline(string title);
        
    private:
        Logger();
        Logger(Logger const&);
        ~Logger();
        void operator=(Logger const&);
        
        class LogBuffer : public streambuf {
        public:
            void addBuffer(streambuf* buf);
            virtual int overflow(int c);
            
        private:
            std::vector<streambuf*> bufs;
        };
        
        LogBuffer buffer;
        void linkStream(ostream& stream);
        
        string getTimestamp();
        mutex mutex;
        ofstream logfile;
    };
}

#endif /* LOGGER_HPP */