#ifndef CODEDUMPER_HPP
#define	CODEDUMPER_HPP

#include "../common.hpp"
#include "../Game/GameManager.hpp"
#include <iostream>
#include <fstream>
#include <ctime>
#include <set>

namespace Dungeon {

    class WorldDumper : public IPropertyStorage {
    public:
        WorldDumper() { }
        WorldDumper(const WorldDumper& other) { }
        virtual ~WorldDumper() { }

        /**
         * @return name of file, URL or whatever how we can find the dump
         */
        virtual string startDump() = 0;
        virtual void endDump() = 0;

        virtual void dumpObject(ObjectPointer obj) = 0;

        virtual string getTimestamp() const;
    };

    class DotDumper : public WorldDumper {
    public:
        virtual void dumpObject(ObjectPointer obj);

        virtual string startDump();
        virtual void endDump();

        virtual IPropertyStorage& have(int& prop, const string& id, const string& desc, bool editable);
        virtual IPropertyStorage& have(long& prop, const string& id, const string& desc, bool editable);
        virtual IPropertyStorage& have(string& prop, const string& id, const string& desc, bool editable);
        virtual IPropertyStorage& have(bool& prop, const string& id, const string& desc, bool editable);

    private:
        ofstream file;
        map<objId, string> objects;
    };
}

#endif

