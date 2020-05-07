/**
 * This file uses a multiway trie to create a dictionary containing words.
 * Each word has its corresponding frequency which is a non negative number
 * that is greater than 0. This program allows for insertion and search for
 * words. Also, if given a prefix, this program will return the most freqeunt
 * words in the dictionary that start with the prefix. If given a word with
 * underscores, it will return the most frequent words in the dictionary
 * that match the given pattern.
 *
 * Authors: Marigold Doan, mvdoan@ucsd.edu
 *          Maisha Maliha, mmaliha@ucsd.edu
 */
#include "DictionaryTrie.hpp"
#include <iostream>
#include <queue>
#include <utility>

#define ALPHABET_SIZE 27  // Accounts for space and underscore
#define UNDERSCORE '_'    // underscore varaible
#define SPACE_INDEX 1     // alphabet size - 2 = space char

/** MWTNode Constructor **/
// Followed Pseudocode from Stepik
MWTNode::MWTNode(void) {
    frequency = 0;
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        children[i] = NULL;
    }
}

MWTNode::~MWTNode(void) {}

/* Constructor for a Dictionary Trie. We implemented it with a multiway trie. */
DictionaryTrie::DictionaryTrie() { root = new MWTNode(); }

/** Insert a word with its frequency into the DictionaryTrie. Return true if the
 *  word was inserted successfully, or false if the given string is empty string
 *  or has already existed in the trie.
 *  Parameters:
 *  word - The word to be inserted into the multiway trie
 *  freq - The number of times the word occurs in the trie
 */

bool DictionaryTrie::insert(string word, unsigned int freq) {
    MWTNode* curr = root;

    // If word is empty, nothing to insert, return false
    if (word.empty()) {
        return false;
    }

    for (unsigned int i = 0; i < word.size(); i++) {
        int c = (int)word.at(i);
        int num = (int)c - (int)'a';

        // Check if the character is an empty space
        if (word.at(i) == ' ') {
            num = ALPHABET_SIZE - SPACE_INDEX;
        }

        if (curr->children[num] == nullptr) {
            // if curr does not have an outgoing edge, create new Node
            curr->children[num] = new MWTNode();
        }
        curr = curr->children[num];
    }

    // if curr is not a word node, create a word node
    if (curr->frequency == 0) {
        curr->frequency = max(curr->frequency, freq);
        return true;
    }

    // If curr is already in the tree, increase its frequency
    return false;
}

/** Return true if the word is in the DictionaryTrie and false otherwise.
 *  Parameter:
 *  word - The word we want to find in the trie
 */
bool DictionaryTrie::find(string word) const {
    MWTNode* curr = root;

    // If word is empty, nothing to find, return false
    if (word.empty()) {
        return false;
    }

    for (unsigned int i = 0; i < word.size(); i++) {
        int c = (int)word.at(i);
        int num = (int)c - (int)'a';

        // Check if the character is an empty space
        if (word.at(i) == ' ') {
            num = ALPHABET_SIZE - SPACE_INDEX;
        }

        if (curr->children[num] == nullptr) {
            // If curr does not have an outgoing edge, return false
            return false;
        } else {
            // Make curr the child of curr along edge labeled by c
            curr = curr->children[num];
        }
    }

    // Check if curr is a word node
    if (curr->frequency != 0) {
        // We have found the word, return true
        return true;
    }
    return false;
}

/** Returns numCompletions amount of completions with a given prefix. If the
 *  amount of words found with the given prefix is smaller than
 *  numCompletions, return the vector of words found. This function also
 *  returns the words in order of frequency, and if two words have the same
 *  frequency, it returns the words in alphabetical order.
 *  Parameters:
 *  prefix - The prefix of the words we want to find
 *  numCompletions - The number of words we want to find
 */
vector<string> DictionaryTrie::predictCompletions(string prefix,
                                                  unsigned int numCompletions) {
    vector<string> possibleWords;
    // MWTNode* curr = root;
    queue<pair<string, MWTNode*>> second_queue;

    // If 0 is inputted return an empty word vector
    if (prefix.length() == 0 || numCompletions <= 0) {
        return possibleWords;
    }

    // Call on findHelper
    MWTNode* curr = findHelper(prefix, root);

    // Check to see if our pointer exists
    // If not just return the empty vector
    if (curr == nullptr) {
        return possibleWords;
    }

    // Initialize priority queue
    priority_queue<pair<string, int>, vector<pair<string, int>>,
                   CustomComparator>
        pq;
    pair<string, MWTNode*> p = make_pair(prefix, curr);
    // add the node
    second_queue.push(p);

    // BFS DOWN
    while (second_queue.empty() != true) {
        // return the top element in queue
        pair<string, MWTNode*> q = second_queue.front();
        second_queue.pop();
        // if word freq > 0 then we add to our priority queue

        if ((q.second->frequency > 0)) {
            pq.push(make_pair(q.first, q.second->frequency));
        }

        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (q.second->children[i] != nullptr) {
                pair<string, MWTNode*> r =
                    make_pair(q.first + returnChar(i), q.second->children[i]);
                second_queue.push(r);
            }
        }
    }

    // Call on helper to check size
    unsigned int returnSize = checkSize(numCompletions, pq.size());

    // Add words to vector and return
    for (unsigned int i = 0; i < returnSize; i++) {
        pair<string, int> returnWord;
        returnWord = pq.top();
        pq.pop();
        possibleWords.push_back(returnWord.first);
    }
    return possibleWords;
}

