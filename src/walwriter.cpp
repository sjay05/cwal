#include "walwriter.h"
#include <iostream>
#include <cassert>

using namespace std;

bool WalWriter::append_log(const LogEntry& log) {
  if (!fs.is_open()) {
    cerr << "file not opened correctly" << '\n';
    return false;
  }
  assert(fs.good() == true);
  if (file_format == FILE_FORMAT::BINARY) {
    if (static_cast<uint64_t>(log.data.size()) != log.byte_length) {
      cerr << "Incorrect byte_len specification" << '\n';
      return false;
    }
  }
  _append_log(log.data, log.byte_length);
  return true;
}

inline void WalWriter::flush() {
  if (fs.is_open() && fs.good()) {
    fs.flush();
  }
}

inline void WalWriter::rotate_log() {
  if (fs.is_open()) {
    fs.seekp(0, ios::beg);
  }
}

void WalWriter::_append_log(const string& data, uint64_t byte_len) {
 if (file_format == FILE_FORMAT::BINARY) {
   size_t size = data.size();
   fs.write(reinterpret_cast<char*>(&byte_len), sizeof(uint64_t));
   fs.write(const_cast<char*>(&data[0]), size);
   fs.put('\n');
   return;
 }
 assert(byte_len == (int) data.size());
 assert(file_format == FILE_FORMAT::TEXT);
 fs << byte_len << " " << data << '\n';
 return;
}

