//
// Created by Mac on 05.12.2023.
//

#include "Lr1.h"
#include "Utility.h"
#include "stack"

using std::stack;

Rule::Rule(char non_terminal, string right_part) : non_terminal(non_terminal), right_part(right_part) {
}

Situation::Situation(Rule rule, int point_index, char next) : rule(rule), point_index(point_index), next(next) {
}

Situation::Situation(char non_terminal, string right_part, int point_index, char next) : rule(Rule(non_terminal, right_part)), point_index(point_index), next(next) {
}

bool SituationComparator::operator()(const Situation& first,
                                     const Situation& second) const {
  if (first.rule.non_terminal != second.rule.non_terminal) {
    return first.rule.non_terminal < second.rule.non_terminal;
  }
  if (first.rule.right_part != second.rule.right_part) {
    return first.rule.right_part < second.rule.right_part;
  }
  if (first.next != second.next) {
    return first.next < second.next;
  }
  return first.point_index < second.point_index;
}

LRTableAction::LRTableAction(LRTableActionType type, int next_vertice, int amount_to_reduce, char reduce_by) :
                      type(type), next_vertice(next_vertice), amount_to_reduce(amount_to_reduce), reduce_by(reduce_by) {
}

bool operator==(const LRTableAction& first, const LRTableAction& second) {
  return first.reduce_by == second.reduce_by && first.next_vertice == second.next_vertice && first.type == second.type;
}

LR1::LR1() : reader(std::make_shared<ConsoleReader>()), parser(std::make_shared<BFParser>(reader)), writer(std::make_shared<ConsoleWriter>()) {
}

LR1::LR1(const std::string& path) : reader(std::make_shared<FileReader>(path)), parser(std::make_shared<BFParser>(reader)), writer(std::make_shared<ConsoleWriter>()) {
}

LR1::LR1(const std::string& input_path, const std::string& output_path) : reader(std::make_shared<FileReader>(input_path)), parser(std::make_shared<BFParser>(reader)), writer(std::make_shared<FileWriter>(output_path)) {
}

void LR1::Init() {
  first.clear();
  epsilon_derivable.clear();
  vertices.clear();
  to.clear();
  table.clear();
  ParseInfo info = parser->Parse();
  words_to_check = info.words_to_check;
  Fit(info.grammar);
  CalcFirst();
  CalcSituations();
  CalcTable();
}

void LR1::Fit(const Grammar& gram) {
  grammar = gram;
  grammar.rules.emplace(ParserConstants::fictious_start_symbol, vector<string>(1, string(1, grammar.start_symbol)));
  AddFictiveEnd();
  grammar.DeleteNonProducing();
}

void LR1::AddFictiveEnd() {
  for (auto& rule : grammar.rules) {
    for (auto& right_part : rule.second) {
      right_part += ParserConstants::end_of_word;
    }
  }
}

void LR1::CalcFirst() {
  for (auto terminal : grammar.terminals) {
    first[terminal] = {terminal};
    epsilon_derivable[terminal] = false;
  }
  first[ParserConstants::end_of_word] = {};
  epsilon_derivable[ParserConstants::end_of_word] = true;
  bool changed = true;
  while (changed) {
    changed = false;
    for (auto& rule : grammar.rules) {
      char non_terminal = rule.first;
      size_t old_size = first[non_terminal].size();
      bool old_derivable = epsilon_derivable[non_terminal];
      for (auto& right_part : rule.second) {
        for (char letter : right_part) {
          if (Contains(grammar.non_terminals, letter)) {
            first[non_terminal].insert(first[letter].begin(), first[letter].end());
            if (!epsilon_derivable[letter]) {
              break;
            }
          } else if (Contains(grammar.terminals, letter)) {
            first[non_terminal].emplace(letter);
            break;
          } else {
            epsilon_derivable[non_terminal] = true;
          }
        }
      }
      changed = changed || first[non_terminal].size() != old_size || epsilon_derivable[non_terminal] != old_derivable;
    }
  }
  DeleteNonTerminalsFromFirst();
}

void LR1::DeleteNonTerminalsFromFirst() {
  for (auto& cur_first : first) {
    for (auto it = cur_first.second.begin(); it != cur_first.second.end(); ++it) {
      if (Contains(grammar.non_terminals, *it)) {
        it = cur_first.second.erase(it);
        --it;
      }
    }
  }
}

void LR1::CalcClosure(map<char, set<Situation, SituationComparator>>& situations) {
  bool changed = true;
  while (changed) {
    changed = false;
    for (auto& situation_set : situations) {
      for (auto& situation : situation_set.second) {
        for (auto& rule : grammar.rules[situation_set.first]) {
          for (auto first : FindFirst(situation.rule.right_part.substr(situation.point_index + 1, situation.rule.right_part.size() - 1) + situation.next)) {
            if (!situations[rule[0]].contains(Situation(situation_set.first, rule, 0, first))) {
              changed = true;
              situations[rule[0]].emplace(situation_set.first, rule, 0, first);
            }
          }
        }
      }
    }
  }
}

