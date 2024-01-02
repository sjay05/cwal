#ifndef WALSTREAM_H
#define WALSTREAM_H

#include <fstream>
#include <string>
#include <filesystem>

using namespace std;

enum class FILE_FORMAT : uint32_t {
  BINARY,
  TEXT,
};

class WalStream {
public:
  string file_path;
  FILE_FORMAT file_format = FILE_FORMAT::TEXT;
  fstream fs;

  bool open_file(const string& file);

  void close_file();
  void set_file_format(FILE_FORMAT f);
};

struct LogEntry {
  uint64_t byte_length = 0;
  string data;

  LogEntry(uint64_t len, string& info) : byte_length{len}, data{info} {}
  LogEntry(uint64_t len, string info) : byte_length{len}, data{info} {}
  LogEntry(string s) : byte_length{0}, data{s} {}
  LogEntry(string& info) : byte_length{0}, data{info} {}
  LogEntry() {}
  ~LogEntry() {}
};

#endif
