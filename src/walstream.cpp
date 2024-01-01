#include "walstream.h"

bool WalStream::open_file(const string& file_path) {
  // open in app | in | out to allow for multiple line IO
  auto mode = ios::app | ios::out;
  if (filesystem::exists(file_path)) {
    mode |= ios::in;
  }
  if (file_format == FILE_FORMAT::BINARY) {
    mode |= ios::binary;
  }
  fs.open(file_path, mode);
  return static_cast<bool>(fs.good());
}

void WalStream::close_file() {
  if (fs.is_open()) {
    fs.close();
  }
}

void WalStream::set_file_format(FILE_FORMAT f) { file_format = f; }
