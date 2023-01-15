#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <math.h>
#include <time.h>
#include <map>
#include <string>

#include <sys/stat.h>
#include <sys/types.h>
using namespace std;
/*---------------------------*/
#define TEAM_SIZE 5
#define RED_TEAM 0
#define GREEN_TEAM 1

/*---------------------------*/
int greenTeam[TEAM_SIZE], redTeam[TEAM_SIZE];
int pid;
int parentId;
char teamName[10];
int i;
int speed;
int nextTeamMember = -1;
map<string, int> result;
int numberOfRounds = 5;
int numberOfRoundsCnt = 0;
int drawerPid;
bool endOfRound = false;
char redFifo[50] = "/home/ibraheem/redfifo";
char greenFifo[50] = "/home/ibraheem/greenfifo";
/*---------------------------*/
void createTeam(int);
void killAllChildrens();
void startingTheRace();
int getSpeedRandomly();
void signal_catcher(int);
void printResults();
void startRound();
void setTeamMemberReady();
void signalSet();
void createDrawer();
void createFifos();
/*---------------------------*/

int main(int argc, char *args[])
{

    printf("%d\n", argc);
    fflush(stdout);

    if (argc > 2)
    {
        printf("error in argumnets please enter one argument only\n");
        exit(1);
    }
    else
    {
        if (args[1] != nullptr)
        {
            if (!(numberOfRounds = atoi(args[1])))
            {
                printf("error in argument format,only integer allowed\n");
                exit(1);
            }
        }
        printf("number of rounds : %d\n\n\n", numberOfRounds);
    }
    fflush(stdout);
    // get parent pid
    parentId = getpid();
    createFifos();
    createDrawer();
    // create both teams
    createTeam(RED_TEAM);
    createTeam(GREEN_TEAM);
    sleep(3);
    startRound();
    return 0;
}

// flag : the zero represent red team,1 represt green team
void createTeam(int flag)
{

    // generate green team processes
    for (i = 0; i < TEAM_SIZE; i++)
    {
        pid = fork();

        // check for fork failure
        if (pid < 0)
        {
            perror("fork failure");
            exit(1);
        }
        else if (!pid)
        {

            speed = getSpeedRandomly();

            if (flag == 1)
            {
                strcpy(teamName, "green");
                if (i > 0)
                    nextTeamMember = greenTeam[i - 1];
            }
            else
            {
                strcpy(teamName, "red");
                if (i > 0)
                    nextTeamMember = redTeam[i - 1];
            }

            setTeamMemberReady();
        }

        else
        {
            char team[7];
            // store children pids in parent
            if (flag == 1)
            {
                greenTeam[i] = pid;
                strcpy(team, "green");
            }

            else
            {
                redTeam[i] = pid;
                strcpy(team, "red");
            }
        }
    }
}

// kill all childes to finish the program
void killAllChildrens()
{
    sleep(10);
    // killing childs processes
    for (int i = 0; i < TEAM_SIZE; i++)
    {
        kill(greenTeam[i], 9);
        kill(redTeam[i], 9);
    }
    sleep(10);
    kill(drawerPid, 9);
}

// begin the race
void startingTheRace()
{
    kill(redTeam[TEAM_SIZE - 1], 2);
    usleep(400000);
    kill(greenTeam[TEAM_SIZE - 1], 2);

    signalSet();
    pause();
}

// signal catch function

void signal_catcher(int signal_number)
{
    if (signal_number == 1)
    {
        if (!endOfRound)
        {
            printf("red win this round\n");
            result["red"]++;
            endOfRound = true;
        }
        else
            endOfRound = false;
    }

    else if (signal_number == 4)
    {
        if (!endOfRound)
        {
            printf("green win this round\n");
            result["green"]++;
            endOfRound = true;
        }
        else
            endOfRound = false;
    }
    fflush(stdout);
    sleep(7);
    if (!endOfRound)
    {
        numberOfRoundsCnt++;
        if (numberOfRoundsCnt == numberOfRounds)
        {
            killAllChildrens();
            printResults();
            exit(0);
        }
        else
        {
            startRound();
        }
    }
}

int getSpeedRandomly()
{
    srand(time(NULL) ^ (getpid() << 8));
    int num = (rand() % 15) + 10;
    return num;
}

void printResults()
{
    int red_result = result["red"];
    int green_result = result["green"];
    printf("green team : %d\nred team : %d\n", green_result, red_result);
    if (red_result > green_result)
        printf("red team win the race,,,number of wining round : %d of %d\n", red_result, numberOfRounds);
    else
        printf("green team win the race,,,number of wining round : %d of %d\n", green_result, numberOfRounds);

    printf("\n\n\n\n\n\n\n\\n");
    fflush(stdout);
}

void startRound()
{

    printf("\n\nround number : %d\n\n", numberOfRoundsCnt);
    printf("starting the game\nparent pid : %d\n\n", parentId);
    fflush(stdout);
    startingTheRace();
}

void setTeamMemberReady()
{

    char teamMemberNumber[2], processId[7], parentIdString[7], speedString[3], nextTeamMemberpPidString[7], drawerPidString[7];
    sprintf(teamMemberNumber, "%d", i);
    sprintf(processId, "%d", getpid());
    sprintf(parentIdString, "%d", getppid());
    sprintf(speedString, "%d", speed);
    sprintf(nextTeamMemberpPidString, "%d", nextTeamMember);
    sprintf(drawerPidString, "%d", drawerPid);
    execlp("./child", teamMemberNumber, processId, teamName, parentIdString, speedString, nextTeamMemberpPidString, drawerPidString, (char *)NULL);
}

void signalSet()
{
    // signal for red team
    if (sigset(SIGHUP, signal_catcher) == SIG_ERR)
    {
        perror("Sigset can not set SIGHUP");
        exit(SIGHUP);
    }

    // signal for green team
    if (sigset(SIGILL, signal_catcher) == SIG_ERR)
    {
        perror("Sigset can not set SIGIIL");
        exit(SIGILL);
    }
}

void createDrawer()
{
    int id = fork();

    if (id < 0)
    {
        perror("fork failuer");
        exit(1);
    }

    if (id == 0)
    {
        execlp("./draw", "sss", "ssss", (char *)NULL);
    }
    else
    {

        drawerPid = id;
        return;
    }
}

void createFifos()
{

    if (mkfifo(redFifo, 0744) < 0)
    {
        perror("cant creaye fifo red\n");
        exit(1);
    }
    if (mkfifo(greenFifo, 0744) < 0)
    {
        perror("cant create green fifo\n");
        exit(1);
    }
}