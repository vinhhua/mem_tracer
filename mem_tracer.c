#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "CommandNode.h"
#include <stdbool.h>
#define PATH "/home/cs149/Desktop/CS149/Assignment4"
/*TO DO:
  store commands into array - DONE
  store those commands inside array into Linked List - DONE
  put those node into the stack - DONE, should push function's name into stack instead of command's node.
  PRINT TRACE, WRITE INTO FILES, RE SIZE ARRAY - figuring out how to write out to file effectively.
  more to come  */

// TRACE_NODE_STRUCT is a linked list of
// pointers to function identifiers
// TRACE_TOP is the head of the list is the top of the stack
struct TRACE_NODE_STRUCT {
    char* functionid;                      // ptr to function identifier
    struct TRACE_NODE_STRUCT* next;  // ptr to next frama
};
typedef struct TRACE_NODE_STRUCT TRACE_NODE;
static TRACE_NODE* TRACE_TOP = NULL;       // ptr to the top of the stack


FILE* fp;
//static int output;              //file output "memtrace"

/* --------------------------------*/
/* function PUSH_TRACE */
void PUSH_TRACE(char* p)          // push p on the stack
{
    TRACE_NODE* tnode;
    static char glob[]="global";

    if (TRACE_TOP==NULL) {

        // initialize the stack with "global" identifier
        TRACE_TOP=(TRACE_NODE*) malloc(sizeof(TRACE_NODE));

        // no recovery needed if allocation failed, this is only
        // used in debugging, not in production
        if (TRACE_TOP==NULL) {
            printf("PUSH_TRACE: memory allocation error\n");
            exit(1);
        }

        TRACE_TOP->functionid = glob;
        TRACE_TOP->next=NULL;
    }

    // create the node for p
    tnode = (TRACE_NODE*) malloc(sizeof(TRACE_NODE));

    // no recovery needed if allocation failed, this is only
    // used in debugging, not in production
    if (tnode==NULL) {
        printf("PUSH_TRACE: memory allocation error\n");
        exit(1);
    }

    tnode->functionid=p;
    tnode->next = TRACE_TOP;  // insert fnode as the first in the list
    TRACE_TOP=tnode;          // point TRACE_TOP to the first node

}/*end PUSH_TRACE*/

/* --------------------------------*/
/* function POP_TRACE */
void POP_TRACE()    // remove the op of the stack
{
    TRACE_NODE* tnode;
    tnode = TRACE_TOP;
    TRACE_TOP = tnode->next;
    free(tnode);
}/*end POP_TRACE*/



/* --------------------------------------------- */
/* function PRINT_TRACE prints out the sequence of function calls that are on the stack at this instance */
/* For example, it returns a string that looks like: funcA:funcB:funcC.  */
char* PRINT_TRACE()
{
    int depth = 50; //A max of 50 levels in the stack will be combined in a string for printing out.
    int i, length, j;
    TRACE_NODE* tnode;
    static char buf[100];

    if (TRACE_TOP==NULL) {     // stack not initialized yet, so we are
        strcpy(buf,"global");   // still in the `global' area
        return buf;
    }

    /* peek at the depth top entries on the stack, but do not
       go over 100 chars and do not go over the bottom of the
       stack */

    sprintf(buf,"%s",TRACE_TOP->functionid);
    length = strlen(buf);                  // length of the string so far
    for(i=1, tnode=TRACE_TOP->next;
        tnode!=NULL && i < depth;
        i++,tnode=tnode->next) {
        j = strlen(tnode->functionid);             // length of what we want to add
        if (length+j+1 < 100) {              // total length is ok
            sprintf(buf+length,":%s",tnode->functionid);
            length += j+1;
        }else                                // it would be too long
            break;
    }
    return buf;
}

// -----------------------------------------
// function REALLOC calls realloc
// TODO REALLOC should also print info about memory usage.
// For instance, example of print out:
// "File tracemem.c, line X, function F reallocated the memory segment at address A to a new size S"
// Information about the function F should be printed by printing the stack (use PRINT_TRACE)
void* REALLOC(void* p,int t,char* file,int line)
{
    /*PRINT STATEMENT FOR DEBUG PURPOSES FOR NOW
      TO DO: WRITE THE OUTPUT INTO A FILE FOR TRACING PURPOSES */
    char *test;
    test = PRINT_TRACE();
    printf("file=\"%s/%s\",line=%d,function=\"%s\",segment reallocated to address %p to a new size %d\n", PATH, file, line, test, p, t);
    p = realloc(p,t);
    return p;
}

// -------------------------------------------
// function MALLOC calls malloc
// TODO MALLOC should also print info about memory usage.
// For instance, example of print out:
// "File tracemem.c, line X, function F allocated new memory segment at address A to size S"
// Information about the function F should be printed by printing the stack (use PRINT_TRACE)
void* MALLOC(int t,char* file,int line)
{
    void* p;
    p = malloc(t);
    char *test = PRINT_TRACE();
    /*PRINT STATEMENT FOR DEBUG PURPOSES FOR NOW
      TO DO: WRITE THE OUTPUT INTO A FILE FOR TRACING PURPOSES */
    printf("file=\"%s/%s\",line=%d,function=\"%s\",segment allocated to address %p to size %d\n", PATH, file, line, test, p, t);
    return p;
}

