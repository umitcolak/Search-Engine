#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <functional>

using namespace std;

template <class HashedObj>
class HashTable {
	// Constructor & Destructor
public:
	/**
	 * Construct the hash table.
	 */
	explicit HashTable(const HashedObj& notFound, int size = 101)
		:ITEM_NOT_FOUND(notFound), array(nextPrime(size))
	{
		makeEmpty();
	}

	HashTable() : ITEM_NOT_FOUND(HashedObj()), currentSize(0) {
		array.resize(101); // Assuming 101 as the default size
		makeEmpty();
	}

	HashTable(const HashTable& rhs)
		: ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND), array(rhs.array), currentSize(rhs.currentSize) {}

	// Public Accessors
			/**
			 * Find item x in the hash table.
			 * Return the matching item, or ITEM_NOT_FOUND, if not found.
			 */
	const HashedObj& find(const HashedObj& x) const {
		int currentPos = findPos(x);
		if (isActive(currentPos)) {
			return array[currentPos].element;
		}
		return ITEM_NOT_FOUND;
	}

	int hash(const HashedObj& x, int tableSize) const {
		int hashValue = 0;
		string str = x.word;

		char ch;
		for (int i = 0; i < str.length(); i++) {
			ch = str.at(i);
			hashValue = 37 * hashValue + ch;
		}
		hashValue %= tableSize;
		if (hashValue < 0) {
			hashValue += tableSize;
		}

		return hashValue;
	}

	bool contains(const HashedObj& x) const {
		return isActive(findPos(x));
	}

	const HashedObj& getNotFoundItem() const {
		return ITEM_NOT_FOUND;
	}

	// Public Mutators
			/**
			 * Empties the table
			 */
	void makeEmpty() {
		currentSize = 0;

		for (int i = 0; i < array.size(); i++) {
			array[i].info = EMPTY;
		}
	}

	/**
	 * Insert item x into the hash table. If the item is
	 * already present, then do nothing.
	 */
	void insert(const HashedObj& x) {
		int currentPos = findPos(x);
		if (isActive(currentPos)) {
			return;
		}
		
		array[currentPos] = HashEntry(x, ACTIVE);
		if (++currentSize >= array.size() / 2) {
			rehash();
		}
	}

	/**
	 * Remove item x from the hash table.
	 *  x has to be in the table
	 */
	void remove(const HashedObj& x) {
		int currentPos = findPos(x);
		if (isActive(currentPos)) {
			array[currentPos].info = DELETED;
		}
	}

	/**
	 * Overloading assignment operator
	 */
	const HashTable<HashedObj>& operator=(const HashTable& rhs) {
		if (this != &rhs) {
			array = rhs.array;
			currentSize = rhs.currentSize;
			ITEM_NOT_FOUND = rhs.ITEM_NOT_FOUND;
		}
		return *this;
	}


	void printCurrentStatus() const {
		double loadRatio = static_cast<double>(currentSize) / array.size();
		cout << "After preprocessing, the unique word count is " << currentSize
			<< ". Current load ratio is " << loadRatio << "." << endl;
	}

	double getLoadFactor() const {
		double loadRatio = static_cast<double>(currentSize) / array.size();
		return loadRatio;
	}



	enum EntryType { ACTIVE, EMPTY, DELETED };

private:
	struct HashEntry {
		HashedObj element;
		EntryType info;
		HashEntry(const HashedObj& e = HashedObj(),
			EntryType i = EMPTY)
			: element(e), info(i) {}
	};

	vector<HashEntry> array;
	int currentSize;
	const HashedObj  ITEM_NOT_FOUND;

	// Private Accessors
			/**
			 * Return true if currentPos exists and is active.
			 */
	bool isActive(int currentPos) const {
		return array[currentPos].info == ACTIVE;
	}

	/**
	 * Method that performs quadratic probing resolution.
	 * Return the position where the search for x terminates.
	 */
	int findPos(const HashedObj& x) const {
		int collisionNum = 0;
		int currentPos = hash(x, array.size());
		int firstDeletedPos = -1; 

		while (array[currentPos].info != EMPTY &&
			array[currentPos].element != x) {
			if (array[currentPos].info == DELETED && firstDeletedPos == -1) {
				firstDeletedPos = currentPos; 
			}
			currentPos += pow(++collisionNum, 2);
			if (currentPos >= array.size()) {
				currentPos -= array.size();
			}
		}

		
		return (array[currentPos].info != EMPTY || firstDeletedPos == -1) ? currentPos : firstDeletedPos;
	}


	/**
	 * Internal method to return a prime number
	* at least as large as n.  Assumes n > 0.
	 */
	int nextPrime(int n) {
		if (n % 2 == 0) {
			n++;
		}
		for (; !isPrime(n); n += 2);
		return n;
	}

	/**
	 * Internal method to test if a positive number is prime.
	 * Not an efficient algorithm.
	 */
	bool isPrime(int n) {
		if (n == 2 || n == 3) {
			return true;
		}
		if (n == 1 || n % 2 == 0) {
			return false;
		}
		for (int i = 3; i * i <= n; i += 2) {
			if (n % i == 0) {
				return false;
			}
		}
		return true;
	}


	// Private Mutators
			/**
			* Expand the hash table.
			*/
	void rehash() {
		int oldArraySize = array.size();
		vector<HashEntry> oldArray = array;

		int uniqueWordCount = currentSize;


		array.resize(nextPrime(2 * oldArray.size()));

		for (int j = 0; j < array.size(); j++) {
			array[j].info = EMPTY;
		}

		currentSize = 0;
		for (int i = 0; i < oldArray.size(); i++) {
			if (oldArray[i].info == ACTIVE) {
				insert(oldArray[i].element);
			}
		}

		double loadFactor = static_cast<double>(uniqueWordCount) / oldArraySize;

	}
};