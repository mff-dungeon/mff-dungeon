#include "SentenceJoiner.hpp"
#include "../Objects/Virtual/IDescriptable.hpp"

namespace Dungeon {
	
	SentenceJoiner& SentenceJoiner::operator<<(ObjectPointer value)
	{
		if (value->instanceOf(IDescriptable))
			return *this << value.unsafeCast<IDescriptable>()->getName();
		else
			return *this  << value.getId();
	}
	
	

}