// ----------------------------------------------
// function FREE calls free
// TODO FREE should also print info about memory usage.
// For instance, example of print out:
// "File tracemem.c, line X, function F deallocated the memory segment at address A"
// Information about the function F should be printed by printing the stack (use PRINT_TRACE)
void FREE(void* p,char* file,int line)
{
    free(p);
    char *test = PRINT_TRACE();
    /*PRINT STATEMENT FOR DEBUG PURPOSES FOR NOW
      TO DO: WRITE THE OUTPUT INTO A FILE FOR TRACING PURPOSES */
    printf("file=\"%s/%s\",line=%d,function=\"%s\",segment deallocated at the address %p\n", PATH, file, line, test, p);
}

#define realloc(a,b) REALLOC(a,b,__FILE__,__LINE__)
#define malloc(a) MALLOC(a,__FILE__,__LINE__)
#define free(a) FREE(a,__FILE__,__LINE__)


// -----------------------------------------
// function add_command will add an extra command to a char**.
// This function is intended to demonstrate how memory usage tracing of realloc is done
void add_command(char*** array, int ind)
{
    PUSH_TRACE("add_command");

    //*array = realloc(*array, sizeof(char*) * (STR_SIZE + ind));
    *array = realloc(*array, sizeof(char*) * (ind+1));
    //20 + 20 / 20 + 40 / 20 + 60
    POP_TRACE();    //pop
    return;
}// end add_column


// ------------------------------------------
// function make_extend_array
// Example of how the memory trace is done
// This function is intended to demonstrate how memory usage tracing of malloc and free is done
void make_extend_array() {
    PUSH_TRACE("make_extend_array");    //push trace

    char **array;       //memory array
    char input[STR_SIZE];     //array input
    int index = 0;       //memory array index init. to 0

    //memory allocate size for memory array - must free
    array = (char**)malloc(sizeof(char*));  //allocate space for 20 commands

    //till end of file
    while(!feof(fp)) {
        if (fgets(input, STR_SIZE, fp) != NULL) {    //read input
            array[index] = (char*)malloc(sizeof(char) * STR_SIZE); //allocate memory per index

            strcpy(array[index], input);    //copy
            printf("%d\n", index);
            printf("%s\n", array[index]);

            index++;

            //expand array if size meets parameters
            //if(index % STR_SIZE == 0 && index != 0)
            add_command(&array, index);
        }
        else
            break;

    }


    printf("%d\n", index);
    //now deallocate it
    for(int i = 0; i < index; i++)
        free((void*)array[i]);      //free array indexes
    free((void*)array);        //free memory array

    POP_TRACE();
    return;
}//end make_extend_array


// ----------------------------------------------
// function main

int main(int argc, char **argv) {
    // error-handling, if more than 2 arguments then print the error message.
    if (argc == 1 || argc > 2) {
        fprintf(stderr, "usage: %s <file_name>\n", argv[0]);
        exit(1);
    }

    PUSH_TRACE("main");

    // validate the file pointer and error-handling, if file pointer points to nothing then file does not exist.
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: %s does not exist.\n", argv[1]);
        exit(1);
    }

    //redirect output
    //output = open("memtrace.out", O_WRONLY | O_CREAT | O_APPEND, 0664);
    //dup2(output, 1);



    /*
    //free(input);
    // Create the head of the Linked list.
    int index = 0;
    CommandNode *commands_list[count_lines]; // supposedly 6 atm
    commands_list[index] = (CommandNode *)malloc(sizeof(CommandNode));    // memory allocated for first index
    CreateCommandNode(commands_list[index], newString[index], 0, NULL);  // Create the node with head as thisnode, cmd[node_index] which is cmd[0], node_index of first index, count_lines as 5, and NULL.
    CommandNode *head = commands_list[0];

    // Store each command into a linked list node.
    for (i = 0; i < count_lines - 1; i++) {
	commands_list[++index] = (CommandNode *)malloc(sizeof(CommandNode));
	CreateCommandNode(commands_list[index], newString[index], index, NULL);
	InsertCommandAfter(commands_list[index - 1], commands_list[index]);
    }

    */


    PRINT_TRACE();

    make_extend_array();

    //PRINT_NODES(head);

    POP_TRACE();
    //close(output);
    return(0);
}// end main

//recursive function - print linked list
void PRINT_NODES(CommandNode *head) {
    PUSH_TRACE("print_node");       //push to trace stack

    CommandNode *temp = head;       //traverse linkd list
    while (temp != NULL) {
        printf("Node index: %d, function ID: %s\n", temp->index, temp->command);
        temp = GetNextCommand(temp);
    }
}
