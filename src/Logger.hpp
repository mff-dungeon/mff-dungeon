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
    /** Central logging singleton outputs all messages to stdout and log file.
     */
    class Logger : public ostream {
    public:
        /** Severity enum.
         */
        enum Severity {
            Verbose = 1,
            Info = 2,
            Warning = 4,
            Error = 8,
            Fatal = 16
        };
        
        /** Retrieves the singleton instance
         */
        static Logger& getInstance() {
            static Logger instance;
            return instance;
        }
        
        /** Outputs timestamp of a new message and locks the mutex.
         @param source      The section of application responsible for the message.
         @param severity    The importance of the message.
         */
        void beginMessage(string source = "", Severity severity = Severity::Info);
        
        /** Flushes streams and unlocks the mutex.
         */
        void endMessage();
        
        /** Writes headline into the log.
         @param title   Title of the headline.
         */
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