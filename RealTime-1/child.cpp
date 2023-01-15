#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;
/*-------------------*/
// defult distance every player need to run
#define DISTANCE_TO_RUN 50
/*-------------------------*/
void callNextTeamMember();
void callParent();
void signal_catcher(int);
void readyState();
void signalSet();
void callDrawer();
/*-------------------------*/
int nextMemberPid;
int childNumber;
int parentId;
int speed;
char teamName[7];
char redFifo[50] = "/home/ibraheem/redfifo";
char greenFifo[50] = "/home/ibraheem/greenfifo";
int delay;
float timeDelay;
char childNumberString[2];
char childPid[10];
string buffer = "";
int counter = 0;
int drawerPid;
int foRed;
int foGreen;
bool redopen = false, greenopen = false;
/*-------------------------*/

int main(int argc, char *args[])
{

     speed = atoi(args[4]);
     nextMemberPid = atoi(args[5]);
     strcpy(teamName, args[2]);
     childNumber = atoi(args[0]);
     parentId = atoi(args[3]);
     strcpy(childNumberString, args[0]);
     drawerPid = atoi(args[6]);
     strcpy(childPid, args[1]);
     printf("i am child number %d with pid %s from team %s,Parent Pid ==>%s,my speed=>%s,my next teammate pid : %s,start running,i wanna to win(:(:\n", 5 - childNumber, args[1], args[2], args[3], args[4], args[5]);
     fflush(stdout);
     readyState();
     return 0;
}

void callNextTeamMember()
{
     if (nextMemberPid != -1)
          kill(nextMemberPid, 2);
     readyState();
}

void callParent()
{

     if (strcmp(teamName, "red") == 0)
     {
          kill(parentId, 1);
     }
     else
          kill(parentId, 4);
     readyState();
}
void signal_catcher(int signal_number)
{

     if (signal_number == 2)
     {
          callDrawer();
     }
     if (signal_number == 14)
     {
          if (childNumber == 0)
               callParent();
          else
               callNextTeamMember();
     }
}

void readyState()
{
     signalSet();
     while (1)
          ;
}

void signalSet()
{
     if (sigset(SIGINT, signal_catcher) == SIG_ERR)
     {
          perror("Sigset can not set SIGINT");
          exit(SIGINT);
     }
     if (sigset(SIGALRM, signal_catcher) == SIG_ERR)
     {
          perror("Sigset can not set SIGALRM");
          exit(SIGALRM);
     }
}

void callDrawer()
{
     int sz = sizeof(childNumberString) / sizeof(char);
     for (int i = 0; i < sz; i++)
     {
          if (childNumberString[i] != '\0')
               buffer.push_back(childNumberString[i]);
     }

     buffer += ",";
     string childPidStr(childPid);
     buffer += childPidStr;
     buffer += "\0";
     int size = buffer.length();
     char *stringBuffer;
     stringBuffer = (char *)malloc(size);

     for (int i = 0; i < size; i++)
     {
          stringBuffer[i] = buffer[i];
     }

     fflush(stdout);
     if (strcmp("red", teamName) == 0)
     {
          if ((foRed = open(redFifo, O_RDWR | O_NONBLOCK)) < 0)
          {
               perror("cant open fifo red");
               exit(1);
          }
          redopen = true;

          int redwritestaste = write(foRed, stringBuffer, sizeof(stringBuffer));
          if (redwritestaste < 0)
          {
               perror("error writing to red fifo green\n");
               exit(1);
          }
          sleep(2);
          kill(drawerPid, 10);
          // close(foRed);
     }
     else
     {

          if ((foGreen = open(greenFifo, O_RDWR | O_NONBLOCK)) < 0)
          {
               perror("cant open fifo green");
               exit(1);
          }

          int greenwritestate = write(foGreen, stringBuffer, sizeof(stringBuffer));
          if (greenwritestate < 0)
          {
               perror("error writing to green fifo\n");
               exit(1);
          }
          sleep(1);
          // close(foGreen);
          kill(drawerPid, 14);
     }
     readyState();
}