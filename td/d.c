#include "d.h"

int main(int argc, char **argv) {
  printf("PATH: %s\nENC: %s\nKEY: %s\n", PATH, ENC_PATH, KEY_PATH);
  for (int i = 0; i < argc; i++) {
    switch (argv[i][1]) {
    case 'a':
      add_todo();
      break;
    case 'l':
      list_tasks();
      break;
    case 'r':
      remove_task_files();
      break;
    }
  }

  return 0;
}
