#ifndef IPROPERTYSTORAGE_HPP
#define	IPROPERTYSTORAGE_HPP

#include <string>
using namespace std;

namespace Dungeon {

    /**
     * Class should implement this interface when 
     * it's able to load & store properties.
     * 
     * Base for Archiver and Configurator.
     */
    class IPropertyStorage {
    public:
        virtual IPropertyStorage& have(int& prop, string desc, bool editable = true) = 0;
        virtual IPropertyStorage& have(string& prop, string desc, bool editable = true) = 0;
        virtual IPropertyStorage& have(bool& prop, string desc, bool editable = true) = 0;
     
    private:

    };
    
    class IStorable {
    public:
        virtual void beforeLoad(IPropertyStorage& storage) {}
        virtual void afterLoad(IPropertyStorage& storage) {}
        
        virtual void beforeStore(IPropertyStorage& storage) const {}
        virtual void afterStore(IPropertyStorage& storage) const {}
        
        virtual void registerProperties(IPropertyStorage& storage) {}
    };
}

#endif

