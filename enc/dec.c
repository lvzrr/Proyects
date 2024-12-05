#include "obsfusc.h"
int main(int argc, char *argv[]) {
  if (argc > 1) {
    return decrypt_file(argv[1]);
  } else {
    printf("Usage: %s <filename>\n", argv[0]);
    return 1;
  }
}
