#ifndef SHARED_DATA
#define SHARED_DATA
#include "header.h"
using namespace std;
int
    programSimulationTime,
    typeCThreshold, typeBThreshold, typeAThreshold, shippingTime, loadedTypeCTruck, loadedTypeBTruck,
    loadedTypeATruck, storageAreaMinCapacity, storageAreaMaxCapacity,
    storageAreaEmployeeTime, printExpiredDateTime, timeC_max, timeC_min, timeB_max, timeB_min, timeA_max, timeA_min;

vector<int> lines_pids;
#endif