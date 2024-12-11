#include "TermStickyNote.h"

int main(int argc, char **argv) {
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
    case 'p':
      printf("PATH: %s\nENC: %s\nKEY: %s\n", PATH, ENC_PATH, KEY_PATH);
      break;
    case 'd':
      delete_task();
      break;
    }
  }

  return 0;
}
