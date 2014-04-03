/* 
 * File:   splayTree.hpp
 * Author: salmelu
 *
 * Created on 3. duben 2014, 11:56
 */

#ifndef SPLAYTREE_HPP
#define	SPLAYTREE_HPP

#include "iObject.hpp"

namespace Dungeon {
	class SplayTree { 
		private:	
			struct Node {
				Node* left = 0;
				Node* right = 0;
				Node* parent = 0;
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
			void remove(int id);
			IObject* find(int id);
	};
}
#endif	/* SPLAYTREE_HPP */

