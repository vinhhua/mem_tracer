/**
 *CommandNode.h
 *Author: Bill Andreopoulos
 *The struct for a Command Node and function prototypes for a linked list of 
commands
 *CS149 assignment 3 usage only
**/
typedef struct command_struct {
   char command[20];
   int index;
   int PID;
   struct command_struct* nextCommandPtr;
} CommandNode;
void CreateCommandNode(CommandNode* thisNode, char *cmd, int ind, CommandNode* nextCmd);
void InsertCommandAfter(CommandNode* thisNode, CommandNode* newNode);
CommandNode* GetNextCommand(CommandNode* thisNode);
CommandNode* FindCommand(CommandNode* cmd, int pid);
