//
// Created by Mac on 05.12.2023.
//

#include "Writer.h"
#include "iostream"

using std::cout;

void ConsoleWriter::Write(const std::string& text) {
  cout << text << std::endl;
}

void ConsoleWriter::EndLine() {
  cout << std::endl;
}

FileWriter::FileWriter(const std::string& path) : path_to_file(path) {
  file.open(path);
}

FileWriter::~FileWriter() {
  file.close();
}

void FileWriter::Write(const std::string& text) {
  file << text;
}

void FileWriter::EndLine() {
 file << std::endl;
}