//Umit Erkut Colak 29198

#include <iostream>
#include <String>
#include <vector>
#include <chrono>
#include <fstream>
#include <sstream>

#include "BinarySearchTree.h"
#include "HashTable.h"
#include "QuickSort.h"
#include "InsertionSort.h"
#include "MergeSort.h"
#include "HeapSort.h"
using namespace std;

//*********** Data Structs ***************
struct DocumentItem {
	string documentName;
	int count;

	bool operator<(const DocumentItem& other) {
		return count < other.count;
	}
	bool operator>(const DocumentItem& other) {
		return count > other.count;
	}
	bool operator==(const DocumentItem& other) {
		return count == other.count;
	}
};

struct WordItem {
	string word;
	vector<DocumentItem> documents;

	bool operator<(const WordItem& other) const {
		return word < other.word;
	}
	bool operator>(const WordItem& other) const {
		return word > other.word;
	}
	bool operator>=(const WordItem& other) const {
		return word >= other.word;
	}
	bool operator<=(const WordItem& other) const {
		return word <= other.word;
	}
	bool operator!=(const WordItem& other) const {
		return word != other.word;
	}
	bool operator==(const WordItem& other) const {
		return word == other.word;
	}
};


//*********** Helper Functions ***************

void updateOrInsertWordBST(const string& word, const string& docName, BinarySearchTree<WordItem>& tree) {
	WordItem temp;
	temp.word = word;
	const WordItem& foundItem = tree.find(temp);

	// Update count
	if (foundItem.word == word) {
		WordItem updateItem = foundItem;

		bool documentFound = false;
		for (int i = 0; i < updateItem.documents.size(); i++) {
			if (updateItem.documents[i].documentName == docName) {
				updateItem.documents[i].count++;
				documentFound = true;
				break;
			}
		}
		if (!documentFound) {
			updateItem.documents.push_back(DocumentItem{ docName,1 });
		}
		tree.remove(foundItem);
		tree.insert(updateItem);
	}

	// Insert new Item
	else {
		WordItem newItem;
		newItem.word = word;
		newItem.documents.push_back(DocumentItem{ docName,1 });
		tree.insert(newItem);
	}
}

void processQueryBST(const vector<string> documents, BinarySearchTree<WordItem>& tree) {
	for (int i = 0; i < documents.size(); i++) {
		string docName = documents[i];
		ifstream file(docName);
		if (!file.is_open()) {
			cout << "Failed to open file: " << docName << endl;
		}

		string word;
		char ch;
		while (file.get(ch)) {
			if (isalpha(ch)) {
				word += tolower(ch);
			}
			else if (!word.empty()) {
				updateOrInsertWordBST(word, docName, tree);
				word.clear();
			}
		}
		if (!word.empty()) {
			updateOrInsertWordBST(word, docName, tree);
		}
		file.close();
	}
}


void updateOrInsertWordHashTable(const string& word, const string& docName, HashTable<WordItem>& hashTable) {
	WordItem temp;
	temp.word = word;
	const WordItem& foundItem = hashTable.find(temp);

	// Update count
	if (foundItem != hashTable.getNotFoundItem()) {
		bool documentFound = false;
		WordItem updateItem = foundItem;
		for (int i = 0; i < updateItem.documents.size(); i++) {
			if (updateItem.documents[i].documentName == docName) {
				updateItem.documents[i].count++;
				documentFound = true;
				break;
			}
		}
		if (!documentFound) {
			updateItem.documents.push_back(DocumentItem{ docName,1 });
		}
		hashTable.remove(foundItem);
		hashTable.insert(updateItem);
	}
	else {
		WordItem newItem;
		newItem.word = word;
		newItem.documents.push_back(DocumentItem{ docName,1 });
		hashTable.insert(newItem);
	}
}

