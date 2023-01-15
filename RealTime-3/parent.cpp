#include "header.h"
#include "shared_data.h"
#include "lines_results.h"
using namespace std;
pid_t create_process(const char *name, int number_of_workers, int number_of_inorder_workers, int time, const char *type);
int read_file();
void create_lines();
void kill_childs(vector<int> pids);
int random_number(int min, int max);
int main()
{
    // read user defined data

    if (read_file() == -1)
    {
        perror("Error in reading file");
        exit(1);
    }
    // create lines
    create_lines();
    sleep(30);
    kill_childs(lines_pids);
    return 0;
}

pid_t create_process(const char *name, int number_of_workers, int number_of_inorder_workers, int time, const char *type)
{
    pid_t pid;
    pid = fork();

    if (pid == 0)
    {
        if (name == "./line")
        {
            // cout << "executing line" << endl;
            execlp(name, name, to_string(number_of_workers).data(), to_string(number_of_inorder_workers).data(), to_string(time).data(), type, (char *)NULL);
            perror("Error in exec");
            exit(1);
        }
    }
    else if (pid < 0)
    {
        printf("Fork failed.\n");
    }
    return pid;
}

int read_file()

{
    string line;
    ifstream myfile("userDefinedData.txt");
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            // Split line by ':'
            size_t pos = line.find(":");
            string name = line.substr(0, pos);
            string value = line.substr(pos + 1);

            // Split value by ','
            pos = value.find(",");
            int value1, value2;
            stringstream(value.substr(0, pos)) >> value1;
            stringstream(value.substr(pos + 1)) >> value2;

            // Assign values to variables
            if (name == "timeA")
            {
                timeA_min = value1;
                timeA_max = value2;
            }
            else if (name == "timeB")
            {
                timeB_min = value1;
                timeB_max = value2;
            }
            else if (name == "timeC")
            {
                timeC_min = value1;
                timeC_max = value2;
            }
            else if (name == "printExpiredDateTime")
            {
                printExpiredDateTime = value1;
            }
            else if (name == "storageAreaEmployeeTime")
            {
                storageAreaEmployeeTime = value1;
            }
            else if (name == "storageAreaMaxCapacity")
            {
                storageAreaMaxCapacity = value1;
            }
            else if (name == "storageAreaMinCapacity")
            {
                storageAreaMinCapacity = value1;
            }
            else if (name == "loadedTypeATruck")
            {
                loadedTypeATruck = value1;
            }
            else if (name == "loadedTypeBTruck")
            {
                loadedTypeBTruck = value1;
            }
            else if (name == "loadedTypeCTruck")
            {
                loadedTypeCTruck = value1;
            }
            else if (name == "shippingTime")
            {
                shippingTime = value1;
            }
            else if (name == "typeAThreshold")
            {
                typeAThreshold = value1;
            }
            else if (name == "typeBThreshold")
            {
                typeBThreshold = value1;
            }
            else if (name == "typeCThreshold")
            {
                typeCThreshold = value1;
            }
            else if (name == "programSimulationTime")
            {
                programSimulationTime = value1;
            }
        }
        myfile.close();
    }
    else
    {
        cout << "Unable to open file" << endl;
        return -1;
    }
    return 0;
}

void create_lines()
{
    int time;
    for (int i = 0; i < 3; i++)
    {
        time = random_number(timeA_min, timeA_max);
        pid_t pid = create_process("./line", 8, 4, time, "A");
        lines_pids.push_back(pid);
    }
    for (int i = 0; i < 2; i++)
    {
        time = random_number(timeB_min, timeB_max);
        pid_t pid = create_process("./line", 6, 6, time, "B");
        lines_pids.push_back(pid);
    }
    for (int i = 0; i < 2; i++)
    {
        time = random_number(timeC_min, timeC_max);
        pid_t pid = create_process("./line", 5, 3, time, "C");
        lines_pids.push_back(pid);
    }
}

void kill_childs(vector<int> pids)
{
    // kill the child process which thier pids stode in the vector
    for (int i = 0; i < lines_pids.size(); i++)
    {
        kill(lines_pids[i], SIGKILL);
    }
}

// randomly generated number in rang
int random_number(int min, int max)
{
    srand(time(NULL) ^ (getpid() << 16));
    return rand() % (max - min + 1) + min;
}