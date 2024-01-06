#include "walstream.h"
#include "walreader.h"
#include "walwriter.h"

#include <iostream>
#include <chrono>
#include <random>
#include <cassert>

#define CLOCK_TIME() chrono::high_resolution_clock::now()

using namespace std;

static mt19937_64 rng((unsigned int) chrono::steady_clock::now().time_since_epoch().count());

const string alphanumeric = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const string test_file_path = "/home/sanjay-udt/dev/cwal/tests/test.txt"; 

template <typename T>
static T benchmark_write(const int LOG_LENGTH, const int LOG_SIZE, bool RFLUSH, bool SYNC, const int SYNC_PERIOD = -1) {
  vector<LogEntry> logs;
  for (int i = 0; i < LOG_LENGTH; i++) {
    string put(LOG_SIZE, ' ');
    for (int j = 0; j < LOG_SIZE; j++) {
      put[j] = alphanumeric[rng() % (int) alphanumeric.size()];
    }
    logs.push_back(LogEntry(LOG_SIZE, put));
  }
  WalWriter w(FILE_FORMAT::BINARY);
  w.open_file_truncated(test_file_path);
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
  w.close_file();
  return static_cast<T>(elap.count());
}

static void regular_benchmark(const int LOG_LENGTH, const int LOG_SIZE, bool RFLUSH, bool SYNC, const int SYNC_PERIOD = -1) {
  long long res = benchmark_write<long long>(LOG_LENGTH, LOG_SIZE, RFLUSH, SYNC, SYNC_PERIOD);
  cout << "Regular Benchmark: " << LOG_LENGTH << " entries | " << "data_length = " << LOG_SIZE << " | ";
  cout << "Flush? " << (RFLUSH ? "Yes" : "No") << " | " << "Sync? " << (SYNC ? "Yes" : "No");
  if (SYNC) {
    cout << " | SYNC_PERIOD = " << SYNC_PERIOD;
  }
  cout << '\n';
  cout << "==> ";
  cout << fixed << setprecision(2);
  cout << res << " ms" << " | " << (double) res / LOG_LENGTH << " ms/log"<< '\n';
  cout << '\n';
}

// RFLUSH = 0, SYNC = 1, SYNC_PERIOD = 1
static void batched_sync_benchmarks(const int LOG_LENGTH, const int LOG_SIZE, const int BATCH_SIZE) {
  assert(BATCH_SIZE <= LOG_LENGTH);
  assert(BATCH_SIZE != 0);
  vector<LogEntry> logs;
  WalWriter w(FILE_FORMAT::BINARY);
  w.open_file_truncated(test_file_path);
  w.rotate_log();
  for (int i = 0; i < LOG_LENGTH; i++) {
    string put(LOG_SIZE, ' ');
    for (int j = 0; j < LOG_SIZE; j++) {
      put[j] = alphanumeric[rng() % (int) alphanumeric.size()];
    }
    logs.push_back(LogEntry(LOG_SIZE, put));
  }
  int rem = LOG_LENGTH % BATCH_SIZE;
  auto init_c = CLOCK_TIME();
  for (int i = 0; i < LOG_LENGTH; i++) {
    if (i % BATCH_SIZE == 0) {
      w.rotate_log();
    }
    w.append_log(logs[i]);
    w.wal_fsync();
  }
  auto end_c = CLOCK_TIME();
  auto elap = chrono::duration_cast<chrono::microseconds>(end_c - init_c);
  w.close_file();
  long long elapsed_time = static_cast<long long>(elap.count());
  cout << "Batched Sync Benchmark: " << LOG_LENGTH << " entries | " << "data_length = " << LOG_SIZE << " | ";
  cout << "BATCH_SIZE = " << BATCH_SIZE << '\n';
  cout << "==> ";
  cout << fixed << setprecision(2);
  cout << elapsed_time << " ms" << " | " << (double) elapsed_time / LOG_LENGTH << " ms/log" << '\n';
}

int main() {
  regular_benchmark(1000, 1000, false, true, 1);
  batched_sync_benchmarks(1000, 1000, (int) sqrt(1000));
  // display_benchmark(1000, 1000, false, false);
  // display_benchmark(1000, 1000, true, false);
  // display_benchmark(1000, 1000, false, true, 1);
  // long long res = benchmark_write<long long>(1000, 1000, false, true, 1);
  // cout << res << " mircoseconds" << '\n';
  // res = benchmark_write<long long>(1000, 1000, false, false);
  // cout << res << " microseconds" << '\n';
  return 0;
}