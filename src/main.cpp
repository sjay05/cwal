#include <bits/stdc++.h>
#include "walstream.h"
#include "walreader.h"
#include "walwriter.h"
#include "CRC/CRC.h"

using namespace std;

int main() {
  string path = "/home/sanjay-udt/dev/cwal/src/foo.txt";
/*  WalWriter w(FILE_FORMAT::BINARY);
  w.open_file(path);
  w.append_log(LogEntry(3, "Abc"));
  w.append_log(LogEntry(5, "hello"));
  w.append_log(LogEntry(1, "d"));
  w.wal_fsync();
  w.close_file(); */
/*  vector<LogEntry> e(3);
  WalReader r(FILE_FORMAT::BINARY);
  r.open_file(path);
  for (int i = 0; i < 3; i++) {
    r.read_log(e[i]);
    cout << e[i].byte_length << " " << e[i].data << '\n';
  }
  fstream fs;
  fs.open(path, ios::app | ios::in | ios::out | ios::binary);
  fs.seekg(0, ios::beg);
  uint64_t byte_len;
  fs.read(reinterpret_cast<char*>(&byte_len), sizeof(uint64_t));
  cout << byte_len << '\n';
  string s(byte_len, ' ');
  fs.read(&s[0], byte_len);
  cout << s << '\n';
  uint32_t crc;
  fs.read(reinterpret_cast<char*>(&crc), sizeof(uint32_t));
  cout << crc << '\n';
  cout << CRC::Calculate("c", 1, CRC::CRC_32()) << '\n';
  WalWriter w(FILE_FORMAT::BINARY);
  w.open_file(path);
  w.append_log(LogEntry(1, "c"));
  w.append_log(LogEntry(1, "d"));
  w.close_file();
  return 0; */
}                     
