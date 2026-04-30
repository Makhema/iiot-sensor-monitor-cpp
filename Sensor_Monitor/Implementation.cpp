#include "Header.h"
#include <fstream>
#include <iostream>
#include <iomanip>

IIoTMonitor::IIoTMonitor()
{

}

// Constructor
IIoTMonitor::IIoTMonitor(int Rows, int Cols)
{
    intRows = Rows;
    intCols = Cols;
    sensors.resize(Rows, vector<IIoTSensor>(Cols));
}

// Destructor
IIoTMonitor::~IIoTMonitor()
{
    // Clean up any dynamically allocated resources if needed
    for (int r = 0; r < intRows; ++r)
    {
        // Release memory for each row
        sensors.clear();
    }
    // Clear the entire sensor array
    sensors.clear();
}


// Read sensor data from a text file
void IIoTMonitor::readSensorsFromTXT(const string& filename)
{
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open input file: " << filename << std::endl;
        return;
    }

    int file_num_rows, file_num_cols;
    inputFile >> file_num_rows >> file_num_cols;

    // Check if the file dimensions match the dimensions of the monitor
    if (file_num_rows != intRows || file_num_cols != intCols)
    {
        cerr << "Error: File dimensions do not match the monitor dimensions." << endl;
        inputFile.close();
        return;
    }

    // Iterate through the rows and columns of the sensorArray and populate it with data from the file
    for (int r = 0; r < intRows; ++r)
    {
        for (int c = 0; c < intCols; ++c)
        {
            IIoTSensor& sensor = sensors[r][c];
            inputFile >> sensor.Sensor_ID >> sensor.Data_FORMAT >> sensor.READING >> sensor.READING_ACTIVE;
        }
    }

    inputFile.close();
}

// Export active sensors to a binary file
void IIoTMonitor::exportSensorsToBinary(const string& filename)
{
     std::ofstream outputFile(filename, std::ios::binary);
    if (!outputFile.is_open())
    {
        std::cerr << "Error: Unable to open output file: " << filename << std::endl;
        return;
    }

    for (int r = 0; r < intRows; ++r)
    {
        for (int c = 0; c < intCols; ++c)
        {
            const IIoTSensor& sensor = sensors[r][c];
            if (sensor.READING_ACTIVE)
            {
                outputFile.write(reinterpret_cast<const char*>(&sensor), sizeof(IIoTSensor));
            }
        }
    }

    outputFile.close();
}


// Calculate and export the average reading to a text file
void IIoTMonitor::exportAverageReadingToTXT(const string& filename)
{
    ofstream outputFile(filename, ios::app); // Append mode
    if (!outputFile.is_open())
    {
        cerr << "Error: Unable to open output file: " << filename << endl;
        return;
    }

    double sum = 0.0;
    int count = 0;

    for (int r = 0; r < intRows; ++r)
    {
        for (int c = 0; c < intCols; ++c)
        {
            const IIoTSensor& sensor = sensors[r][c];
            if (sensor.READING_ACTIVE)
            {
                sum += sensor.READING;
                ++count;
            }
        }
    }

    if (count > 0)
    {
        double average = sum / count;
        outputFile << std::fixed << std::setprecision(2) << "Average Reading: " << average << std::endl;
    } else {
        outputFile << "No active sensors to calculate the average." << std::endl;
    }

    outputFile.close();
}

// Get the number of sensors in the binary file
int IIoTMonitor::getNumSensorsInBinary() const
{
    int count = 0;
    for (int r = 0; r < intRows; ++r)
    {
        for (int c = 0; c < intCols; ++c)
        {
            const IIoTSensor& sensor = sensors[r][c];
            if (sensor.READING_ACTIVE)
            {
                ++count;
            }
        }
    }
    return count;
}

// Update a sensor's reading
void IIoTMonitor::updateSensor(int row, int col, double newReading)
{
    if (row >= 0 && row < intRows && col >= 0 && col < intCols)
    {
        IIoTSensor& sensor = sensors[row][col];
        sensor.READING = newReading;

        // After updating, save all active sensors to the binary file
        exportSensorsToBinary("active_sensors.bin");
    } else {
        cerr << "Error: Invalid row or column coordinates." << endl;
    }
}
