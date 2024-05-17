#include "datachunks.hpp"

// provide ( >> ) operator for type detail::Line
// This will permit stream from input to output: input >> output
namespace detail
{
    class Line : std::string
    {
        friend std::istream& operator>>(std::istream& is, Line& line)
        {   // delimit on comma to process data lines
            return std::getline(is, line, ',');
        }
    };
}

template<class Alloc> // type must be an allocator: 
void read_lines(std::istream& is, Alloc dest)
{
    dataPoint streamData;
    typedef std::istreambuf_iterator<detail::Line> InIt;

    // stream insert data into struct
    InIt(is) >> streamData.name >> streamData.legLength >> streamData.diet;

    // fill dest container with streamData
    std::fill(dest.begin(), dest.end(), streamData);
}

void sortIntoBuffer(const std::string& filename, int numStructs) {
    
    std::string token;
    dataPoint streamData;
    std::ifstream file(filename);
    datachunks::DataPointCollection collection(numStructs); // limit collection size

    int tempFileNumber = 0;
    while (!file.eof()) {

        read_lines(file, collection.data); // read -> call iterator

        // see note on comparator in header file : dataPoint
        std::sort(collection.data.begin(), collection.data.end());

        std::ofstream out("tempfile" + \
            std::to_string(tempFileNumber++) + ".txt");

        for (auto it : collection) // why we created the iterator
            out << it.name << it.legLength << it.diet << std::endl;

        /* This accomplishes the file processing and sorting when available
        is tight. The dataPoint struct will eventually have more members */
        
        out.close();
    }
    file.close();
}

void mergeSortedFiles(const std::string& outputFile, int numStructs) {
    /*
     * A modified merge-sort algorithm, that sorts the data in the file
     * chunks k ways.
     * 
     * Using a Priority Queue (MinHeap) will maintain the sorted order, 
     * allowing us to pop() 1 dataPoint and >> to the final outputFile.
     *
    */
    std::ofstream out(outputFile);
    std::vector<std::ifstream> inputs(numStructs); // container of file streams for data intake
    std::priority_queue<queuePair, std::vector<queuePair>, std::greater<>> minHeap;

    for (int i = 0; i < numStructs; i++) { // push first dataPoint in file
        inputs[i].open("tempfile" + std::to_string(i) + ".txt");
        std::string line;
        if (inputs[i] >> line) {
            minHeap.push({i, line});
        }
    }

    while (!minHeap.empty()) {
        auto [idx, value] = minHeap.top(); // pointer to top of Queue
        minHeap.pop();
        out << value << std::endl;

        // Get the next line at the same index, push to Heap
        std::string nextLine;
        if (inputs[idx] >> nextLine) {
            minHeap.push({idx, nextLine});
        }
    }

    for (auto& in : inputs) {
        in.close();
    }
    out.close();
}