void processQueryHashTable(const vector<string> documents, HashTable<WordItem>& hashTable) {
	for (int i = 0; i < documents.size(); i++) {
		string docName = documents[i];
		ifstream file(docName);
		if (!file.is_open()) {
			cout << "Failed to open file: " << docName << endl;
		}
		string word;
		char ch;
		while (file.get(ch)) {
			if (isalpha(ch)) {
				word += tolower(ch);
			}
			else if (!word.empty()) {
				updateOrInsertWordHashTable(word, docName, hashTable);
				word.clear();
			}
		}

		if (!word.empty()) {
			updateOrInsertWordHashTable(word, docName, hashTable);
		}
		file.close();
	}
}


void updateOrPushBack(const string& word, const string& documentName, vector<WordItem>& wordItems) {
	for (int i = 0; i < wordItems.size(); i++) {
		if (wordItems[i].word == word) {
			bool documentFound = false;
			for (int j = 0; j < wordItems[i].documents.size(); j++) {
				if (wordItems[i].documents[j].documentName == documentName) {
					wordItems[i].documents[j].count++;
					documentFound = true;
					break;
				}
			}
			if (!documentFound) {
				wordItems[i].documents.push_back(DocumentItem{ documentName, 1 });
			}
			return;
		}
	}
	wordItems.push_back(WordItem{ word, vector<DocumentItem>{DocumentItem{documentName, 1}} });
}

void processDocumentsVector(const vector<string>& documentNames, vector<WordItem>& wordItems) {
	wordItems.clear();

	for (int i = 0; i < documentNames.size(); i++) {
		ifstream file(documentNames[i]);
		if (!file.is_open()) {
			cout << "Failed to open file: " << documentNames[i] << endl;
			continue;
		}

		string word;
		char ch;
		while (file.get(ch)) {
			if (isalpha(ch)) {
				word += tolower(ch);
			}
			else if (!word.empty()) {
				updateOrPushBack(word, documentNames[i], wordItems);
				word.clear();
			}
		}
		// Handle the last word in file
		if (!word.empty()) {
			updateOrPushBack(word, documentNames[i], wordItems);
		}
		file.close();
	}
}

WordItem* binarySearch(vector<WordItem>& sortedItems, const string& queryWord) {
	int low = 0;
	int high = sortedItems.size() - 1;
	while (low <= high) {
		int mid = low + (high - low) / 2;
		if (sortedItems[mid].word == queryWord) {
			return &sortedItems[mid];
		}
		else if (sortedItems[mid].word < queryWord) {
			low = mid + 1;
		}
		else {
			high = mid - 1;
		}
	}
	return nullptr; 
}



