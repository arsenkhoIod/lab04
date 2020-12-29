#include <FileSystem.hpp>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    MyFilesystem Tested(".");
    Tested.PrintAllFiles();
    return 0;
  } else {
    MyFilesystem Tested(argv[1]);
    Tested.PrintAllFiles();
    Tested.PrintTotalInformation();
  }}