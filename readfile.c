#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 10

int main(int argc, char **argv) {
    if (argc == 1 || argc > 2) {
	fprintf(stderr, "usage: %s <file_name>\n", argv[0]);
	exit(1);
    }

    FILE *fp;
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
	fprintf(stderr, "%s does not exist\n", argv[1]);
	exit(1);
    }
 
    char **str = (char **)malloc(sizeof(char *) * SIZE);
    char input[20];
    int count_lines = 0;
    for (int i=0; i < SIZE; i++) {
	str[i] = (char *)malloc(sizeof(char *) * 20);
	if (fgets(input, 20, fp) != NULL)
	    count_lines += 1;
	    strcpy(str[i], input); 
    }

    printf("The contents inside the file are: \n");
    for (int i=0; i < count_lines; i++) {
	printf("array[%d] = %s\n", i, str[i]);
    }
}
