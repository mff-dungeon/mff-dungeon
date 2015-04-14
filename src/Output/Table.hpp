#ifndef TABLE_HPP
#define	TABLE_HPP

#include "../common.hpp"
#include "Output.hpp"
#include <vector>

namespace Dungeon {
namespace Output {
    /**
     * Formatting helper to produce nice table.
     * More fancy features can be added later.
     * 
     * TODO Now works on string, should work as Output container
     */
    class Table : public Base
    {
    public:        
        Table& operator<<(const string& text) {
            row.emplace_back(text);
            return *this;
        }
        
        void finishRow() {
            vector<string> newRow;
            newRow.swap(row);
            table.push_back(move(newRow));
        }
        
        virtual string plainString() const;
        virtual void xhtml(Tag * parent) const;
        
    private:
        vector<string> row;
        vector<vector<string>> table;
    };
    
}
}

#endif

