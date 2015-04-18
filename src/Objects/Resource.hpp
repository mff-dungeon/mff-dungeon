#ifndef RESOURCE_HPP
#define	RESOURCE_HPP

#include "../common.hpp"
#include "../Utils/RandomString.hpp"
#include "../Utils/FuzzyStringMatcher.hpp"
#include "Traps/Trap.hpp"
#include "Item.hpp"

namespace Dungeon {

	/**
	 * Resources of the same type get summed together when picked.
	 */
	class Resource : public Item {
	public:

		/**
		 * Determines the type of the resource.
		 * Mana shard should always be the last as many iterations depend on it
		 * Gold and Mana shards not used yet
		 */
		enum ResourceType {
			BEGIN = 0,
			Gold = BEGIN,
			Wood = 1,
			Iron = 2,
			DragonSkin = 3,
			Leather = 4,
			Sand = 5,
			WhitePowder = 6,
			RedPowder = 7,
			MagicalStone = 8,
			ManaShard = 9,
			END // do not use, for iteration purposes only!
		};

        static const int Count = ResourceType::END;

		static const char* ResourceName[];
		static const char* ResourceIdentifier[];
		Resource() { }
		Resource(const objId& id) : Item(id) { }
		Resource(ResourceType type, int quantity) : Item(getResourceTypeId(type)), resourceType(type), quantity(quantity) { }
		virtual ~Resource() { }

		ResourceType getType() const;
		Resource* setType(ResourceType type);

		int getQuantity() const;
		Resource* setQuantity(int quantity);
		/**
		 * Splits the resource into two items and returns pointer to the new one
         * @param secondAmount the amount of the resource in the second part
         * @return a new resource, or nullptr, if the split doesn't make sense
         */
		ObjectPointer split(int secondAmount);

		ObjectPointer join(ObjectPointer other);

		virtual string getDescription() const;
		virtual string getName() const;
		virtual string getLongName() const;

		virtual int getWeight() const;
		virtual Item* setWeight(int weight);
		virtual int getSize() const;
		virtual Item* setSize(int size);

		virtual void registerProperties(IPropertyStorage& storage);
		static inline std::string getResourceAmountWord(int amount) {
			if (amount > 10000) {
				return (RandomString::get() << "huge shitload" << endr
						<< "worth a fortune" << endr
						<< "good supply" << endr
						<< "gazillion" << endr);
			} else if (amount > 9000) {
				return "OVER NINE THOUSAND!";
			} else if (amount > 5000) {
				return (RandomString::get() << "a shitload" << endr
						<< "small warehouse worth" << endr
						<< "rich" << endr
						<< "enough to piss off others" << endr);
			} else if (amount > 1000) {
				return (RandomString::get() << "medium supply" << endr
						<< "good to go" << endr
						<< "no problemo" << endr
						<< "enough to mess around" << endr);
			} else if (amount > 500) {
				return (RandomString::get() << "low supply" << endr
						<< "will last a while" << endr
						<< "don't worry about it now" << endr
						<< "okay" << endr);
			} else if (amount > 100) {
				return (RandomString::get() << "enough for a while" << endr
						<< "enough to survive" << endr
						<< "bare minimum" << endr
						<< "not much" << endr);
			} else if (amount > 0) {
				return (RandomString::get() << "almost depleted" << endr
						<< "really not much" << endr
						<< "poor" << endr
						<< "scarse" << endr);
			} else if (amount == 0) {
				return (RandomString::get() << "zero" << endr
						<< "zilch" << endr
						<< "null" << endr
						<< "nil" << endr
						<< "not" << endr
						<< "nothing" << endr
						<< "nada" << endr
						<< "empty" << endr);
			} else {
				return (RandomString::get() << "impossibru" << endr
						<< "wait what?!" << endr
						<< "something wrong" << endr
						<< "you've broken physics" << endr
						<< "keep calm and nuke it from orbit" << endr);
			}
		}

            static inline FuzzyStringMatcher<ResourceType> resourceTypeMatcher() {
                FuzzyStringMatcher<ResourceType> fsm;
                for (int it = ResourceType::BEGIN; it != ResourceType::END; it++)
                    fsm.add(ResourceName[it], (ResourceType) it);
                return fsm;
            }

            virtual ObjectPointer onPick(ActionDescriptor* ad);
            virtual ObjectPointer onDrop(ActionDescriptor* ad);


	private:
		ResourceType resourceType = Resource::Gold;
		int quantity = 0;

		objId getResourceTypeId(ResourceType type) const;

		PERSISTENT_DECLARATION(Resource, Item)
	};

}

#endif	/* RESOURCE_HPP */

