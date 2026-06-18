/**
 * @file cartalk_puzzle.cpp
 * Holds the function which solves a CarTalk puzzler.
 *
 * @author Matt Joras
 * @date Winter 2013
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

#include "cartalk_puzzle.h"

using namespace std;

/**
 * Solves the CarTalk puzzler described here:
 * http://www.cartalk.com/content/wordplay-anyone.
 * @return A vector of (string, string, string) tuples
 * Returns an empty vector if no solutions are found.
 * @param d The PronounceDict to be used to solve the puzzle.
 * @param word_list_fname The filename of the word list to be used.
 */
vector<std::tuple<std::string, std::string, std::string>>
cartalk_puzzle(PronounceDict d, const string &word_list_fname) {
  vector<std::tuple<std::string, std::string, std::string>> ret;

  ifstream wordsFile(word_list_fname);
  string word;
  if (wordsFile.is_open()) {
    while (getline(wordsFile, word)) {
      string first = word.substr(1, word.size());
      string second{word};
      second.erase(1, 1);

      if (d.homophones(word, first) && d.homophones(first, second)) {
          // printf("found\n");
          ret.push_back({word, first, second});
      }
      // printf("first: %s, second: %s, third: %s\n", word.c_str(), first.c_str(), second.c_str());
      // std::cout << word << std::endl;
    }
  }

  /* Your code goes here! */
  return ret;
}
