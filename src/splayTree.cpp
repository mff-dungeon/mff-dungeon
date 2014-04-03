#include "iObject.hpp"
#include "splayTree.hpp"

namespace Dungeon {
	
	/*
	 * Initialize tree, root is empty pointer
	 */
	SplayTree::SplayTree() {
		mroot = 0;
	};

	/*
	 * Right rotation
	 * Node n - node which will be rotated right
	 */
	void SplayTree::rotateRight(Node* n) {
		Node* t = n->left;
		n->left = t->right;
		if(t->right != 0) t->right->parent = n;
		t->parent = n->parent;
		if(n->parent == 0) this->mroot = t;
		else if(n == n->parent->left) n->parent->left = t;
		else n->parent->right = t;
		t->right = n;
		n->parent = t;
	};

	/*
	 * Left rotation
	 * Node n - node which will be rotated left
	 */
	void SplayTree::rotateLeft(Node* n) {
		Node* t = n->right;
		n->right = t->left;
		if(t->left != 0) t->left->parent = n;
		t->parent = n->parent;
		if(n->parent == 0) this->mroot = t;
		else if(n == n->parent->left) n->parent->left = t;
		else n->parent->right = t;
		t->left = n;
		n->parent = t;
	};

	/* 
	 * Splay the tree - move the Node n to the root of the tree
	 */
	void SplayTree::splay(Node* n) {
		while(n->parent != 0) {			// do while n is not root
			if(n->parent->parent == 0) {
				if(n->parent->left == n) {
					rotateRight(n->parent);
				} else {
					rotateLeft(n->parent);
				}
			}
			else if(n->parent->left == n && n->parent->parent->left == n->parent) {
				rotateRight(n->parent->parent);
				rotateRight(n->parent);
			}
			else if(n->parent->right == n && n->parent->parent->right == n->parent) {
				rotateLeft(n->parent->parent);
				rotateLeft(n->parent);
			}
			else if(n->parent->right == n && n->parent->parent->left == n->parent) {
				rotateLeft(n->parent);
				rotateRight(n->parent);
			}
			else {
				rotateRight(n->parent);
				rotateLeft(n->parent);
			}
		}
	};

	/* 
	 * Replaces Node u with Node v and preserves the pointers
	 *	so Node u can be deleted
	 */
	void SplayTree::replace(Node* u, Node* v) {
		if(u->parent == 0) this->mroot = v;
		else if(u == u->parent->left) u->parent->left = v;
		else u->parent->right = v;
		if(v != 0) v->parent = u->parent;
	};

	/* 
	 * Finds the minimal Node in a subtree with Node root 
	 */
	SplayTree::Node* SplayTree::findMinimum(Node* root) {
		while(root->left != 0)
			root = root->left;
		return root;
	}

	/* 
	 * Insert IObject obj into the tree
	 */
	void SplayTree::insert(IObject* obj) {
		Node* f = this->mroot;
		Node* p = 0;
		while(f != 0) {
			p = f;
			if(f->value->getId() == obj->getId()) {
				// throw error - already in the tree
				return;
			}
			else if(f->value->getId() < obj->getId()) {
				f = f->right;
			}
			else {
				f = f->left;
			}
		}
		f = new Node;
		f->value = obj;
		f->parent = p;	
		if(p == 0)
			this->mroot = f;
		else if(f->value->getId() < p->value->getId())
			p->left = f;
		else
			p->right = f;

		this->splay(f);
	};

	/* 
	 * Finds and returns the object in the tree with id = int id
	 */
	IObject* SplayTree::find(int id) {
		Node* f = this->mroot;
		while(f != 0) {
			if(f->value->getId() == id) 
				{
					this->splay(f);
					return f->value;
				}
			else if(f->value->getId() < id)
				f = f->right;
			else
				f = f->left;
		}
		return 0;
	};

	/* 
	 * Removes node with IObject having id = int id
	 */
	void SplayTree::remove(int id) {
		Node* f = this->mroot;
		while(f != 0 && f->value->getId() != id) {
			if(f->value->getId() < id)
				f = f->right;
			else
				f = f->left;
		}
		
		if(f == 0) return;

		if(f->left == 0)
			this->replace(f, f->right);
		else if(f->right == 0)
			this->replace(f, f->left);
		else {
			Node *t = this->findMinimum(f->right);
			if(t->parent != f) {
				this->replace(t, t->right);
				t->right = f->right;
				t->right->parent = t;
			}
			replace(f, t);
			t->left = f->left;
			t->left->parent = t;
		}

		delete f;
	};
}