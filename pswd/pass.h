#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Lowercase letters: 97 - 122 */
/* Uppercase letters: 65 - 90 */
/* Numbers: 48 - 57 */
/* Special characters: 33 - 47, 58 - 64, 91 - 96, 123 - 126 */

#define LENGTH 250

char *gen_pass() {
  srand(time(NULL));
  char *pass = (char *)malloc(LENGTH * sizeof(char));
  pass[0] = '\0';
  char *buffer = (char *)malloc(LENGTH * LENGTH * sizeof(char));
  buffer[0] = pass[0];
  for (int j = 0; j < LENGTH * LENGTH; j++) {
    if (j % 2 == 0) {
      buffer[j] = 97 + rand() % 26;
    } else if (j % 3 == 0) {
      buffer[j] = 65 + rand() % 26;
    } else {
      buffer[j] = 48 + rand() % 10;
    }
  }

  for (int i = 0; i < LENGTH; i++) {
    strncat(pass, &buffer[rand() % (LENGTH * LENGTH)], 1);
  }
  free(buffer);
  return pass;
}
