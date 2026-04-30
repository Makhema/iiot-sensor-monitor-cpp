#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <iostream>
#include <vector>

using namespace std;

/*This is the record structure with the necessary data members for
handling the data*/

#pragma pack(push, 1)
struct IIoTSensor
{
    int Sensor_ID;
    string Data_FORMAT;
    double READING;
    bool READING_ACTIVE;
};
#pragma pack(pop)

class IIoTMonitor
{
    private:
        int intRows; //These are the rows of the array.
        int intCols; //These are the columns of the array.
        vector<vector<IIoTSensor>> sensors; //Then this is the 2D array.

    public:
        //These are the constructors of the class.
        IIoTMonitor();
        IIoTMonitor(int Rows, int Cols);

        //This is the destructor of the class.
        ~IIoTMonitor();

        //These are the other required functions.
        void readSensorsFromTXT(const std::string& filename);
        void exportSensorsToBinary(const std::string& filename);
        void exportAverageReadingToTXT(const std::string& filename);
        int getNumSensorsInBinary() const;
        void updateSensor(int row, int col, double newReading);
};

#endif // HEADER_H_INCLUDED
