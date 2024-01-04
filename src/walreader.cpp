#include "walreader.h"
#include "walstream.h"
#include "CRC/CRC.h"
#include <cassert>
#include <iostream>

using namespace std;

void WalReader::open_file(const string& file_path) {
  WalStream::open_file(file_path);
  fs.seekg(0, ios::beg);
}

void WalReader::reset_iter() {
  if (fs.is_open()) {
    fs.seekg(0, ios::beg);
  }
}

bool WalReader::read_log(LogEntry& log) {
  if (!fs.is_open() || !fs.good()) {
    cerr << "file not opened correctly" << '\n';
    return false;
  }
  uint32_t file_chksum;
  if (file_format == FILE_FORMAT::BINARY) {
    if (fs.peek() == char_traits<char>::eof()) {
      cerr << "reached end of file" << '\n';
      return false;
    }
    fs.read(reinterpret_cast<char*>(&log.byte_length), sizeof(uint64_t));
    log.data.resize(log.byte_length);
    fs.read(&log.data[0], log.byte_length);
    fs.read(reinterpret_cast<char*>(&file_chksum), sizeof(uint32_t));
  } else {
    fs >> log.byte_length;
    log.data.resize(log.byte_length);
    fs.seekg(1, ios::cur);
    fs.read(&log.data[0], log.byte_length);
    fs >> file_chksum;
  }
  uint32_t org_chksum = CRC::Calculate(log.data.c_str(), log.byte_length,
      WalStream::CRC32_TABLE);
  if (org_chksum != file_chksum) {
    throw invalid_argument("ERR: CRC32 Checksum does not match");
  }
  assert(org_chksum == file_chksum);
  return true;
}
