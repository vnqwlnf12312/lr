//
// Created by Mac on 05.12.2023.
//

#ifndef LR1__WRITER_H_
#define LR1__WRITER_H_


#include "string"
#include "fstream"

using std::string;

struct Writer {
  virtual void Write(const string& text) = 0;
  virtual void EndLine() = 0;
};

struct ConsoleWriter : Writer {
  virtual void Write(const string& text) override;
  virtual void EndLine() override;
};

struct FileWriter : Writer {
  string path_to_file;
  std::ofstream file;
  FileWriter() = default;
  FileWriter(const string& path);
  ~FileWriter();
  virtual void Write(const string& text) override;
  virtual void EndLine() override;
};

#endif //LR1__WRITER_H_
