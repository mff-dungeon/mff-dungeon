#include "Logger.hpp"
#include <algorithm>

namespace Dungeon {

	Logger::Logger() : ostream(NULL) {
		mutex_lock.lock();

		// link logger with log stream
		ostream::rdbuf(&buffer);

		// output all messages to stdout
		this->linkStream(cout, Severity::Info);
		// open file stream with "append at the end" flag and make it copy stdout
		stdoutFile.open(Utils::currentTime("%Y-%m-%d") + "_stdout.log", fstream::out | fstream::app | fstream::ate);
		this->linkStream(stdoutFile, Severity::Info);

		// open file stream with "append at the end" flag and make it receive all messages
		verboseFile.open(Utils::currentTime("%Y-%m-%d") + "_verbose.log", fstream::out | fstream::app | fstream::ate);
		this->linkStream(verboseFile, Severity::Verbose);

		// open file stream with "append at the end" flag and make it receive warnings or worse
		warningsFile.open(Utils::currentTime("%Y-%m-%d") + "_warnings.log", fstream::out | fstream::app | fstream::ate);
		this->linkStream(warningsFile, Severity::Warning);

		mutex_lock.unlock();
	}

	Logger::~Logger() {
		// write new line into log and flush
		*this << endl;

		// close the log file
		stdoutFile.close();
		verboseFile.close();
		warningsFile.close();
	}

	string Logger::getSeverityName(Severity severity) {
		switch (severity) {
			case Severity::Debug:
				return "debug";
				
			case Severity::Verbose:
				return "verbose";

			case Severity::Info:
				return "info";

			case Severity::Warning:
				return "warning";

			case Severity::Error:
				return "error";

			case Severity::Fatal:
				return "fatal";

			default:
				return "unknown";
		}
	}

	void Logger::linkStream(std::ostream& stream, Severity minSeverity) {
		stream.flush();
		stream << "[" << this->getTimestamp() << "] Logging began with minimal severity level: ";
		stream << getSeverityName(minSeverity) << endl;

		buffer.addBuffer(stream.rdbuf(), minSeverity);
	}

	bool Logger::setMinSeverity(ostream& stream, Severity minSeverity) {
		mutex_lock.lock();
		bool result = false;

		if (buffer.setBufferSeverity(stream.rdbuf(), minSeverity)) {
			stream.flush();
			stream << "[" << this->getTimestamp() << "] Changed minimal log severity level to: ";
			stream << getSeverityName(minSeverity) << endl;

			result = true;
		}

		mutex_lock.unlock();

		if (stream.rdbuf() == cout.rdbuf()) {
			// stdout file should reflect what is written into stdout
			this->setMinSeverity(stdoutFile, minSeverity);
		}

		return result;
	}

	bool Logger::unlinkStream(ostream& stream) {
		mutex_lock.lock();
		bool result = false;

		if (buffer.removeBuffer(stream.rdbuf())) {
			stream.flush();
			stream << "[" << this->getTimestamp() << "] Logging ended." << endl << endl;

			result = true;
		}

		mutex_lock.unlock();
		return result;
	}

	void Logger::LogBuffer::addBuffer(streambuf* buf, Severity minSeverity) {
		bufs.insert(pair<streambuf*, Severity>(buf, minSeverity));
	}

	bool Logger::LogBuffer::setBufferSeverity(streambuf* buf, Severity minSeverity) {
		LogBufferList::iterator it = bufs.find(buf);

		if (it == bufs.end())
			return false;

		it->second = minSeverity;
		return true;
	}

	bool Logger::LogBuffer::removeBuffer(streambuf* buf) {
		LogBufferList::iterator it = bufs.find(buf);

		if (it == bufs.end())
			return false;

		bufs.erase(it);
		return true;
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
        
        void Logger::writeMessage(const string source, const Severity severity, const ostream& data) {
            // atomic operation
            mutex_lock.lock();
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
            
            // print the actual message
            *this << data.rdbuf();
            
            // print new line and flush
            *this << endl;

            // file streams need to be flushed separately
            stdoutFile.flush();
            verboseFile.flush();
            warningsFile.flush();

            // end atomic operation
            mutex_lock.unlock();
        }

	string Logger::getTimestamp() {
		return Utils::currentTime("%d/%m %H:%M:%S");
	}

	void Logger::setHeadline(string title) {
		// headline is actually just a fancy message
		LOG("") << "------------------------------------------------=[ " << title << " ]" << LOGF;
	}
}
