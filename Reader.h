//
// Created by Mac on 05.12.2023.
//

#ifndef LR1__READER_H_
#define LR1__READER_H_

#include "string"
#include "fstream"

using std::string;


struct InputReader {
  virtual string ReadLine() = 0;
  virtual string ReadString() = 0;
  virtual char ReadSymbol() = 0;
  virtual int ReadInt() = 0;
  virtual void flush() = 0;
};

struct ConsoleReader : InputReader {
  virtual string ReadLine() override;
  virtual string ReadString() override;
  virtual char ReadSymbol() override;
  virtual int ReadInt() override;
  virtual void flush() override;
};

struct FileReader : InputReader {
  string path_to_file;
  std::ifstream file;
  FileReader() = default;
  FileReader(const string& path);
  ~FileReader();
  virtual string ReadLine() override;
  virtual string ReadString() override;
  virtual char ReadSymbol() override;
  virtual int ReadInt() override;
  virtual void flush() override;
};

#endif //LR1__READER_H_
