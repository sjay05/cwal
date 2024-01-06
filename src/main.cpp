#include "walstream.h"
#include "walreader.h"
#include "walwriter.h"
#include "CRC/CRC.h"

// extern C
#include <fcntl.h>
#include <unistd.h>

using namespace std;

int main() {
  const char* path = "/home/sanjay-udt/dev/cwal/build/foo.txt";
  int fd = open(path, O_RDWR | O_CREAT | O_DIRECTORY);
  close(fd);
  return 0;
}                     