//*********** Main Function ***************
int main() {
	int numOfInputs;
	cout << "Enter number of input files : ";
	cin >> numOfInputs;
	vector<string> documents(numOfInputs);
	string docName;


	for (int i = 0; i < numOfInputs; i++) {
		cout << "Enter " + to_string(i + 1) + ". file name: ";
		cin >> docName;
		documents[i] = docName;
	}

	WordItem defaultWordItem;
	BinarySearchTree<WordItem> BST(defaultWordItem);
	HashTable<WordItem> hashTable(defaultWordItem);


	
	processQueryBST(documents, BST);
	processQueryHashTable(documents, hashTable);
	vector<WordItem> wordItems;
	processDocumentsVector(documents, wordItems);

	cout << endl;
	double loadFactor = hashTable.getLoadFactor();
	cout << "After preprocessing, the unique word count is " << wordItems.size()
		<< ". Current load ratio is " << loadFactor << "." << endl;

	cout << "Enter queried words in one line: ";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	string inputLine;
	getline(cin, inputLine);

	vector<string> queryWords;
	string temp;
	char ch;
	for (int i = 0; i < inputLine.length(); i++) {
		ch = inputLine.at(i);
		if (isalpha(ch)) {
			temp += tolower(ch);
		}
		else if (!temp.empty()) {
			queryWords.push_back(temp);
			temp.clear();
		}
	}
	if (!temp.empty()) {
		queryWords.push_back(temp);
	}


	int k = 100;

	//////////////////////////////////////////////////////// BST PART ///////////////////////////////////////////////////////////////
	auto start = std::chrono::high_resolution_clock::now();
	bool wordFound = false;
	for (int i = 0; i < documents.size(); i++) {
		string docName = documents[i];
		bool allWordsFound = true;
		stringstream output;

		for (int j = 0; j < queryWords.size(); j++) {
			string wordInQuery = queryWords[j];
			WordItem wordToFind;
			wordToFind.word = wordInQuery;
			const WordItem& foundWordItem = BST.find(wordToFind);

			bool currentWordFound = false;
			for (int k = 0; k < foundWordItem.documents.size(); k++) {
				const DocumentItem& docItem = foundWordItem.documents[k];

				if (docItem.documentName == docName) {
					output << wordInQuery << " found " << docItem.count << " times, ";
					currentWordFound = true;
					break;
				}
			}
			if (!currentWordFound) {
				allWordsFound = false;
				break;
			}
		}
		if (allWordsFound) {
			wordFound = true;
			cout << "in Document " << docName << ", " << output.str().substr(0, output.str().length() - 2) << ".\n";
		}

	}
	if (!wordFound) {
		cout << "No document contains the given query" << endl;
	}

	auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
		(std::chrono::high_resolution_clock::now() - start);


	////////////////////////////////////////////////////////////////// HASH TABLE PART /////////////////////////////////////////////////////////////////////////////


	start = std::chrono::high_resolution_clock::now();

	wordFound = false;
	for (int i = 0; i < documents.size(); i++) {
		string docName = documents[i];
		bool allWordsFound = true;
		stringstream out;

		for (int j = 0; j < queryWords.size(); j++) {
			WordItem wordToFind;
			wordToFind.word = queryWords[j];

			if (!hashTable.contains(wordToFind)) {
				allWordsFound = false;
				break;
			}

			const WordItem& foundWordItem = hashTable.find(wordToFind);

			bool foundInCurrentDoc = false;
			for (int k = 0; k < foundWordItem.documents.size(); k++) {
				DocumentItem docItem = foundWordItem.documents[k];
				if (docItem.documentName == docName) {
					out << queryWords[j] << " found " << docItem.count << " times, ";
					foundInCurrentDoc = true;
					break;
				}
			}

			if (!foundInCurrentDoc) {
				allWordsFound = false;
				break;
			}
		}
		if (allWordsFound) {
			cout << "in Document " << docName << ", " << out.str().substr(0, out.str().length() - 2) << ".\n";
			wordFound = true;
		}
	}
	if (!wordFound) {
		cout << "No document contains the given query" << endl;
	}

	auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
		(std::chrono::high_resolution_clock::now() - start);


	


	////////////////////////////////////////////////////////////////// SORTING VECTORS PART /////////////////////////////////////////////////////////////////////////////
	
	vector<WordItem> quickSortedRandom = wordItems;
	vector<WordItem> quickSortedMedian = wordItems;
	vector<WordItem> quickSortedFirst = wordItems;
	vector<WordItem> insertionSorted = wordItems;
	vector<WordItem> mergeSorted = wordItems;
	vector<WordItem> heapSorted = wordItems;


	srand(static_cast<unsigned int>(time(nullptr)));

	//Sort the vectors with different sorting algorithms:
	start = chrono::high_resolution_clock::now();
	QuickSort<WordItem>::quicksort(quickSortedRandom, QuickSort<WordItem>::RANDOM_ELEMENT);
	auto durationQuickSortedRandom = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start);
	
	start = chrono::high_resolution_clock::now();
	QuickSort<WordItem>::quicksort(quickSortedMedian, QuickSort<WordItem>::MEDIAN_OF_THREE);
	auto durationQuickSortedMedian = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start);
	
	start = chrono::high_resolution_clock::now();
	QuickSort<WordItem>::quicksort(quickSortedFirst, QuickSort<WordItem>::FIRST_ELEMENT);
	auto durationQuickSortedFirst = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start);
	

	start = chrono::high_resolution_clock::now();
	MergeSort<WordItem>::sort(mergeSorted);
	auto durationMergeSorted = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start);
	

	start = chrono::high_resolution_clock::now();
	HeapSort<WordItem>::sort(heapSorted);
	auto durationHeapSorted = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start);
	
	start = chrono::high_resolution_clock::now();
	InsertionSort<WordItem>::sort(insertionSorted);
	auto durationInsertionSorted = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start);
	

	////////////////////////////////////////////////////////////////// BINARY SEARCH PART /////////////////////////////////////////////////////////////////////////////
	start = chrono::high_resolution_clock::now();
	wordFound = false;
	for (int i = 0; i < documents.size(); i++) {
		string documentName = documents[i];
		bool allWordsFound = true;
		stringstream output;

		for (int j = 0; j < queryWords.size(); j++) {
			string queryWord = queryWords[j];
			WordItem* foundWordItem = binarySearch(mergeSorted, queryWord);

			if (foundWordItem != nullptr) {
				bool wordInCurrentDoc = false;

				for (int k = 0; k < foundWordItem->documents.size(); k++) {
					const DocumentItem& docItem = foundWordItem->documents[k];

					if (docItem.documentName == documentName) {
						output << queryWord << " found " << docItem.count << " times, ";
						wordInCurrentDoc = true;
						break;
					}

				}
				if (!wordInCurrentDoc) {
					allWordsFound = false;
					break;
				}
			}
			else {
				allWordsFound = false;
				break;
			}
		}
		if (allWordsFound) {
			wordFound = true;
			cout << "in Document " << docName << ", " << output.str().substr(0, output.str().length() - 2) << ".\n";
		}
	}
	if (!wordFound) {
		cout << "No document contains the given query" << endl;
	}

	auto binarySearchTime = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start);

	


	cout << endl;
	
	cout << "Binary Search Tree Time: " << BSTTime.count() / k << "\n";
	cout << "Hash Table Time: " << HTTime.count() / k << "\n";
	cout << "Binary Search Time: " << binarySearchTime.count() / k << "\n";
	cout << endl;

	cout << "Quick Sort(random) Time: " << durationQuickSortedRandom.count() / k << "\n";
	cout << "Quick Sort(median) Time: " << durationQuickSortedMedian.count() / k << "\n";
	cout << "Quick Sort(first) Time: " << durationQuickSortedFirst.count() / k << "\n";
	cout << "Merge Sort Time: " << durationMergeSorted.count() / k << "\n";
	cout << "Heap Sort Time: " << durationHeapSorted.count() / k << "\n";
	cout << "Insertion Sort Time: " << durationInsertionSorted.count() / k << "\n";
	cout << endl;

	cout << "Speed Up BST/HST: " << static_cast<double>(BSTTime.count()) / static_cast<double>(HTTime.count()) << "\n";
	cout << "Speed Up Merge Sort/Quick Sort(Median): " << static_cast<double>(durationMergeSorted.count()) / static_cast<double>(durationQuickSortedMedian.count()) << "\n";
	cout << "Speed Up Heap Sort/Quick Sort(Median): " << static_cast<double>(durationHeapSorted.count()) / static_cast<double>(durationQuickSortedMedian.count()) << "\n";
	cout << "Speed Up Insertion Sort/Quick Sort(Median): " << static_cast<double>(durationInsertionSorted.count()) / static_cast<double>(durationQuickSortedMedian.count()) << "\n";
	cout << endl;

	cout << "Speed Up Binary Search Tree Time / Binary Search: " << static_cast<double>(BSTTime.count()) / static_cast<double>(binarySearchTime.count()) << "\n";
	cout << "Speed Up Binary Search / Hash Table Time: " << static_cast<double>(binarySearchTime.count()) / static_cast<double>(HTTime.count()) << "\n";


	return 0;
}