/** Returns a vector of strings that match the user inputted pattern.
 *  The pattern contains underscores that represent wildcards. This function
 *  will find numCompletions amount of possible words, and if the number
 *  of words found is less than numCompletions, then the vector will return
 *  what it has. Also, the words will appear in order of frequency and if
 *  two words have the same frequency, then it will be sorted
 *  alphabetically.
 *  Parameters:
 *  pattern - The pattern of the word that we will find suggestions for
 *  numCompletions - the amount of words we want to find
 */
std::vector<string> DictionaryTrie::predictUnderscores(
    string pattern, unsigned int numCompletions) {
    // intialize a queue to contain words
    queue<pair<string, MWTNode*>> underscoreQueue;
    // initialize vector
    vector<string> wildCard;
    // initialize a priority queue
    priority_queue<pair<string, int>, vector<pair<string, int>>,
                   CustomComparator>
        pq;

    // Return empty vector if inputs are invalid
    if (numCompletions <= 0 || pattern.size() == 0) {
        return wildCard;
    }

    // find pre-underscore
    MWTNode* curr = findHelperUnderscore(pattern, root);

    // Check to see if our pointer exists
    // If not just return the empty vector
    if (curr == nullptr) {
        return wildCard;
    }

    pair<string, MWTNode*> p = make_pair("", curr);
    // add the node
    underscoreQueue.push(p);

    // BFS DOWN
    while (underscoreQueue.empty() != true) {
        // return the top element in queue
        pair<string, MWTNode*> q = underscoreQueue.front();
        underscoreQueue.pop();
        // if word freq > 0 then we add to our priority queue

        if (q.second->frequency > 0) {
            pq.push(make_pair(q.first, q.second->frequency));
        }
        for (int j = 0; j < ALPHABET_SIZE; j++) {
            if (q.second->children[j] != nullptr) {
                pair<string, MWTNode*> r =
                    make_pair(q.first + returnChar(j), q.second->children[j]);
                underscoreQueue.push(r);
            }
        }
    }

    // pop the queue sequentially using a for loop

    for (unsigned int k = 0; k < pq.size(); k++) {
        pair<string, int> possibleWord = pq.top();
        pq.pop();
        for (unsigned int i = 0; i < pattern.size(); i++) {
            string q = pattern.substr(i + 1, pattern.size());
            string r =
                possibleWord.first.substr(i + 1, possibleWord.first.size());
            if (q.compare(r) == 0) {
                wildCard.push_back(possibleWord.first);
            }
        }
    }

    return wildCard;
}

/** Destructor for the DictionaryTrie class */
DictionaryTrie::~DictionaryTrie() { deleteTrie(root); }

/** Helper method for deleting the trie
 *  Parameter:
 *  n - The MWT node to start at
 */
void DictionaryTrie::deleteTrie(MWTNode* n) {
    // base case
    if (n == nullptr) {
        return;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        deleteTrie(n->children[i]);  // delete the entire char array
    }
    delete n;
}

/** Helper to turn number to char
 *  Parameter:
 *  i - ASCII value in decimal that we want to convert into a char
 */
char DictionaryTrie::returnChar(int i) {
    return "abcdefghijklmnopqrstuvwxyz "[i];
}

/** Find helper returns MWTNode when it exists in the trie
 *  Follows the const find algorithm
 */
MWTNode* DictionaryTrie::findHelper(string prefix, MWTNode* root) {
    MWTNode* curr = root;

    // Find prefix in our MWT
    for (unsigned int i = 0; i < prefix.length(); i++) {
        int c = (int)prefix.at(i);
        int num = (int)c - (int)'a';

        // Check if the character is an empty space
        if (prefix.at(i) == ' ') {
            num = ALPHABET_SIZE - SPACE_INDEX;
        }

        // Check to see if our pointer exists
        // If not just return the empty vector
        if (curr->children[num] == nullptr) {
            return nullptr;
        }
        // Go down the tree
        curr = curr->children[num];
    }
    return curr;
}

/** Helper used when we encounter an underscore */
MWTNode* DictionaryTrie::findHelperUnderscore(string prefix, MWTNode* root) {
    MWTNode* curr = root;

    // Find prefix in our MWT
    unsigned int i = 0;
    while (prefix.at(i) != '_') {
        int c = (int)prefix.at(i);
        int num = (int)c - (int)'a';

        // Check if the character is an empty space
        if (prefix.at(i) == ' ') {
            num = ALPHABET_SIZE - SPACE_INDEX;
        }

        // Check to see if our pointer exists
        // If not just return the empty vector
        if (curr->children[num] == nullptr) {
            return nullptr;
        }

        // Go down the tree
        curr = curr->children[num];
        if (i < prefix.size()) {
            i++;
        }
    }

    return curr;
}
/** Helper method that checks if size of vector should be changed
 * Helps in optimizing predictCompletions
 * */

int DictionaryTrie::checkSize(int numCompletions, int returnSize) {
    if (returnSize < numCompletions) {
        return returnSize;
    }
    return numCompletions;
}
