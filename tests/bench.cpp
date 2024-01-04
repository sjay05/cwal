#include "walstream.h"
#include "walreader.h"
#include "walwriter.h"

#include <iostream>
#include <chrono>
#include <random>

#define CLOCK_TIME() chrono::high_resolution_clock::now()

using namespace std;

static mt19937_64 rng((unsigned int) chrono::steady_clock::now().time_since_epoch().count());

const string alphanumeric = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const string path = "/home/sanjay-udt/dev/cwal/tests/test.txt"; 

template <typename T>
T benchmark_write(const int LOG_LENGTH, const int LOG_SIZE, bool RFLUSH, bool SYNC, const int SYNC_PERIOD = -1) {
  vector<LogEntry> logs;
  for (int i = 0; i < LOG_LENGTH; i++) {
    string put(LOG_SIZE, ' ');
    for (int j = 0; j < LOG_SIZE; j++) {
      put[j] = alphanumeric[rng() % (int) alphanumeric.size()];
    }
    logs.push_back(LogEntry(LOG_SIZE, put));
  }
  WalWriter w(FILE_FORMAT::BINARY);
  w.open_file(path);
  w.rotate_log();
  auto init_c = CLOCK_TIME();
  for (int i = 0; i < LOG_LENGTH; i++) {
    w.append_log(logs[i]);
    if (RFLUSH) {
      w.wal_flush();
    }
    if (SYNC) {
      if (i % SYNC_PERIOD == 0) {
        w.wal_fsync();
      }
    }
  }
  auto end_c = CLOCK_TIME();
  auto elap = chrono::duration_cast<chrono::microseconds>(end_c - init_c);
  return static_cast<T>(elap.count());
}

int main() {
  long long res = benchmark_write<long long>(5, 5, false, true, 1);
  return 0;
}