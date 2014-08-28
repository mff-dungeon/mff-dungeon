#include <string>
#include <sstream>

#ifndef ARCHIVER_HPP
#define	ARCHIVER_HPP

using namespace std;


namespace Dungeon {

    /*
     * Class implementing the archivation of objects into a stringstream
     * Holds private stringstream, which is converted to string when required by
     *	ObjectLoader.
     * Defines operator << and >> for all data types to define converting to 
     *  different data types.
     * Storing variable defines the direction - if true, Archiver is saving 
     * 
     * TODO - rewrite it so directions aren't necessary using referenced parameters
     */
    class Archiver {
    private:
            bool storing;
            stringstream* stream;
    public:
            Archiver(stringstream* stream, bool isStoring=true);
            virtual ~Archiver();

            void write(const void* buffer, size_t length);
            void read(void* buffer, size_t length);

            Archiver& operator<<(const string& str);
            Archiver& operator>>(string& str);

            Archiver& operator<<(int val);
            Archiver& operator>>(int& val);

            Archiver& operator<<(bool b);
            Archiver& operator>>(bool& b);

            bool isStoring() const;
            void setDirection(bool isStoring);

            string printStream();
    };
}
#endif

