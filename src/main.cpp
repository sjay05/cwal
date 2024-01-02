#include <bits/stdc++.h>
#include "walstream.h"
#include "walreader.h"
#include "walwriter.h"

using namespace std;

int main() {
  string path = "/home/sanjay-udt/dev/cwal/src/foo.txt";
  WalWriter w;
  w.open_file(path);
  w.append_log(LogEntry(1, "c"));
  w.append_log(LogEntry(2, "d"));
  w.wal_fsync();
  w.close_file();
  return 0;
}                     
