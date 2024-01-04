#ifndef WALREADER_H
#define WALREADER_H

#include "walstream.h"

class WalReader : public WalStream {
public:
  WalReader() {}
  ~WalReader() {
    if (fs.is_open()) {
      fs.close();
    }
  }
  WalReader(FILE_FORMAT fmt) { file_format = fmt; }

  void open_file(const string& file_path);

  void reset_iter();

  bool read_log(LogEntry& log);
};

#endif
