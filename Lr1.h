//
// Created by Mac on 05.12.2023.
//

#ifndef LR1__LR1_H_
#define LR1__LR1_H_

#include "Grammar.h"
#include "unordered_map"
#include "map"
#include "set"
#include "vector"
#include "Reader.h"
#include "Parser.h"
#include "Writer.h"

using std::map;
using std::unordered_map;
using std::set;
using std::vector;

namespace ErrorMessages {
  static const string ActionConflict = "There are too many actions in LR table!\n The grammar is not LR1!\n";
}

struct Rule {
  char non_terminal;
  string right_part;
  Rule() = default;
  Rule(char non_terminal, string right_part);
  Rule(const Rule& other) = default;
};

struct Situation {
  Rule rule;
  int point_index;
  char next;

  Situation() = default;
  Situation(Rule rule, int point_index, char next);
  Situation(char non_terminal, string right_part, int point_index, char next);

};

bool operator==(const Situation& first, const Situation& second);

struct SituationComparator {
  bool operator()(const Situation& first, const Situation& second) const;
};

enum LRTableActionType {
  shift, reduce, accept
};

struct LRTableAction {
  LRTableActionType type;
  int next_vertice;
  int amount_to_reduce;
  char reduce_by;


  LRTableAction() = default;
  LRTableAction(LRTableActionType type, int next_vertice, int amount_to_reduce, char reduce_by);
};

bool operator==(const LRTableAction& first, const LRTableAction& second);

struct LR1 {
  shared_ptr<InputReader> reader;
  shared_ptr<Parser> parser;
  shared_ptr<Writer> writer;
  Grammar grammar;
  std::map<char, set<char>> first;
  std::map<char, bool> epsilon_derivable;
  vector<map<char, set<Situation, SituationComparator>>> vertices; // key in map - first symbol after dot
  vector<map<char, int>> to;
  vector<map<char, LRTableAction>> table;
  vector<string> words_to_check;

  LR1();
  LR1(const string& path);
  LR1(const string& input_path, const string& output_path);

  void Init();
  void Fit(const Grammar& gram);

  void AddFictiveEnd();

  void CalcFirst();
  void DeleteNonTerminalsFromFirst();

  void CalcSituations();
  void CalcClosure(map<char, set<Situation, SituationComparator>>& situations);
  set<char> FindFirst(const string& string);
  int AddVertice(const set<Situation, SituationComparator>& situations);

  void CalcTable();
  void AddTableElementWithCheck(const LRTableAction& action,
                           char edge_symbol, int vertice);

  void CheckWords();
  bool CheckWord(string to_check);
};

#endif //LR1__LR1_H_
