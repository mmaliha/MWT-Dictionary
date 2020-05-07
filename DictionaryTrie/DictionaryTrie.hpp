/**
 * This file declares the classes used in DictionaryTrie. It also contains
 * declarations of the methods used in the classes. Additionally, there
 * is a comparator declared within this file that is used in the priority
 * queue that is later used in the predictCompletions and predictUnderscore
 * methods.
 *
 * Authors: Marigold Doan, mvdoan@ucsd.edu
 *          Maisha Maliha, mmaliha@ucsd.edu
 */
#ifndef DICTIONARY_TRIE_HPP
#define DICTIONARY_TRIE_HPP

#include <string>
#include <utility>
#include <vector>

using namespace std;

/** MWTNode class to support our MWT. Contains the members of the class, all
 *  of which are public. There is an array of letters including the space
 *  character that is used to indicate which character is which when going
 *  through the trie. Frequency determines the number of times the word
 *  occurs in the dictionary.
 */
class MWTNode {
  public:
    MWTNode* children[27];  // array for the letters
    unsigned int
        frequency;  // 0 if the word does not exist and more than 1 otherwise
    MWTNode();
    ~MWTNode();  // destructor
};

/** Comparator to sort our priority queue */
struct CustomComparator {
    bool operator()(pair<string, int> P1, pair<string, int> P2) {
        if (P1.second == P2.second) {
            return P1.first > P2.first;
        }
        return P1.second < P2.second;
    }
};

/**
 * The class for a dictionary ADT, implemented as either
 * a mulit-way trie or a ternary search tree.
 */
class DictionaryTrie {
  private:
    // TODO: add private members and helper methods here
    // MWTNode* findHelper(string prefix, MWTNode* root);
    void deleteTrie(MWTNode* n);
    char returnChar(int i);
    MWTNode* findHelper(string prefix, MWTNode* root);
    MWTNode* findHelperUnderscore(string prefix, MWTNode* root);
    int checkSize(int numCompletions, int returnSize);

  public:
    // Adding MWTRoot pointer
    MWTNode* root;

    /* Built with a multiway trie. */
    DictionaryTrie();

    /* Insert a word with its frequency into the DictionaryTrie. Return true if
     * the word was inserted successfully, or false if the given string is empty
     * string or has already existed in the trie.
     * Parameters:
     * word - The word to be inserted into the multiway trie
     * freq - The number of times the word occurs in the trie
     */
    bool insert(string word, unsigned int freq);

    /* Return true if the word is in the DictionaryTrie and false otherwise.
     *  Parameter:
     *  word - The word we want to find in the trie
     */
    bool find(string word) const;

    /* Returns numCompletions amount of completions with a given prefix. If the
     * amount of words found with the given prefix is smaller than
     * numCompletions, return the vector of words found. This function also
     * returns the words in order of frequency, and if two words have the same
     * frequency, it returns the words in alphabetical order.
     * Parameters:
     * prefix - The prefix of the words we want to find
     * numCompletions - The number of words we want to find
     */
    vector<string> predictCompletions(string prefix,
                                      unsigned int numCompletions);

    /* Returns a vector of strings that match the user inputted pattern.
     * The pattern contains underscores that represent wildcards. This function
     * will find numCompletions amount of possible words, and if the number
     * of words found is less than numCompletions, then the vector will return
     * what it has. Also, the words will appear in order of frequency and if
     * two words have the same frequency, then it will be sorted
     * alphabetically.
     * Parameters:
     * pattern - The pattern of the word that we will find suggestions for
     * numCompletions - the amount of words we want to find
     */
    vector<string> predictUnderscores(string pattern,
                                      unsigned int numCompletions);

    /* Destructor for the DictionaryTrie class */
    ~DictionaryTrie();
};

#endif  // DICTIONARY_TRIE_HPP
