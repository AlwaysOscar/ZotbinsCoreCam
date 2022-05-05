#include "Fullness.hpp"
#include "IDistance.hpp"

using namespace NFullness;

#define SENSOR_CALLS 8 // made it a number divisible by 4 for IQM calculations

// using constructor
Fullness::Fullness(uint32_t binHeight, IDistance &distanceSensor) : mBinHeight(binHeight), mDistanceSensor(distanceSensor)
{
}

// Fullness::Fullness(int32_t binHeight, IDistance::IDistance &distanceSensor)
// {
//     height = binHeight;
//     sensor = &distanceSensor;
// }

float Fullness::getFullness()
{
    // call getDistance on sensor SENSOR_CALLS times to get an array of distances
    std::array<int32_t, distanceBufferSize> distanceBuffer;
    for (int i = 0; i < SENSOR_CALLS; i++)
    {
        distanceBuffer[i] = mDistanceSensor.getDistance();
    }

    // use IQM to get average distance
    int averageDistance = IQM(distanceBuffer);

    float fullness = (static_cast<float>(mBinHeight) - averageDistance) / mBinHeight;

    return fullness;
}

bool Fullness::isValidDistance(uint32_t distance)
{
    return (mBinHeight < distance) && (distance >= 0);
}

float Fullness::IQM(std::array<int32_t, distanceBufferSize> &distanceBuffer)
{
    // return variable
    int32_t averageDistance = 0;

    // sort the data
    shellSort(distanceBuffer);

    // find the average not including the first two and last two indexes
    for (size_t i = 2; i < distanceBufferSize - 2; i++)
    {
        averageDistance += distanceBuffer[i];
    }
    averageDistance /= (distanceBufferSize - 4);

    return averageDistance;
}

void Fullness::shellSort(std::array<int32_t, distanceBufferSize> &arr)
{
    for (size_t gap = arr.size() / 2; gap > 0; gap /= 2)
    {
        for (size_t i = gap; i < arr.size(); i += 1)
        {
            int32_t temp = arr[i];

            size_t j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap)
            {
                arr[j] = arr[j - gap];
            }

            arr[j] = temp;
        }
    }
}