#ifndef WALWRITER_H
#define WALWRITER_h

#include "walstream.h"

class WalWriter : public WalStream {
public:
  WalWriter() {}
  ~WalWriter() {
    if (fs.is_open()) {
      fs.close();
    }
  }
  WalWriter(FILE_FORMAT fmt) { file_format = fmt; }

  bool append_log(const LogEntry& log);

  inline void wal_flush();
  void wal_fsync();
  inline void rotate_log();

private:
  void _append_log(const string& data, uint64_t byte_len);
};


#endif
