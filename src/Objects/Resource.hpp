#ifndef RESOURCE_HPP
#define	RESOURCE_HPP

#include "../common.hpp"
#include "Item.hpp"
#include "../Traps/Trap.hpp"

namespace Dungeon {
	
        /**
         * Resources of the same type get summed together when picked.
         */
	class Resource : public Item {
	public:
		/**
		 * Determines the type of the resource.
		 */
		enum ResourceType {
                    Gold = 0,
                    Wood = 1,
                    Stone = 2,
                    Food = 3,
                    ManaShard = 4
                };
		
		Resource() {}
		Resource(objId id) : Item(id) {}
                Resource(ResourceType type, int quantity) : Item(getResourceTypeId(type)), resourceType(type), quantity(quantity) {}
		virtual ~Resource() {}
		
		ResourceType getType() const;
                Resource* setType(ResourceType type);
                string getTypeName() const;
                
                int getQuantity() const;
                Resource* setQuantity(int quantity);
                
                virtual string getDescription() const;
                virtual string getName() const;
                virtual string getLongName() const;
                
                Resource* attachSumTrap();
                
		virtual void registerProperties(IPropertyStorage& storage);
                	
	private:
		
            ResourceType resourceType;
            int quantity = 0;
            
            objId getResourceTypeId(ResourceType type) const;
		
	PERSISTENT_DECLARATION(Resource, Item)
	};
        
    class ResourceSumTrap : public Trap
	{
	public:
            ResourceSumTrap() : Trap () {}
            ResourceSumTrap(string id) : Trap (id) {}
            virtual ~ResourceSumTrap() {}

            virtual void trigger(string event, ObjectPointer target, ActionDescriptor* ad);

	private:
            

            PERSISTENT_DECLARATION(ResourceSumTrap, Trap)
	};
        
}

#endif	/* RESOURCE_HPP */

