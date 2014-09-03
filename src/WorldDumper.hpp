#ifndef CODEDUMPER_HPP
#define	CODEDUMPER_HPP

#include "common.hpp"
#include "GameManager.hpp"
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
        
        virtual void dumpObject(IObject* obj) = 0;
        
        virtual string getTimestamp() const;
    };
    
    class DotDumper : public WorldDumper {
    public:
        virtual void dumpObject(IObject* obj);

        virtual string startDump();
        virtual void endDump();
        
        virtual IPropertyStorage& have(int& prop, string id, string desc, bool editable);
        virtual IPropertyStorage& have(string& prop, string id, string desc, bool editable);
        virtual IPropertyStorage& have(bool& prop, string id, string desc, bool editable);

    private:
        ofstream file;
        map<objId, string> objects;
    };
}

#endif

