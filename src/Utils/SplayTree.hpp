#ifndef SPLAYTREE_HPP
#define	SPLAYTREE_HPP

#include <fstream>
#include <memory>
#include "../common.hpp"

namespace Dungeon {

    /**
     * Generic implementation of Splay tree.
     */
    class SplayTree {
    private:
        typedef std::shared_ptr<Base> ptr_t;
        struct Node {
            Node* left;
            Node* right;
            Node* parent;
            ptr_t value;
        };

        Node* mroot;
        void rotateLeft(Node* n);
        void rotateRight(Node* n);
        void splay(Node* n);
        void replace(Node* u, Node* v);		
        Node* findMinimum(Node *root);
		
        void printDotNull(objId key, int nullcount, std::ofstream& stream);
        void printDotVertex(Node* node, std::ofstream& stream);
    public:
        SplayTree();
        void insert(ptr_t obj);
        void remove(const objId& id);
        ptr_t find(const objId& id);
		/**
		 * Removes the whole tree and deletes all Nodes and their values
         */
		void clearTree();
        void printDotFile(std::ofstream& stream);
    };
}
#endif

