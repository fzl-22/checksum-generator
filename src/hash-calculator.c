#include <openssl/evp.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

void calculate_hash(FILE *file, const EVP_MD *md, const char *algorithm,
                    FILE *outputFile) {
  EVP_MD_CTX *mdctx;
  unsigned char digest[EVP_MAX_MD_SIZE];
  int bytesRead;
  unsigned char buffer[BUFFER_SIZE];

  mdctx = EVP_MD_CTX_new();
  EVP_DigestInit_ex(mdctx, md, NULL);

  while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, file)) != 0) {
    EVP_DigestUpdate(mdctx, buffer, bytesRead);
  }

  EVP_DigestFinal_ex(mdctx, digest, NULL);
  EVP_MD_CTX_free(mdctx);

  fprintf(outputFile, "%s checksum: ", algorithm);
  for (int i = 0; i < EVP_MD_size(md); i++) {
    fprintf(outputFile, "%02x", digest[i]);
  }
  fprintf(outputFile, "\n");
}

char hex_digit(unsigned char byte, int index);

int compare_checksum(FILE *file, const EVP_MD *md, const char *algorithm,
                     FILE *inputFile) {
  EVP_MD_CTX *mdctx;
  unsigned char digest[EVP_MAX_MD_SIZE];
  int bytesRead;
  unsigned char buffer[BUFFER_SIZE];

  mdctx = EVP_MD_CTX_new();
  EVP_DigestInit_ex(mdctx, md, NULL);

  while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, file)) != 0) {
    EVP_DigestUpdate(mdctx, buffer, bytesRead);
  }

  EVP_DigestFinal_ex(mdctx, digest, NULL);
  EVP_MD_CTX_free(mdctx);

  rewind(inputFile);

  char storedChecksum[EVP_MAX_MD_SIZE * 2 + 1];
  if (fscanf(inputFile, "%*s %*s %s", storedChecksum) != 1) {
    fprintf(stderr, "Error reading stored checksum from file\n");
    return 0;
  }

  for (int i = 0; i < EVP_MD_size(md) * 2; i++) {
    if (storedChecksum[i] != hex_digit(digest[i / 2], i % 2)) {
      return 0;
    }
  }

  return 1; 
}

char hex_digit(unsigned char byte, int index) {
  // Convert a single byte to a hexadecimal character
  static const char hex_chars[] = "0123456789abcdef";
  return hex_chars[(byte >> (4 * (1 - index))) & 0xF];
}
