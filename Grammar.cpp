//
// Created by Mac on 05.12.2023.
//

#include "Grammar.h"
#include "Utility.h"
#include "Parser.h"
#include "set"

using std::set;

Grammar::Grammar(const vector<char>& non_terminals, const vector<char>& terminals,
                 unordered_map<char, vector<string>> rules, char start_symbol) :
    non_terminals(non_terminals), terminals(terminals),
    rules(rules), start_symbol(start_symbol) {
}

void Grammar::DeleteNonProducing() {
  for (auto terminal : terminals) {
    is_producing[terminal] = true;
  }
  for (auto non_terminal : non_terminals) {
    is_producing[non_terminal] = false;
  }
  is_producing[ParserConstants::end_of_word] = true;
  bool changed = true;
  while (changed) {
    changed = false;
    for (auto& rule : rules) {
      for (auto& right_part : rule.second) {
        if (IsProducing(right_part) && !is_producing[rule.first]) {
          is_producing[rule.first] = true;
          changed = true;
        }
      }
    }
  }
  set<char> non_producing;
  for (auto it : is_producing) {
    if (!it.second) {
      non_producing.emplace(it.first);
    }
  }
  for (auto it = rules.begin(); it != rules.end(); ++it) {
    for (int i = 0; i < it->second.size(); ++i) {
      if (Intersects(it->second[i], non_producing)) {
        it->second.erase(it->second.begin() + i);
      }
    }
  }
}

bool Grammar::IsProducing(const std::string& string) {
  for (auto letter : string) {
    if (!is_producing[letter]) {
      return false;
    }
  }
  return true;
}