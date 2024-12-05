#include "obsfusc.h"
int main(int argc, char *argv[]) {
  if (argc > 1) {
    srand(time(NULL));
    return gen_encrypted_files(argv[1]);
  } else {
    printf("Usage: %s <filename>\n", argv[0]);
    return 1;
  }
}
