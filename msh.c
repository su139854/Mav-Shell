/*
Name: Safi Ullah
ID: 1001541732
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define WHITESPACE " \t\n"      // We want to split our command line up into tokens
                                // so we need to define what delimits our tokens.
                                // In this case  white space
                                // will separate the tokens on our command line

#define MAX_COMMAND_SIZE 255    // The maximum command-line size

#define MAX_NUM_ARGUMENTS 11     // Mav shell only supports five arguments

int main()
{

  char * cmd_str = (char*) malloc( MAX_COMMAND_SIZE );
  int arr[15];
  char *his[15];
  int counter=-1;
  int hcounter=-1;

  int i;
  for( i = 0; i < 15; i++ )
  {
    his[i] = (char*) malloc( 255 );
    memset( his[i], 0, 255 );
  }
  while( 1 )
  {
          // Print out the msh prompt
          printf ("msh> ");

          // Read the command from the commandline.  The
          // maximum command that will be read is MAX_COMMAND_SIZE
          // This while command will wait here until the user
          // inputs something since fgets returns NULL when there
          // is no input
          while( !fgets (cmd_str, MAX_COMMAND_SIZE, stdin) );



          if(strcmp(cmd_str,"\n")==0)// if enter is pushed we will go back to the start of the while loop
          {
             continue;
          }
          else
          {


               if(cmd_str[0]=='!')// if the user wants to run a previous command this if will take care of that
               {
                 if(his[atoi(&cmd_str[1])][0]=='\0')// if the previous command is not in history do this
                 {
                   printf("Command not found.\n");
                 }
                 else// if it is in history then copy it to cmd_str and this will just run the command
                 {
                   int n=atoi(&cmd_str[1]);
                   strncpy(cmd_str, his[n], 255);
                 }
               }
          }






          if(hcounter==14) // if the history array is full then it will delete the oldest one and add the newest history to the end
          {

               int r;
               for(r=0;r<hcounter;r++)
               {
                  his[r]=his[r+1];

               }
               strncpy(his[hcounter],cmd_str, 255 );



          }
          else// if history is not full then just add history to the end of the array
          {
                hcounter++;
                strncpy(his[hcounter],cmd_str, 255);


          }



          /* Parse input */
          char *token[MAX_NUM_ARGUMENTS];

          int   token_count = 0;

          // Pointer to point to the token
          // parsed by strsep
          char *argument_ptr;

          char *working_str  = strdup( cmd_str );

          // we are going to move the working_str pointer so
          // keep track of its original value so we can deallocate
          // the correct amount at the end
          char *working_root = working_str;

          // Tokenize the input stringswith whitespace used as the delimiter
          while ( ( (argument_ptr = strsep(&working_str, WHITESPACE ) ) != NULL) &&
                    (token_count<MAX_NUM_ARGUMENTS))
          {
            token[token_count] = strndup( argument_ptr, MAX_COMMAND_SIZE );
            if( strlen( token[token_count] ) == 0 )
            {
              token[token_count] = NULL;
            }
              token_count++;
          }

          if(strcmp(token[0],"quit")==0||strcmp(token[0],"exit")==0)// handling quit commands
          {
             exit(0);
          }
          else if(strcmp(token[0],"cd")==0)// handling change of directory
          {


             chdir(token[1]);
             continue;

          }
          else if(strcmp(token[0],"showpids")==0)// show previous pids used
          {


              int ij=0;

              if(counter==-1)
              {
                 printf("No pids.\n");

              }
              else if(counter==0)
              {
                 printf("%d: %d",0,arr[0]);
                 printf("\n");
              }
              else
              {
                 while(ij<counter+1)// printing pids
                 {


                         printf("%d: %d",ij,arr[ij]);
                         printf("\n");
                         ij++;


                 }
              }
          }
          else if(strcmp(token[0],"history")==0)// outputting the history
          {

              int rj=0;


                 while(rj<hcounter+1)
                 {



                       printf("%d: %s",rj,his[rj]);

                       rj++;





                 }



          }


          else
          {


             pid_t pid = fork( );
             if( pid == 0 )
             {



                   int ret = execvp( token[0], &token[0] );
                   if( ret == -1 )// if command is invalid
                   {
                     char *r=strcat(token[0],": Command not found");
                     exit(0);
                     perror(r);
                   }


               }

               else
               {


                  if(counter==14)// adding pids if the array is full by deleting oldest one and sorting the array
                  {
                     int r;
                     for(r=0;r<counter;r++)
                     {
                        arr[r]=arr[r+1];
                     }
                     arr[counter]=pid;

                  }
                  else// if array is not full add to end of array
                  {
                     counter++;
                     arr[counter]=pid;

                  }


                  int status;
                  wait( & status );

                }

           }
           free( working_root );


    }
   return 0;
}
