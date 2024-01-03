# CWAL

A write ahead log implementation in C++, with support for fast sequential file IO and CRC32 Checksums to preserve file integrity. 

## Build
The main dependencies include `cmake` (>= Version 3.22) and `g++/gcc` with support for C++17 and onwards. For Linux systems, run
```
git clone https://github.com/sjay05/cwal.git
cd cwal

cmake -S . -B <build-folder>
cd <build-folder>/

make
```

CWAL can also be imported as a Git submodule in a CMake project with `git submodule add https://github.com/sjay05/cwal`.

## LogEntry Format
Log entries consist of of a `byte_len` representing the size of `data` in bytes, and a 4 byte CRC Checksum value (`CRC_CHECKSUM`).
```
----|-------------------|--------------------|-----------------------|-----
... | uint64_t byte_len | const string* data | uint32_t CRC_CHECKSUM | ... 
    | (8 bytes)         | (byte_len bytes)   | (4 bytes)             |
----|-------------------|--------------------|-----------------------|-----
```

## Example
```
#include "walstream.h"
#include "walreader.h"
#include "walwriter.h"

#include <iostrream>
#include <vector>

int main() {
  WalWriter w(FILE_FORMAT::BINARY); // support for FILE_FORMAT::TEXT also
  w.open_file("foo.txt");
  w.append_log(LogEntry(12, "Log Entry #1")) // <length in bytes>, <log message>
  w.append_log(LogEntry(12, "Log Entry #2")) 
  w.append_log(LogEntry(12, "Log Entry #3")) 
  w.wal_fsync(); // Synchronize log file with storage device
  w.rotate_log(); // Seek to ios::beg of file to overwrite.
  w.close_file();

  WalReader r(FILE_FORMAT::BINARY);
  r.open_file("foo.txt");
  vector<LogEntry> logs(3);
  for (int i = 0; i < 3; i++) {
    r.read_log(logs[i]);
    // provided with logs[i].byte_length & logs[i].data
  }
  r.close_file();
}
```