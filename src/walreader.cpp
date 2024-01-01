#include "walreader.h"
#include <cassert>
#include <iostream>

using namespace std;

void WalReader::open_file(const string& file_path) {
  WalStream::open_file(file_path);
  fs.seekg(0, ios::beg);
}

inline void WalReader::reset_iter() {
  if (fs.is_open()) {
    fs.seekg(0, ios::beg);
  }
}

bool WalReader::read_log(LogEntry& log) {
  if (!fs.is_open()) {
    cerr << "file not opened correctly" << '\n';
    return false;
  }
  assert(fs.good() == true);
  if (file_format == FILE_FORMAT::BINARY) {
    fs.read(reinterpret_cast<char*>(&log.byte_length), sizeof(uint64_t));
    log.data.resize(log.byte_length);
    fs.read(&log.data[0], log.byte_length);
    fs.seekg(sizeof(uint64_t) + log.byte_length, ios::cur);
    return true;
  }      
  fs >> log.byte_length;
  log.data.resize(log.byte_length);
  fs.seekg(1, ios::cur);
  fs.read(&log.data[0], log.byte_length);
  return true;
}
