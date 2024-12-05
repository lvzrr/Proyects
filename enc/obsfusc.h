/* obfusc.h */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int gen_encrypted_files(const char *filename) {
  FILE *fr;
  FILE *fw;
  FILE *fw2;
  char *keyfilename = (char *)malloc(256);
  char *encryptedfile = (char *)malloc(256);

  /*
   * strip filename extension and add .key
   */

  strcpy(keyfilename, filename);
  strcpy(encryptedfile, keyfilename);
  strcat(keyfilename, ".key");
  strcat(encryptedfile, ".crypt");

  /*
   * open files
   */

  fr = fopen(filename, "rb");
  if (fr == NULL) {
    printf("file %s not found\n", filename);
    return 1;
  }
  fw = fopen(keyfilename, "wb");
  fw2 = fopen(encryptedfile, "wb");

  if (fw == NULL || fw2 == NULL) {
    printf("failed to open file %s or %s\n", keyfilename, encryptedfile);
    return 1;
  }
  /*
   * encrypt with XOR gate and random chars
   */

  while (1) {
    int to_encrypt = fgetc(fr);
    if (to_encrypt == EOF) {
      break;
    }
    int rand_byte = rand() % 256;
    fputc(rand_byte, fw);
    fputc(rand_byte ^ to_encrypt, fw2);
  }
  fclose(fr);
  fclose(fw);
  remove(filename);
  return 0;
}
int decrypt_file(char *encryptedfilename) {

  FILE *keyfile;
  FILE *encryptedfile;
  FILE *decryptedfile;

  char *keyfilename = (char *)malloc(256);
  char *outfilename = (char *)malloc(256);

  /*
   * strip filename extensions .key and .crypt to get (output) filename
   */
  strcpy(keyfilename, encryptedfilename);
  char *ext = strrchr(keyfilename, '.');
  if (ext && strcmp(ext, ".crypt") == 0) {
    *ext = '\0';
  }
  strcpy(outfilename, keyfilename);
  strcat(keyfilename, ".key");

  encryptedfile = fopen(encryptedfilename, "rb");
  if (encryptedfile == NULL) {
    printf("encrypted file %s not found\n", keyfilename);
    return 1;
  }
  keyfile = fopen(keyfilename, "rb");
  if (keyfile == NULL) {
    printf("key file %s not found\n", keyfilename);
    return 1;
  }
  decryptedfile = fopen(outfilename, "wb");
  if (decryptedfile == NULL) {
    printf("failed to open file %s\n", outfilename);
    return 1;
  }
  /*
   *    Decrypt using XOR ant the key
   */

  while (1) {
    int encrypted_byte = fgetc(encryptedfile);
    int key_byte = fgetc(keyfile);
    if (encrypted_byte == EOF || key_byte == EOF) {
      break;
    }
    fputc(encrypted_byte ^ key_byte, decryptedfile);
  }

  /*
   *  close files
   */

  fclose(keyfile);
  fclose(encryptedfile);
  fclose(decryptedfile);

  /*
   *  delete key and encrypted file
   */

  remove(keyfilename);
  remove(encryptedfilename);
  return 0;
}
