//
// Created by Mac on 05.12.2023.
//

#include "Parser.h"
#include "Utility.h"
#include "Grammar.h"

ParseInfo::ParseInfo(const Grammar& grammar,
                     const vector<std::string>& words_to_check) : grammar(grammar), words_to_check(words_to_check) {
}

Parser::Parser(shared_ptr<InputReader> reader) : reader(reader) {
}

ParseInfo BFParser::Parse() {
  Grammar grammar;
  int amount_of_non_terminals = reader->ReadInt();
  int amount_of_terminals = reader->ReadInt();
  int amount_of_rules = reader->ReadInt();
  grammar.non_terminals.resize(amount_of_non_terminals);
  grammar.terminals.resize(amount_of_terminals);
  reader->flush();
  string non_terminals = reader->ReadLine();
  grammar.non_terminals = ParseLetters(non_terminals);
  if (grammar.non_terminals.size() != amount_of_non_terminals) {
    throw std::runtime_error(ErrorMessages::non_terminal_amount_doesnt_match);
  }
  string terminals = reader->ReadLine();
  grammar.terminals = ParseLetters(terminals);
  if (grammar.terminals.size() != amount_of_terminals) {
    throw std::runtime_error(ErrorMessages::terminal_amount_doesnt_match);
  }
  if (Intersects(grammar.terminals, grammar.non_terminals)) {
    throw std::runtime_error(ErrorMessages::terminals_and_non_terminals_intersects);
  }
  vector<string> rules;
  for (int i = 0; i < amount_of_rules; ++i) {
    rules.emplace_back(reader->ReadLine());
  }
  for (int cur_rule = 0; cur_rule < amount_of_rules; ++cur_rule) {
    ParseRule(grammar, rules[cur_rule]);
  }
  grammar.start_symbol = reader->ReadSymbol();
  if (!Contains(grammar.non_terminals, grammar.start_symbol)) {
    throw std::runtime_error(ErrorMessages::start_symbol_not_non_terminal);
  }
  int amount_to_check = reader->ReadInt();
  vector<string> words_to_check;
  reader->flush();
  for (int i = 0; i < amount_to_check; ++i) {
    words_to_check.emplace_back(reader->ReadLine());
  }
  return ParseInfo(grammar, words_to_check);
}

vector<char> BFParser::ParseLetters(const std::string& line) {
  vector<char> answer;
  for (size_t i = 0; i < line.size(); ++i) {
    if (line[i] == ' ') {
      continue;
    }
    answer.emplace_back(line[i]);
  }
  return answer;
}

template<typename T, typename U>
bool Contains(const T& container, const U& object) {
  return std::find(container.begin(), container.end(), object) != container.end();
}

void BFParser::ParseRule(Grammar& grammar, std::string rule) {
  size_t cur_index = 0;
  SkipSpaces(rule, cur_index);
  if (cur_index >= rule.size() || !Contains(grammar.non_terminals,
                                            rule[cur_index])) {
    throw std::runtime_error(ErrorMessages::non_terminal_expected_rule);
  }
  char non_terminal = rule[cur_index];
  ++cur_index;
  SkipSpaces(rule, cur_index);
  if (cur_index + ParserConstants::rule_separator.size() > rule.size() ||
      rule.substr(cur_index, ParserConstants::rule_separator.size()) != ParserConstants::rule_separator) {
    throw std::runtime_error(ErrorMessages::separator_expected);
  }
  cur_index += ParserConstants::rule_separator.size();
  ParseRightPartOfRule(grammar, non_terminal, rule.substr(cur_index));
}

void BFParser::ParseRightPartOfRule(Grammar& grammar, char rule_non_terminal,
                                    const std::string& right_part_of_rule) {
  vector<string> right_parts = SplitBy(right_part_of_rule, ParserConstants::rule_right_parts_separator);
  if (!IsRightPartsOk(grammar, right_parts)) {
    throw std::runtime_error(ErrorMessages::bad_right_part_of_rule);
  }
  for (const auto& right_part : right_parts) {
    grammar.rules[rule_non_terminal].emplace_back(trim(right_part));
  }
}

vector<string> BFParser::SplitBy(const std::string& to_split,
                                 const string& split_symbol) {
  vector<string> answer;
  size_t cur_block_start = 0;
  for (size_t i = 0; i < to_split.size(); ++i) {
    if (to_split.substr(i, split_symbol.size()) == split_symbol) {
      answer.emplace_back(to_split.substr(cur_block_start, i - cur_block_start));
      cur_block_start = i + split_symbol.size();
    }
  }
  answer.emplace_back(to_split.substr(cur_block_start));
  return answer;
}

bool BFParser::IsRightPartsOk(const Grammar& grammar, const vector<std::string>& right_parts) {
  for (const auto& right_part : right_parts) {
    for (auto symbol : trim(right_part)) {
      if (!Contains(grammar.non_terminals, symbol) && !Contains(grammar.terminals,
                                                                symbol) && !isspace(symbol)) {
        return false;
      }
    }
  }
  return true;
}

string BFParser::trim(const std::string& string) {
  size_t cur_index = 0;
  SkipSpaces(string, cur_index);
  size_t answer_start = cur_index;
  cur_index = 0;
  std::string reversed = string;
  std::reverse(reversed.begin(), reversed.end());
  SkipSpaces(reversed, cur_index);
  size_t answer_end = string.size() - cur_index;
  return string.substr(answer_start, answer_end - answer_start);
}

void BFParser::SkipSpaces(const std::string& string, size_t& index) {
  while (index < string.size() && isspace(string[index])) {
    ++index;
  }
}
