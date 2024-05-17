
#ifndef DATACHUNK_H
#define DATACHUNK_H

#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <cmath>
#include <map>

/* pair used for priority queue
int = index // string = data line */
typedef std::pair<int, std::string> queuePair;

/* data format struct */
typedef struct _dataPoint {
    float legLength;
	std::string name;
	std::string diet;

    // provide comparator ( < / less than ) for sorting dataPoints types
    friend bool operator< (const _dataPoint& dp1, const _dataPoint& dp2) {
        if (dp1.legLength == dp2.legLength)
            return dp1.name < dp2.name;

        return (dp1.legLength < dp2.legLength) ? true : false;
    }

    // used by datachunks::Iterator() to stream output to file
    friend std::ifstream& operator<<(const _dataPoint& dp1, const _dataPoint& dp2) {
        /* nothing needed here */
    }
	
} dataPoint;


namespace datachunks
{

    /* iterator for datapoint */
    class DataPointCollection
    {
        public:

            std::vector<dataPoint> data;


            /* in case non-default constructor isn't called */
            DataPointCollection() : DataPointCollection(2) {
                /* nothing needed here */    
            }

            DataPointCollection(int numStructs) {
                std::vector<dataPoint> data(numStructs);
            }

        class Iterator // used by DataPointCollection, only
        {
            /*
             *
             * * * * * * * * * See the following documentation * * * * * * * * * * * * * * * *
             * 
             * Iterator Reference   : https://cplusplus.com/reference/iterator/
             * Category Specifics   : https://en.cppreference.com/w/cpp/iterator/iter_t
             * std::iterator        : https://cplusplus.com/reference/iterator/iterator/
             * Iterator Category    : https://en.cppreference.com/w/cpp/iterator/iterator_tags
             * 
             * Forward iterators allow access to the sequence of elements in a container.
             * Allows for multiple copies of the iterator to pass by iterator values (i.e.
             * the foward iterator is dereferenceable).
             * 
             * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
             * 
            */
            public:
                using iterator_category = std::forward_iterator_tag;
                using difference_type   = std::ptrdiff_t;
                using value_type        = dataPoint;
                using pointer           = dataPoint*;  // or value_type*
                using reference         = dataPoint&;  // or value_type&

            private:
                pointer ptr;

            public:
                Iterator(pointer ptr) : ptr(ptr) {}

                reference operator*() const { return *ptr; } // dereference pointer
                pointer operator->() { return ptr; } // access pointer member

                // Prefix increment
                Iterator& operator++() { ptr++; return *this; }
                // Postfix increment
                Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

                friend bool operator== (const Iterator& a, const Iterator& b) { return a.ptr == b.ptr; };
                friend bool operator!= (const Iterator& a, const Iterator& b) { return a.ptr != b.ptr; };
        };
        
        // for range-based loops
        Iterator begin() { return Iterator(&data[0]); }
        Iterator end() { return Iterator(&data[0] + data.size()); }
    };
} // namespace datachunks

void printDinoOrder(const std::map<std::string, int>& stats);
void sortIntoBuffer(const std::string& filename, int chunkSize);
void mergeSortedFiles(const std::string& outputFile, int numTempFiles);
void joinStrings(std::string& f1, std::unordered_map<std::string, dataPoint>& dinoMap);
void computeSpeed(const std::unordered_map<std::string, dataPoint>& dinoMap, std::string name);

#endif