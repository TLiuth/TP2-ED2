#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quicksort.h"
#include "lista.h"




 #define MAX_LINE_LENGTH 1024

typedef struct {
    long int id;
    float value;
    char state[2];
    char city[50];
    char curso[30]; // Assuming the course name is at most 50 characters
} Record;

void read_and_print_records(const char *filename);

int main() {
    FILE *input_file, *output_file;
    char line[MAX_LINE_LENGTH];
    Record record;

    // Open input and output files
    input_file = fopen("PROVAO.TXT", "r");
    if (input_file == NULL) {
        perror("Error opening input file");
        return 1;
    }
    output_file = fopen("PROVAO.dat", "wb");
    if (output_file == NULL) {
        perror("Error opening output file");
        fclose(input_file);
        return 1;
    }

    // Read lines from input file and write to binary file
    while (fgets(line, MAX_LINE_LENGTH, input_file) != NULL) {
        // Parse the line into the record struct
        if (sscanf(line, "%8ld %5f %2s %50s %30s", &record.id, &record.value, record.state, record.city, record.curso) != 5) {
            fprintf(stderr, "Error parsing line: %s\n", line);
            continue;
        }

        // Write the record to the binary file
        if (fwrite(&record, sizeof(Record), 1, output_file) != 1) {
            perror("Error writing to output file");
            break;
        }
    }

    // Close files
    fclose(input_file);
    fclose(output_file);

    read_and_print_records("PROVAO.dat");

    return 0;
}


void read_and_print_records(const char *filename) {
     debugger("Entrou em read and print records");
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
     while (i < 100) {
        fread(&record, sizeof(TipoRegistro), 1, input_file);
        printf("%ld %.2f %s %s %s\n", record.inscricao, record.nota, record.estado, record.cidade, record.curso);
        i++;
     }

    // Close the file
     fclose(input_file);

 }


// int main(){
//     debugger("Entrou na main");
//     FILE *ArqLEs, *ArqLi, *ArqEi;
//     ArqLi = fopen("PROVAO.dat", "r+b");
//     if(ArqLi == NULL){
//         printf("Erro na abertura do arquivo!");
//         exit(1);
//     }
//     ArqEi = fopen("PROVAO.dat", "r+b");
//     if(ArqEi == NULL){
//         printf("Erro na abertura do arquivo!");
//         exit(1);
//     }
//     ArqLEs = fopen("PROVAO.dat", "r+b");
//     if(ArqLEs == NULL){
//         printf("Erro na abertura do arquivo!");
//         exit(1);
//     }
//     debugger("Abriu ponteiros");

//     QuicksortExterno(&ArqLi, &ArqEi, &ArqLEs, 1, 100);

//     read_and_print_records("PROVAO.dat");

//     return 0;


// }

