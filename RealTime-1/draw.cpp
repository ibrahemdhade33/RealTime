#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <signal.h>
using namespace std;
struct points
{
    float x;
    float y;
    float z;
};
float x_red_pos, x_green_pos, red_dis, green_dis, next_red, next_green;
bool check = false;
void display(void);
void init_point();
void reshape(int w, int h);
void timer(int);
int getSpeedRandomly();
struct points green[4];
struct points red[4];
char readFromFifoRed[10];
char readFromFifoGreen[10];
char redFifo[50] = "/home/ibraheem/redfifo";
char greenFifo[50] = "/home/ibraheem/greenfifo";
void listenToFifo(int);
void setSignalRed();
void setSignalGreen();
bool red_turn[5];
bool green_turn[5];
int redpid;
int greenpid;
int fRed;
int fGreen;
bool opengeeen = false, openred = false;
int main(int argc, char **argv)
{

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(1800, 500);
    glutCreateWindow("Big Race");
    init_point();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    setSignalRed();
    setSignalGreen();
    x_red_pos = red[0].x;
    x_green_pos = green[0].x;
    red_dis = 0;
    green_dis = 0;
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}

void display(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glPointSize(20);
    glBegin(GL_POINTS);

    glColor3f(0.5, 0, 0);
    if (red_turn[0])
        glVertex3f(x_red_pos, red[0].y, 0);
    else
        glVertex3f(red[0].x, red[0].y, 0);
    glEnd();

    glPointSize(20);
    glBegin(GL_POINTS);

    glColor3f(0.5, 0, 0);
    if (red_turn[1])
        glVertex3f(x_red_pos, red[1].y, 0);
    else
        glVertex3f(red[1].x, red[1].y, 0);
    glEnd();

    glPointSize(20);
    glBegin(GL_POINTS);

    glColor3f(0.5, 0, 0);
    if (red_turn[2])
        glVertex3f(x_red_pos, red[2].y, 0);
    else
        glVertex3f(red[2].x, red[2].y, 0);
    glEnd();

    glPointSize(20);
    glBegin(GL_POINTS);
    glColor3f(0.5, 0, 0);
    if (red_turn[3])
        glVertex3f(x_red_pos, red[3].y, 0);
    else
        glVertex3f(red[3].x, red[3].y, 0);
    glEnd();

    glPointSize(20);
    glBegin(GL_POINTS);
    glColor3f(0.5, 0, 0);
    if (red_turn[4])
        glVertex3f(x_red_pos, red[4].y, 0);
    else
        glVertex3f(red[4].x, red[4].y, 0);
    glEnd();

    glPointSize(20);
    glBegin(GL_POINTS);

    glColor3f(0, 1.0, 0);
    if (green_turn[0])
        glVertex3f(x_green_pos, green[0].y, 0);
    else
        glVertex3f(green[0].x, green[0].y, 0);
    glEnd();

    glPointSize(20);
    glBegin(GL_POINTS);
    glColor3f(0, 1.0, 0);
    if (green_turn[1])
        glVertex3f(x_green_pos, green[1].y, 0);
    else
        glVertex3f(green[1].x, green[1].y, 0);
    glEnd();

    glPointSize(20);
    glBegin(GL_POINTS);
    glColor3f(0, 1.0, 0);
    if (green_turn[2])
        glVertex3f(x_green_pos, green[2].y, 0);
    else
        glVertex3f(green[2].x, green[2].y, 0);
    glEnd();

    glPointSize(20);
    glBegin(GL_POINTS);
    glColor3f(0, 1.0, 0);
    if (green_turn[3])
        glVertex3f(x_green_pos, green[3].y, 0);
    else
        glVertex3f(green[3].x, green[3].y, 0);
    glEnd();

    glPointSize(20);
    glBegin(GL_POINTS);
    glColor3f(0, 1.0, 0);
    if (green_turn[4])
        glVertex3f(x_green_pos, green[4].y, 0);
    else
        glVertex3f(green[4].x, green[4].y, 0);
    glEnd();
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-15, 15, -15, 15);
    glMatrixMode(GL_MODELVIEW);
}