set<char> LR1::FindFirst(const std::string& string) {
  set<char> answer;
  bool is_epsilon_derivable = true;
  for (auto letter : string) {
    answer.insert(first[letter].begin(), first[letter].end());
    if (!epsilon_derivable[letter]) {
      is_epsilon_derivable = false;
      break;
    }
  }
  if (is_epsilon_derivable) {
    answer.emplace(ParserConstants::end_of_word);
  }
  return answer;
}

void LR1::CalcSituations() {
  vertices.resize(1);
  to.resize(1);
  vertices[0][grammar.start_symbol].emplace(ParserConstants::fictious_start_symbol, string(1, grammar.start_symbol) + ParserConstants::end_of_word, 0, ParserConstants::end_of_word);
  CalcClosure(vertices[0]);
  int cur_situation = 0;
  while (cur_situation < vertices.size()) {
    for (auto& situation_set : vertices[cur_situation]) {
      if (situation_set.first != ParserConstants::end_of_word) {
        size_t old_size = vertices.size();
        to[cur_situation][situation_set.first] = AddVertice(situation_set.second);
        if (vertices.size() != old_size) {
          --cur_situation;
          break;
        }
      }
    }
    ++cur_situation;
  }
}

bool operator==(const Situation& first, const Situation& second) {
  return first.rule.right_part == second.rule.right_part && first.rule.non_terminal == second.rule.non_terminal
      && first.point_index == second.point_index && first.next == second.next;
}

int LR1::AddVertice(const set<Situation, SituationComparator>& situations) {
  map<char, set<Situation, SituationComparator>> add_candidate;
  for (auto& situation : situations) {
    Situation to_add = situation;
    ++to_add.point_index;
    add_candidate[to_add.rule.right_part[to_add.point_index]].emplace(to_add);
  }
  CalcClosure(add_candidate);
  for (int i = 0; i < vertices.size(); ++i) {
    if (add_candidate == vertices[i]) {
      return i;
    }
  }
  vertices.emplace_back(add_candidate);
  to.emplace_back();
  return vertices.size() - 1;
}

void LR1::AddTableElementWithCheck(
    const LRTableAction& action, char edge_symbol,
    int vertice) {
  if (table[vertice].contains(edge_symbol) && table[vertice][edge_symbol] != action) {
    throw std::runtime_error(ErrorMessages::ActionConflict);
  }
  table[vertice][edge_symbol] = action;
}

void LR1::CalcTable() {
  table.resize(vertices.size());
  for (int i = 0; i < table.size(); ++i) {
    for (auto& vertice : vertices[i]) {
      if (vertice.first != ParserConstants::end_of_word) {
        AddTableElementWithCheck(LRTableAction(LRTableActionType::shift, to[i][vertice.first], -1, -1), vertice.first, i);
      } else {
        for (auto& situation : vertice.second) {
          if (situation == Situation(ParserConstants::fictious_start_symbol, string(1, grammar.start_symbol) + ParserConstants::end_of_word, 1, ParserConstants::end_of_word)) {
            AddTableElementWithCheck(LRTableAction(LRTableActionType::accept, -1, -1, -1), situation.next, i);
          } else {
            AddTableElementWithCheck(LRTableAction(LRTableActionType::reduce, -1, situation.rule.right_part.size() - 1, situation.rule.non_terminal), situation.next, i);
          }
        }
      }
    }
  }
}

void LR1::CheckWords() {
  for (auto& word : words_to_check) {
    writer->Write(CheckWord(word) ? "YES" : "NO");
    writer->EndLine();
  }
}

bool LR1::CheckWord(std::string to_check) {
  to_check += ParserConstants::end_of_word;
  stack<int> vertices_indexes;
  stack<char> letters;
  vertices_indexes.push(0);
  size_t i = 0;
  while (i < to_check.size()) {
    if (!table[vertices_indexes.top()].contains(to_check[i])) {
      return false;
    }
    switch (table[vertices_indexes.top()][to_check[i]].type) {
      case LRTableActionType::accept :
        return true;
      case LRTableActionType::shift :
        vertices_indexes.push(table[vertices_indexes.top()][to_check[i]].next_vertice);
        letters.push(to_check[i]);
        ++i;
        break;
      case LRTableActionType::reduce:
        char reduce_by = table[vertices_indexes.top()][to_check[i]].reduce_by;
        int amount_to_reduce = table[vertices_indexes.top()][to_check[i]].amount_to_reduce;
        for (int i = 0; i < amount_to_reduce; ++i) {
          vertices_indexes.pop();
          letters.pop();
        }
        if (!table[vertices_indexes.top()].contains(reduce_by) ||
            table[vertices_indexes.top()][reduce_by].type != LRTableActionType::shift) {
          return false;
        }
        letters.push(reduce_by);
        vertices_indexes.push(table[vertices_indexes.top()][reduce_by].next_vertice);
        break;
    }
  }
  return false;
}
