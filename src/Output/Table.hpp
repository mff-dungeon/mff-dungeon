#ifndef TABLE_HPP
#define	TABLE_HPP

#include "../common.hpp"
#include "Output.hpp"
#include "Container.hpp"
#include <vector>

namespace Dungeon {
namespace Output {

    /**
     * Formatting helper to produce nice table.
     * More fancy features can be added later.
     */
    class Table : public Container
    {
    public:
        Table() {
            displayMode = Block;
        }
        
        Table(Table&& move) = default;
        Table& operator=(Table&& move) = default;
        virtual ~Table() {}
        
        void finishRow() {
            table.emplace_back();
            contents.swap(table.back());
        }

        virtual string plainString() const;
        virtual void xhtml(Tag * parent) const;
        
        
        static unique_ptr<Table> create() {
            return Base::create<Table>();
        }

    private:
        vector<cont_t> table;
    };

}
}

#endif

