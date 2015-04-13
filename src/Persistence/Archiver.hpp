#include "IPropertyStorage.hpp"
#include <string>
#include <sstream>

#ifndef ARCHIVER_HPP
#define	ARCHIVER_HPP

namespace Dungeon {

	/*
	 * Class implementing the archivation of objects into a std::stringstream
	 * Holds private std::stringstream, which is converted to std::string when required by
	 *	ObjectLoader.
	 * Doesn't inherit stringstream ownership, only holds a reference (pointer) to it
	 * Defines operator << and >> for all supported data types to define 
	 *  converting to different data types.
	 * Storing variable defines the direction - if true, Archiver is saving 
	 * 
	 * TODO - rewrite it so directions aren't necessary using referenced parameters
	 */
	class Archiver : public IPropertyStorage {
	private:
		bool storing;
		std::stringstream* stream;
	public:
		Archiver(std::stringstream* stream, bool isStoring = true);
		virtual ~Archiver();

		void write(const void* buffer, size_t length);
		void read(void* buffer, size_t length);

		Archiver& operator<<(const std::string& str);
		Archiver& operator>>(std::string& str);

		Archiver& operator<<(int val);
		Archiver& operator>>(int& val);

		Archiver& operator<<(long val);
		Archiver& operator>>(long& val);

		Archiver& operator<<(bool b);
		Archiver& operator>>(bool& b);

		virtual IPropertyStorage& have(int& prop, const string& id, const string& desc, bool editable = true);
		virtual IPropertyStorage& have(long& prop, const string& id, const string& desc, bool editable = true);
		virtual IPropertyStorage& have(std::string& prop, const string& id, const string& desc, bool editable = true);
		virtual IPropertyStorage& have(bool& prop, const string& id, const string& desc, bool editable = true);

		bool isStoring() const;
		void setDirection(bool isStoring);

		std::string printStream();
	};
}
#endif

