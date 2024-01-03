#include <openssl/evp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash-calculator/hash-calculator.h"

#define PROGRAM_VERSION "1.0.0"

int main(int argc, char *argv[]) {
  if (argc == 2 && strcmp(argv[1], "--version") == 0) {
    printf("checksum-gen v%s\n", PROGRAM_VERSION);
    return EXIT_SUCCESS;
  }

  if ((argc != 4 && argc != 6 && argc != 7) || strcmp(argv[1], "-a") != 0) {
    fprintf(stderr,
            "Usage: %s -a <algorithm> <filename> [-v <checksum_file>]\n",
            argv[0]);
    return EXIT_FAILURE;
  }

  const char *algorithm = argv[2];
  const EVP_MD *md;

  if (strcmp(algorithm, "md5") == 0) {
    md = EVP_md5();
  } else if (strcmp(algorithm, "sha256") == 0) {
    md = EVP_sha256();
  } else if (strcmp(algorithm, "sha512") == 0) {
    md = EVP_sha512();
  } else {
    fprintf(stderr, "Unsupported algorithm: %s\n", algorithm);
    return EXIT_FAILURE;
  }

  FILE *file = fopen(argv[3], "rb");
  if (file == NULL) {
    perror("Error opening file");
    return EXIT_FAILURE;
  }

  if (argc == 6 && strcmp(argv[4], "-v") == 0) {
    FILE *inputFile = fopen(argv[5], "r");
    if (inputFile == NULL) {
      perror("Error opening checksum file");
      fclose(file);
      return EXIT_FAILURE;
    }

    int result = compare_checksum(file, md, algorithm, inputFile);

    if (result) {
      printf("Checksums match.\n");
    } else {
      printf("Checksums do not match.\n");
    }

    fclose(inputFile);
  } else if (argc == 7 && strcmp(argv[4], "-v") == 0) {
    FILE *verifyFile = fopen(argv[5], "r");
    if (verifyFile == NULL) {
      perror("Error opening verify checksum file");
      fclose(file);
      return EXIT_FAILURE;
    }

    int result = compare_checksum(file, md, algorithm, verifyFile);

    if (result) {
      printf("Checksums match.\n");
    } else {
      printf("Checksums do not match.\n");
    }

    fclose(verifyFile);
  } else {
    char outputFileName[50];
    snprintf(outputFileName, sizeof(outputFileName), "%s-checksum.txt",
             algorithm);
    FILE *outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
      perror("Error opening output file");
      fclose(file);
      return EXIT_FAILURE;
    }

    calculate_hash(file, md, algorithm, outputFile);

    fclose(outputFile);
  }

  fclose(file);

  return EXIT_SUCCESS;
}
