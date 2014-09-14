#include "common.hpp"

#ifndef LOGGER_HPP
#define	LOGGER_HPP

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
         
       /** Starts outputting log messages into a new stream.
         @param stream      Stream to which the log will be written.
         @param minSeverity All messages below this severity level will be discarded.
         */
        void linkStream(ostream& stream, Severity minSeverity);
        
        /** Changes severity filter for log output stream.
         @param stream      Log output stream added using {@link #linkStream} method.
         @param minSeverity The new severity filter. All messages below this severity level will be discarded.
         @return True if the operation succeeded.
         */
        bool setMinSeverity(ostream& stream, Severity minSeverity);
        
        /** Stops outputting log messages into the stream.
         @param stream      Log output stream added using {@link #linkStream} method.
         @return True if the operation succeeded.
         */
        bool unlinkStream(ostream& stream);
        
    private:
        Logger();
        Logger(Logger const&);
        ~Logger();
        void operator=(Logger const&);
        
        string getSeverityName(Severity severity);
        
        typedef map<streambuf*, Severity> LogBufferList;
        
        class LogBuffer : public streambuf {
        public:
            void addBuffer(streambuf* buf, Severity minSeverity);
            bool setBufferSeverity(streambuf* buf, Severity minSeverity);
            bool removeBuffer(streambuf* buf);
            virtual int overflow(int c);
            
            Severity currentSeverity;
            
        private:
            LogBufferList bufs;
        };
        
        LogBuffer buffer;
        string currentTime(string format);
        
        string getTimestamp();
        mutex mutex_lock;
        
        ofstream stdoutFile, verboseFile, warningsFile;
    };
}

#endif /* LOGGER_HPP */