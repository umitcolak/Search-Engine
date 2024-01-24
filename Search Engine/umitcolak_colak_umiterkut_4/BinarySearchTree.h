#pragma once
#include<iostream>
#include<vector>
using namespace std;

template<class Comparable>
class BinarySearchTree;

template<class Comparable>
class BinaryNode {
	Comparable element;
	BinaryNode* left;
	BinaryNode* right;

	BinaryNode(const Comparable& theElement, BinaryNode* lt, BinaryNode* rt)
		: element(theElement), left(lt), right(rt) {}

	friend class BinarySearchTree<Comparable>;
};


template<class Comparable>
class BinarySearchTree {

public:
	// Constructor & Destructor
	explicit BinarySearchTree(const Comparable& notFound) :
		ITEM_NOT_FOUND(notFound), root(NULL) {}
	BinarySearchTree(const BinarySearchTree<Comparable>& rhs)
		: root(NULL), ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND)
	{
		*this = rhs;
	}
	~BinarySearchTree() {
		makeEmpty();
	}

	// Public Accessors
			/**
			 * Find the smallest item in the tree.
			 * Return smallest item or ITEM_NOT_FOUND if empty.
			 */
	const Comparable& findMin() const {
		return elementAt(findMin(root));
	}

	/**
	 * Find the smallest item in the tree.
	* Return smallest item or ITEM_NOT_FOUND if empty.
	*/
	const Comparable& findMax() const {
		return elementAt(findMax(root));
	}

	/**
	 * Find item x in the tree.
	 * Return the matching item or ITEM_NOT_FOUND if not found.
	*/
	const Comparable& find(const Comparable& x) const {
		return elementAt(find(x, root));
	}

	bool isEmpty() const {
		return root == NULL;
	}

	/**
	 * Print the tree contents in sorted order.
	 */
	void printTree() const {
		if (isEmpty()) {
			cout << "Empty tree" << endl;
		}
		else {
			printTree(root);
		}
	}

	// Public Mutators
			/**
			 * Make the tree logically empty.
			 */
	void makeEmpty() {
		makeEmpty(root);
	}

	/**
	 * Insert x into the tree; duplicates are ignored.
	 */
	void insert(const Comparable& x) {
		insert(x, root);
	}

	/**
	* Remove x from the tree. Nothing is done if x is not found.
	*/
	void remove(const Comparable& x) {
		remove(x, root);
	}

	const BinarySearchTree& operator =(const BinarySearchTree& rhs) {
		if (this != &rhs) {
			makeEmpty();
			root = clone(rhs.root);
		}
		return *this;
	}

private:
	BinaryNode<Comparable>* root;
	const Comparable ITEM_NOT_FOUND;

	// Private Accessors
			/**
			 * Internal method to get element field in node t.
			 * Return the element field or ITEM_NOT_FOUND if t is NULL.
			 */
	const Comparable& elementAt(BinaryNode<Comparable>* t) const {
		return t == NULL ? ITEM_NOT_FOUND : t->element;
	}

	/**
	* Internal method to insert into a subtree.
	* x is the item to insert.
	* t is the node that roots the tree.
	* Set the new root.
	*/
	void insert(const Comparable& x, BinaryNode<Comparable>*& t) const {
		if (t == NULL) {
			t = new BinaryNode<Comparable>(x, NULL, NULL);
		}
		else if (x < t->element) {
			insert(x, t->left);
		}
		else if (x > t->element) {
			insert(x, t->right);
		}
	}

	/**
	 * Internal method to remove from a subtree.
	 * x is the item to remove.
	 * t is the node that roots the tree.
	 * Set the new root.
	 */
	void remove(const Comparable& x, BinaryNode<Comparable>*& t) const {
		if (t == NULL) {
			return;
		}
		if (x < t->element) {
			remove(x, t->left);
		}
		else if (x > t->element) {
			remove(x, t->right);
		}
		else if (t->left != NULL && t->right != NULL) {
			t->element = findMin(t->right)->element;
			remove(t->element, t->right);
		}
		else {
			BinaryNode<Comparable>* oldNode = t;
			t = (t->left != NULL) ? t->left : t->right;
			delete oldNode;
		}
	}

	/**
	 * Internal method to find the smallest item in a subtree t.
	 * Return node containing the smallest item.
	 */
	BinaryNode<Comparable>* findMin(BinaryNode<Comparable>* t) const {
		if (t == NULL) {
			return NULL;
		}
		if (t->left == NULL) {
			return t;
		}
		return findMin(t->left);
	}

	/**
	* Internal method to find the largest item in a subtree t.
	 * Return node containing the largest item.
	*/
	BinaryNode<Comparable>* findMax(BinaryNode<Comparable>* t) const {
		if (t != NULL) {
			while (t->right != NULL) {
				t = t->right;
			}
		}
		return t;
	}

	/**
	 * Internal method to find an item in a subtree.
	 * x is item to search for.
	 * t is the node that roots the tree.
	 * Return node containing the matched item.
	 */
	BinaryNode<Comparable>* find(const Comparable& x, BinaryNode<Comparable>* t) const {
		if (t == NULL) {
			return NULL;
		}
		else if (x < t->element) {
			return find(x, t->left);
		}
		else if (x > t->element) {
			return find(x, t->right);
		}
		else {
			return t;
		}
	}


	// Private Mutators
			/**
			 * Internal method to make subtree empty.
			 */
	void makeEmpty(BinaryNode<Comparable>*& t) const {
		if (t != NULL) {
			makeEmpty(t->left);
			makeEmpty(t->right);
			delete t;
		}
		t = NULL;
	}

	void printTree(BinaryNode<Comparable>* t) const {
		if (t != NULL) {
			printTree(t->left);
			cout << t->element << endl;
			printTree(t->right);
		}
	}

	BinaryNode<Comparable>* clone(BinaryNode<Comparable>* t) const {
		if (t == NULL) {
			return NULL;
		}
		else {
			return new BinaryNode<Comparable>(t->element, clone(t->left), clone(t->right));
		}
	}

};