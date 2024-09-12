#include "gerador.h"
#include <stdio.h>
#include "quicksort.h"

void read_and_print_records(const char *filename) {
     FILE *input_file;
     TipoRegistro record;

    // Open the binary file
     input_file = fopen(filename, "rb");
     if (input_file == NULL) {
         perror("Error opening input file");
         return;
     }
     int i = 0;
     // Read records from the binary file and print them
     while (i < 1000) {
        fread(&record, sizeof(TipoRegistro), 1, input_file);
        printf("%ld %.2f %s %s %s\n", record.inscricao, record.nota, record.estado, record.cidade, record.curso);
        i++;
     }

    // Close the file
     fclose(input_file);

 }


 void txtParaBin(const char *txt, const char *bin){
    FILE *input_file, *output_file;
    char line[MAX_LINE_LENGTH];
    TipoRegistro record;

    // Open input and output files
    input_file = fopen(txt, "r");
    if (input_file == NULL) {
        perror("Error opening input file");
        return;
    }
    output_file = fopen(bin, "wb");
    if (output_file == NULL) {
        perror("Error opening output file");
        fclose(input_file);
        return;
    }

    // Read lines from input file and write to binary file
    while (fgets(line, MAX_LINE_LENGTH, input_file) != NULL) {
        // Parse the line into the record struct
        if (sscanf(line, "%8ld %5f %2s %50s %30s", &record.inscricao, &record.nota, record.estado, record.cidade, record.curso) != 5) {
            fprintf(stderr, "Error parsing line: %s\n", line);
            continue;
        }

        // Write the record to the binary file
        if (fwrite(&record, sizeof(TipoRegistro), 1, output_file) != 1) {
            perror("Error writing to output file");
            break;
        }
    }

    //read_and_print_records("PROVAO.dat");

    printf("------------Arquivo gerado com sucesso--------------\n");

    // Close files
    fclose(input_file);
    fclose(output_file);
 }
