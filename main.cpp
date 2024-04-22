//
// Created by Mac on 05.12.2023.
//

#include "iostream"
#include "Lr1.h"

int main() {
  string input = "/Users/mac/CLionProjects/lr1/tests/data/input/test7.txt";
  string output = "/Users/mac/CLionProjects/lr1/tests/data/output/out7.txt";
  LR1 solution(input, output);
  solution.Init();
  solution.CheckWords();
}