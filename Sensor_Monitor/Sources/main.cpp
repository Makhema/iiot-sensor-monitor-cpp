#include <iostream>
#include "Header.h"

//This is the standard namespace.
using namespace std;

int main()
{
    //Create IIoTMonitor object.
    IIoTMonitor Object(10, 10);

    Object.readSensorsFromTXT("sensor_readings.cleartext");

    return 0;
}
