#include <gtest/gtest.h>
#include "Lr1.h"

bool AreIdentical(std::ifstream& first, std::fstream& second) {
  string ans_left, ans_right;
  while (first >> ans_left && second >> ans_right) {
    if (ans_right != ans_left) {
      return false;
    }
  }
  return true;
}

TEST(lr1, hard_tests) {
  for (int i = 1; i < TestsAmount + 1; ++i) {
    char* path = (char*) malloc(1024);
    std::fstream out;
    snprintf(path, 1024, "%s/test%d.txt", TestDataPath, i);
    string test_path = path;
    snprintf(path, 1024, "%s/out%d.txt", TestOutputPath, i);
    out.open(path, std::ios::out | std::ios::in);

    string output_path = path;
    LR1 solution(test_path, output_path);
    try {
      solution.Init();
      solution.CheckWords();
    } catch (...) {
      out << "ERROR";
    }

    std::ifstream answers;
    snprintf(path, 1024, "%s/ans%d.txt", TestAnswerPath, i);
    string answer_path = path;
    answers.open(answer_path);
    out.close();
    snprintf(path, 1024, "%s/out%d.txt", TestOutputPath, i);
    out.open(path, std::ios::out | std::ios::in);
    ASSERT_TRUE(AreIdentical(answers, out));

    out.close();
    answers.close();
    free(path);
  }
}
