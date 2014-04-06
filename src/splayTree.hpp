/* 
 * File:   splayTree.hpp
 * Author: salmelu
 *
 * Created on 3. duben 2014, 11:56
 */

#ifndef SPLAYTREE_HPP
#define	SPLAYTREE_HPP

#include "common.hpp"

namespace Dungeon {
	class SplayTree { 
		private:	
			struct Node {
				Node* left;
				Node* right;
				Node* parent;
				IObject* value;
			};

			Node* mroot;
			void rotateLeft(Node* n);
			void rotateRight(Node* n);
			void splay(Node* n);
			void replace(Node* u, Node* v);
			Node* findMinimum(Node *root);
		public:
			SplayTree();
			void insert(IObject* obj);
			void remove(objId id);
			IObject* find(objId id);
	};
}
#endif

