#ifndef WALSTREAM_H
#define WALSTREAM_H

#include <fstream>
#include <string>
#include <filesystem>
#include "CRC/CRC.h"

using namespace std;

enum class FILE_FORMAT : uint32_t {
  BINARY,
  TEXT, // for debugging purposes, only with -DDEBUG defn enabled
};

class WalStream {
public:
  string file_path;
#ifdef LOCAL_DEBUG
  FILE_FORMAT file_format = FILE_FORMAT::TEXT;
#else
  FILE_FORMAT file_format = FILE_FORMAT::BINARY;
#endif
  static inline auto CRC32_TABLE { CRC::CRC_32().MakeTable() };
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
