//
// Created by Mac on 05.12.2023.
//

#ifndef LR1__GRAMMAR_H_
#define LR1__GRAMMAR_H_

#include "vector"
#include "unordered_map"
#include "string"
#include "map"

using std::vector;
using std::unordered_map;
using std::string;
using std::map;

struct Grammar {
  vector<char> non_terminals;
  vector<char> terminals;
  unordered_map<char, vector<string>> rules;
  char start_symbol;
  map<char, bool> is_producing;
  Grammar() = default;
  Grammar(const vector<char>& non_terminals, const vector<char>& terminals,
          unordered_map<char, vector<string>> rules, char start_symbol);

  void DeleteNonProducing();
  bool IsProducing(const string& string);
};

#endif //LR1__GRAMMAR_H_
