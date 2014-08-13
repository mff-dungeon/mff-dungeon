#ifndef LOGGER_HPP
#define	LOGGER_HPP

#include "common.hpp"
#include <iomanip>
#include <ctime>
#include <sstream>
#include <fstream>
#include <mutex>
#include <map>

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
        
        /** Initializes logging
         */
        static void initialize() {
            Logger::getInstance();
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
            void addBuffer(streambuf* buf, Severity minSeverity);
            virtual int overflow(int c);
            
            Severity currentSeverity;
            
        private:
            map<streambuf*, Severity> bufs;
        };
        
        LogBuffer buffer;
        void linkStream(ostream& stream, Severity minSeverity);
        
        string currentTime(string format);
        
        string getTimestamp();
        mutex mutex;
        
        ofstream stdoutFile, verboseFile, warningsFile;
    };
}

#endif /* LOGGER_HPP */