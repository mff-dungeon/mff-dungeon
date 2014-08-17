#ifndef IDESCRIPTABLE_HPP
#define	IDESCRIPTABLE_HPP

#include "../common.hpp"

namespace Dungeon {
    /**
     * An object with a name and some description. 
     * In fact, every object should be descriptable, 
     * but the functionality is divided better this way.
     */

    class IDescriptable : public IObject {
    public:
        IDescriptable() : IObject() {}
        IDescriptable(objId id) : IObject(id) {}
        IDescriptable(const IDescriptable& other) : name(other.name), longName(other.longName), description(other.description) {}        

	virtual string getDescription() const;
	IDescriptable* setDescription(string description);

	virtual string getLongName() const;
	IDescriptable* setLongName(string longName);

	virtual string getName() const;
	IDescriptable* setName(string name);

        virtual bool isDescriptable() const;

        
        virtual void serialize(Archiver& stream);

        
    private:
        string name, longName;
        string description;
    };
}

#endif

