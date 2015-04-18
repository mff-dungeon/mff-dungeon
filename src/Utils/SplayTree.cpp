#include "SplayTree.hpp"
#include <fstream>

namespace Dungeon {

	/*
	 * Initialize tree, root is empty pointer
	 */
	SplayTree::SplayTree() {
		mroot = nullptr;
	};

	/*
	 * Right rotation
	 * Node n - node which will be rotated right
	 */
	void SplayTree::rotateRight(Node* n) {
		Node* t = n->left;
		n->left = t->right;
		if(t->right != nullptr) t->right->parent = n;
		t->parent = n->parent;
		if(n->parent == nullptr) mroot = t;
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
		if(t->left != nullptr) t->left->parent = n;
		t->parent = n->parent;
		if(n->parent == nullptr) mroot = t;
		else if(n == n->parent->left) n->parent->left = t;
		else n->parent->right = t;
		t->left = n;
		n->parent = t;
	};

	/*
	 * Splay the tree - move the Node n to the root of the tree
	 */
	void SplayTree::splay(Node* n) {
		while(n->parent != nullptr) {			// do while n is not root
			if(n->parent->parent == nullptr) {
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
		if(u->parent == nullptr) mroot = v;
		else if(u == u->parent->left) u->parent->left = v;
		else u->parent->right = v;
		if(v != nullptr) v->parent = u->parent;
	};

	/*
	 * Finds the minimal Node in a subtree with Node root
	 */
	SplayTree::Node* SplayTree::findMinimum(Node* root) {
		while(root->left != nullptr)
			root = root->left;
		return root;
	}

	/*
	 * Insert IObject obj into the tree
	 */
	void SplayTree::insert(ptr_t obj) {
		Node* f = mroot;
		Node* p = nullptr;
		if(obj == nullptr) {
			LOGS(Fatal) << "Attempted to insert a null pointer." << LOGF;
			return;
		}
		while(f != nullptr) {
			p = f;
			if(f->value->getId() == obj->getId()) {
				LOGS(Error) << "Insertion failed. Node with id " << obj->getId() << " is already in the tree." << LOGF;
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
		f->left = 0;
		f->right = 0;
		f->value = obj;
		f->parent = p;
		if(p == nullptr)
			mroot = f;
		else if(f->value->getId() < p->value->getId())
			p->left = f;
		else
			p->right = f;

		LOGS(Debug) << "Node with id " << obj->getId() << " inserted." << LOGF;
		splay(f);
	};

	/*
	 * Finds and returns the object in the tree with id = objId id
	 */
	SplayTree::ptr_t SplayTree::find(const objId& id) {
		Node* f = mroot;
		while(f != nullptr) {
			if(f->value->getId() == id)
			{
				splay(f);
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
	 * Removes node with IObject having id = objId id
	 */
	void SplayTree::remove(const objId& id) {
		Node* f = mroot;
		while(f != nullptr && f->value->getId() != id) {
			if(f->value->getId() < id)
				f = f->right;
			else
				f = f->left;
		}

		if(f == nullptr) {
			LOGS(Debug) << "Requested removing node with id " << id << ", but it wasn't found." << LOGF;
			return;
		}

		if(f->left == nullptr)
			replace(f, f->right);
		else if(f->right == nullptr)
			replace(f, f->left);
		else {
			Node *t = findMinimum(f->right);
			if(t->parent != f) {
				replace(t, t->right);
				t->right = f->right;
				t->right->parent = t;
			}
			replace(f, t);
			t->left = f->left;
			t->left->parent = t;
		}
		if (!f->value.unique())
			throw GameException("The object is strongly held somewhere else.");

		delete f;
		LOGS(Debug) << "Node with id " << id << " removed from the tree." << LOGF;
	};

	void SplayTree::clearTree() {
		LOG << "Clearing whole game tree." << LOGF;
		Node *f = mroot;
		while(f != nullptr) {
			if(f->left != nullptr) {
				f = f->left;
			}
			else if (f->right != nullptr) {
				f = f->right;
			}
			else {
				Node *p = f->parent;
				if (!f->value.unique())
					throw GameException("The object is strongly held somewhere else.");
				if (p) {
					if (p->left == f)
						p->left = nullptr;
					else
						p->right = nullptr;
				}
				delete f;
				f = p;
			}
		}
		mroot = nullptr;
	}

	/*
	 * Print blank Vertex
	 */
	void SplayTree::printDotNull(objId key, int nullcount, std::ofstream& stream) {
		stream << "    null" << nullcount << " [shape=point];" << std::endl;
		stream << "    " << key << " -> null" << nullcount << ";" << std::endl;
	};

	/*
	 * Print Vertex
	 */
	void SplayTree::printDotVertex(Node* node, std::ofstream& stream) {
		static int nullcount = 0;

		if(node->left)
		{
			stream << "    " << node->value->getId() << " -> " << node->left->value->getId() << ";" << std::endl;
			printDotVertex(node->left, stream);
		}
		else
			printDotNull(node->value->getId(), nullcount++, stream);

		if(node->right)
		{
			stream << "    " << node->value->getId() << " -> " << node->right->value->getId() << ";" << std::endl;
			printDotVertex(node->right, stream);
		}
		else
			printDotNull(node->value->getId(), nullcount++, stream);
	};

	/*
	 * Serializes the Splay Tree into DOT notation
	 */
	void SplayTree::printDotFile(std::ofstream& stream) {
		stream << "digraph BST {" << std::endl;
		stream << "    node [fontname=\"Arial\"];\n";

		if (!mroot)
			stream << std::endl;
		else if (!mroot->right && !mroot->left)
			stream << "    " << mroot->value->getId() << std::endl;
		else
			printDotVertex(mroot, stream);

		stream << "}" << std::endl;
	};
}
