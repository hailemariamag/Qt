//Alazar Hailemariam
//ECE-2500 Project 3

#include<QMap>
#include<QString>
#include<QLinkedList>

#ifndef CACHE_H
#define CACHE_H
/**
 * @brief The cacheInfo struct
 * This struct is used to report results from a cache
 * read or write operation
 */
struct cacheInfo {
    bool write;             //True if writing to memory
    bool hit;               //True if there's a hit
    int comparisons;        //Number of comparisons
};

/**
 * @brief The cache class
 * This class is used to model a cache
 * Comments for methods are in cpp file
 */
class cache
{
public:
    cache();
    cache(int, int, char);
    cacheInfo read(int);
    cacheInfo write(int);

private:
    void directMapped(int byteAddress);
    void twoWay(int byteAddress);
    void fourWay(int byteAddress);
    void fullyAssociative(int byteAddress);
    QMap<int, int> memoryDM;                //Used to store data for DM caches
    QMap<int, QLinkedList<int> > memory2W;  //Used to store data for 2W caches
    QMap<int, QLinkedList<int> > memory4W;  //Used to store data for 4W caches
    QLinkedList<int> memoryFA;              //Used to store data for FA caches
    QLinkedList<int> memorySet;             //Used to store sets inside set associative caches
    int cacheSize;                          //Saves the current cache size
    int setCount;                           //Used for set associative caches
    int blockCount;                         //Used for DM and FA caches
    int blockAddress;                       //Used to store block address in cache
    int tag;                                //Used to store the tag
    int blockSize;                          //Used to store current block size of cache
    char mapping;                           //Used to store the current cache mapping type
    struct cacheInfo cacheResult;           //Used to store read and write results
};

#endif // CACHE_H
