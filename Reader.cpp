//
// Created by Mac on 05.12.2023.
//

#include "Reader.h"
#include "iostream"

using std::cin;

string ConsoleReader::ReadLine() {
  string to_return;
  getline(cin, to_return);
  return to_return;
}

string ConsoleReader::ReadString() {
  string to_return;
  cin >> to_return;
  return to_return;
}

char ConsoleReader::ReadSymbol() {
  char to_return;
  cin >> to_return;
  return to_return;
}

int ConsoleReader::ReadInt() {
  int to_return;
  cin >> to_return;
  return to_return;
}

void ConsoleReader::flush() {
  cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}


FileReader::FileReader(const std::string& path) : path_to_file(path) {
  file.open(path);
}

FileReader::~FileReader() {
  file.close();
}

string FileReader::ReadLine() {
  string answer;
  getline(file, answer);
  return answer;
}

int FileReader::ReadInt() {
  int answer;
  file >> answer;
  return answer;
}

char FileReader::ReadSymbol() {
  char answer;
  file >> answer;
  return answer;
}

string FileReader::ReadString() {
  string answer;
  file >> answer;
  return answer;
}

void FileReader::flush() {
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
