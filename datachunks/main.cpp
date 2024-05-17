#include "datachunks.hpp"

int main(void)
{

    const std::string inputFile = "dataset1.csv";
    const std::string outputFile = "finaldataset.csv";

    sortIntoBuffer(inputFile, 2); // 2 structs worth of data / file
    mergeSortedFiles(outputFile, 4);

    return 0;
}