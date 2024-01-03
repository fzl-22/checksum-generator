#pragma once

void calculate_hash(FILE *file, const EVP_MD *md, const char *algorithm, FILE *outputFile);

char hex_digit(unsigned char byte, int index);

int compare_checksum(FILE *file, const EVP_MD *md, const char *algorithm, FILE *inputFile);
