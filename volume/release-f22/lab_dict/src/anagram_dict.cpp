/**
 * @file anagram_dict.cpp
 * Implementation of the AnagramDict class.
 *
 * @author Matt Joras
 * @date Winter 2013
 */

#include "anagram_dict.h"

#include <algorithm> /* I wonder why this is included... */
#include <cctype>
#include <fstream>
#include <vector>

using std::ifstream;
using std::string;
using std::vector;

/**
 * Constructs an AnagramDict from a filename with newline-separated
 * words.
 * @param filename The name of the word list file.
 */
AnagramDict::AnagramDict(const string &filename) {
  /* Your code goes here! */
  ifstream wordFile(filename);
  string word;
  vector<string> words;

  if (wordFile.is_open()) {
    while (std::getline(wordFile, word)) {
      words.push_back(word);
    }
  }

  init(words);
}

/**
 * Constructs an AnagramDict from a vector of words.
 * @param words The vector of strings to be used as source words.
 */
AnagramDict::AnagramDict(const vector<string> &words) {
  /* Your code goes here! */
  init(words);
}

void AnagramDict::init(const vector<string> &words) {
  auto canonical = [](string word) {
    std::transform(word.cbegin(), word.cend(), word.begin(),
                   [](char c) { return std::tolower(c); });
    std::sort(word.begin(), word.end());
    return word;
  };

  for (auto word1 : words) {
    for (auto word2 : words) {
      if (canonical(word1) == canonical(word2)) {
        dict[word1].push_back(word2);
      }
    }
  }
}

/**
 * @param word The word to find anagrams of.
 * @return A vector of strings of anagrams of the given word. Empty
 * vector returned if no anagrams are found or the word is not in the
 * word list.
 */
vector<string> AnagramDict::get_anagrams(const string &word) const {
  /* Your code goes here! */
    auto it = dict.find(word);
  if (it != dict.end()) {
    if (it->second.size() > 1) return it->second;
  }
  return vector<string>{};
}

/**
 * @return A vector of vectors of strings. Each inner vector contains
 * the "anagram siblings", i.e. words that are anagrams of one another.
 * NOTE: It is impossible to have one of these vectors have less than
 * two elements, i.e. words with no anagrams are ommitted.
 */
vector<vector<string>> AnagramDict::get_all_anagrams() const {
  /* Your code goes here! */
  vector<vector<string>> ret;
  for (auto [key, val] : dict) {
    if (val.size() > 1) ret.push_back(val);
  }
  return ret;
}