void init_point()
{
    float x_pos = -8.0;
    for (int i = 0; i < 5; i++)
    {
        green[i].x = x_pos;
        green[i].y = 3.0;
        red[i].x = x_pos;
        red[i].y = -3.0;
        x_pos += 4.0;
    }
}

void timer(int z)
{

    int fram_rate = 1000 / 60;

    glutTimerFunc(fram_rate, timer, 0);
    glutPostRedisplay();
    if (x_red_pos < next_red)
        x_red_pos += red_dis;
    else
        kill(redpid, 14);

    if (x_green_pos < next_green)
        x_green_pos += green_dis;
    else
        kill(greenpid, 14);
}

void listenToFifo(int signal)
{
    memset(readFromFifoRed, '\0', sizeof(readFromFifoRed));
    memset(readFromFifoGreen, '\0', sizeof(readFromFifoGreen));
    memset(red_turn, false, sizeof(red_turn));
    memset(green_turn, false, sizeof(green_turn));
    // int xxx = 1;
    // while (xxx)
    //     ;
    if (signal == 10)
    {

        if ((fRed = open(redFifo, O_RDWR | O_NONBLOCK)) < 0)
        {
            perror("fifo open error\n");
            exit(0);
        }

        int redsz = read(fRed, readFromFifoRed, sizeof(readFromFifoRed));
        if (redsz < 0)
        {
            perror("error reading from red fifo\n");
            exit(1);
        }
        printf("read from red fifo :  %s,size = %d,openfifo state =%d\n", readFromFifoRed, redsz, fRed);
        // close(fRed);

        if (readFromFifoRed != (char *)NULL)
        {

            string s(readFromFifoRed);
            vector<string> split;
            stringstream stringStream(s);
            string temp;

            while (getline(stringStream, temp, ','))
            {
                split.push_back(temp);
            }

            int childNumber = atoi(split[0].c_str());
            redpid = atoi(split[1].c_str());
            x_red_pos = red[4 - childNumber].x;
            red_dis = getSpeedRandomly() / 100.0;
            next_red = red[(4 - childNumber)].x + 4;
            red_turn[4 - childNumber] = true;
        }
        setSignalRed();
    }

    if (signal == 14)
    {

        if ((fGreen = open(greenFifo, O_RDWR | O_NONBLOCK)) < 0)
        {
            perror("fifo open error\n");
            exit(0);
        }

        int greensz = read(fGreen, readFromFifoGreen, sizeof(readFromFifoGreen));
        if (greensz < 0)
        {
            perror("error reading from green fifo\n");
            exit(1);
        }

        printf("read from green fifo :  %s,size=%d,openfifo state=%d\n", readFromFifoGreen, greensz, fGreen);
        // close(fGreen);
        if (readFromFifoGreen != (char *)NULL)
        {
            string s(readFromFifoGreen);
            vector<string> split;
            stringstream stringStream(s);
            string temp;
            while (getline(stringStream, temp, ','))
            {
                split.push_back(temp);
            }
            int childNumber = atoi(split[0].c_str());
            greenpid = atof(split[1].c_str());
            x_green_pos = green[4 - childNumber].x;
            green_dis = getSpeedRandomly() / 100.0;
            next_green = green[(4 - childNumber)].x + 4;
            green_turn[4 - childNumber] = true;
        }
        setSignalGreen();
    }
}

void setSignalRed()
{

    // signal for red team
    if (sigset(SIGUSR1, listenToFifo) == SIG_ERR)
    {
        perror("Sigset can not set SIGHUP");
        exit(SIGUSR1);
    }

    // signal for green team
}
void setSignalGreen()
{

    if (sigset(SIGALRM, listenToFifo) == SIG_ERR)
    {
        perror("Sigset can not set SIGIIL");
        exit(SIGALRM);
    }
}
int getSpeedRandomly()
{
    srand(time(NULL));
    return ((rand() % 15) + 10);
}