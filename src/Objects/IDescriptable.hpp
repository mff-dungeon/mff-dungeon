#ifndef IDESCRIPTABLE_HPP
#define	IDESCRIPTABLE_HPP

#include "../common.hpp"

namespace Dungeon {
    
    /**
     * An object with a name and some description. 
     * In fact, every object should be descriptable, 
     * but the functionality is split better this way.
     */
    class IDescriptable : public IObject {
    public:
        IDescriptable() : IObject() {}
        IDescriptable(objId id) : IObject(id) {}
        IDescriptable(const IDescriptable& other) : name(other.name), longName(other.longName), description(other.description) {}        

        /**
         * Description string. Will be displayed hile exploring room.
         */
        virtual string getDescription() const;
        IDescriptable* setDescription(string description);

        /**
         * Long name is used mainly for matching.
         */
        virtual string getLongName() const;
        IDescriptable* setLongName(string longName);

        /**
         * Short name is used when there are many things 
         * and we need to spare some bytes :)
         */
        virtual string getName() const;
        IDescriptable* setName(string name);

        /**
         * Simpler "instanceof" detection
         */
        virtual bool isDescriptable() const;
        
        /**
         * Describes this item in explore reply
         */
        virtual string getDescriptionSentence();
        
        /**
         * When there are more instances of this class, 
         * we can produce better summary.
         */
        virtual string getGroupDescriptionSentence(vector<IDescriptable *> others);
        
        virtual void serialize(Archiver& stream);

        
    private:
        string name, longName;
        string description;
    };
}

#endif

