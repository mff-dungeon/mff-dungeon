#ifndef IPROPERTYSTORAGE_HPP
#define	IPROPERTYSTORAGE_HPP

#include <string>

namespace Dungeon {

	using namespace std;
	/**
	 * Class should implement this interface when
	 * it's able to load & store properties.
	 *
	 * Base for Archiver and Configurator.
	 */
	class IPropertyStorage {
	public:
		virtual IPropertyStorage& have(int& prop, const string& id, const string& desc, bool editable = true) = 0;
		virtual IPropertyStorage& have(long& prop, const string& id, const string& desc, bool editable = true) = 0;
		virtual IPropertyStorage& have(string& prop, const string& id, const string& desc, bool editable = true) = 0;
		virtual IPropertyStorage& have(bool& prop, const string& id, const string& desc, bool editable = true) = 0;

	private:

	};

	class IStorable {
	public:
		virtual void beforeLoad(IPropertyStorage& storage) { }
		virtual void afterLoad(IPropertyStorage& storage) { }
		virtual void beforeStore(IPropertyStorage& storage) const { }
		virtual void afterStore(IPropertyStorage& storage) const { }
		virtual void registerProperties(IPropertyStorage& storage) { }
	};
}

#endif

