//
// Created by Mac on 05.12.2023.
//

#ifndef LR1__PARSER_H_
#define LR1__PARSER_H_

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <list>
#include <fstream>
#include "Grammar.h"
#include "Reader.h"


using std::cin;
using std::cout;
using std::vector;
using std::shared_ptr;
using std::unique_ptr;
using std::string;
using std::unordered_map;
using std::unordered_multimap;
using std::list;

namespace ParserConstants {
const string rule_separator = "->";
const string rule_right_parts_separator = "|";
const char fictious_start_symbol = '#';
const char end_of_word = '$';
}

namespace ErrorMessages {
const string separator_expected = "Expected separator\n";
const string non_terminal_expected_rule =
    "There has to be a single non terminal at the left side of rules!\n";
const string bad_right_part_of_rule = "Right parts of rules must contain only terminals and non terminals!\n";
const string
    non_terminal_amount_doesnt_match = "Non terminal amount doesn't match\n";
const string terminal_amount_doesnt_match = "Terminal amount doesn't match\n";
const string
    start_symbol_not_non_terminal = "Start symbol must be non terminal!\n";
const string word_non_terminal = "Words must contain only terminal symbols!\n";
const string terminals_and_non_terminals_intersects = "Terminals and non terminals must not intersect with each other!\n";
}

struct ParseInfo {
  Grammar grammar;
  vector<string> words_to_check;
  ParseInfo() = default;
  ParseInfo(const Grammar& grammar, const vector<string>& words_to_check);
};

struct Parser {
  shared_ptr<InputReader> reader;
  Parser() = default;
  Parser(shared_ptr<InputReader> reader);
  virtual ParseInfo Parse() = 0;
};

struct BFParser : Parser {
  const string rule_parts_separator = ParserConstants::rule_separator;
  const string right_parts_separator = ParserConstants::rule_right_parts_separator;
  BFParser(shared_ptr<InputReader> reader) : Parser(reader) {
  }
  vector<char> ParseLetters(const string& line);
  void ParseRule(Grammar& grammar, string rule);
  void ParseRightPartOfRule(Grammar& grammar, char rule_non_terminal, const string& right_part_of_rule);
  vector<string> SplitBy(const string& to_split, const string& split_symbol);
  bool IsRightPartsOk(const Grammar& grammar, const vector<string>& right_parts);
  string trim(const string& string);
  void SkipSpaces(const string& string, size_t& index);
  virtual ParseInfo Parse() override;
};


#endif //LR1__PARSER_H_
