#include <FileSystem.hpp>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    Filesystem Tested(".");
    Tested.PrintAllFiles();
    return 0;
  } else {
    Filesystem Tested(argv[1]);
    Tested.PrintAllFiles();
    Tested.PrintTotalInformation();
  }}
