#include "walreader.h"
#include "walstream.h"
#include "walwriter.h"
#include "CRC/CRC.h"

#include <iostream>
#include <random>
#include <cassert>

using namespace std;

static mt19937_64 rng((unsigned int) chrono::steady_clock::now().time_since_epoch().count());

void random_test_wal(const int sz, const int max_data_len) {
  static const string alphanumeric = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  static const string path = "/home/sanjay-udt/dev/cwal/tests/test.txt"; // works for .txt only
  vector<LogEntry> entries;
  const int size = sz;
  WalWriter w(FILE_FORMAT::BINARY);
  w.open_file(path);
  w.rotate_log();
  for (int i = 0; i < size; i++) {
    uint64_t byte_len = uniform_int_distribution<int>(1, max_data_len)(rng);
    string data(byte_len, ' ');
    for (int j = 0; j < byte_len; j++) {
      data[j] = alphanumeric[rng() % (int) alphanumeric.size()];
    }
    w.append_log(LogEntry(byte_len, data));
    entries.push_back(LogEntry(byte_len, data));
  }
  w.wal_fsync();
  w.close_file();
  WalReader r(FILE_FORMAT::BINARY);
  r.open_file(path);
  LogEntry tmp;
  for (int i = 0; i < (int) entries.size(); i++) {
    r.read_log(tmp);
    if (entries[i].byte_length != tmp.byte_length || entries[i].data != tmp.data) {
      cerr << "Mismatch found between:" << '\n';
      cerr << "Original: " << entries[i].byte_length << " " << entries[i].data << '\n';
      cerr << "Found: " << tmp.byte_length << " " << tmp.data << '\n';
    }
    assert(entries[i].byte_length == tmp.byte_length);
    assert(entries[i].data == tmp.data);
  }
//  cout << (r.fs.peek() == char_traits<char>::eof()) << '\n';
//  cout << (char) (r.fs.peek()) << '\n';
  cerr << "Passed Random WAL Test of " << sz << " entries with max length " << max_data_len << "." << '\n';
}

int main(int argc, char* argv[]) {
  rng.seed(5489); // default_seed;
  random_test_wal(1000, 1000);
  return 0;
}