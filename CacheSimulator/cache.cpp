//Alazar Hailemariam
//ECE-2500 Project 3

#include "cache.h"

/**
 * @brief cache::cache
 * Description: Default constructor, does nothing
 */
cache::cache() {

}

/**
 * @brief cache::cache
 * Description: Contructor which initializes the correct type of cache from
 * the arguments provided
 * @param cacheSize
 * @param blockSize
 * @param mapping
 */
cache::cache(int cacheSize, int blockSize, char mapping) {
    this->cacheSize = cacheSize;
    this->blockSize = blockSize;
    this->mapping = mapping;
    //Prepare the correct memory module
    switch(mapping) {
        case 'D':
            memoryDM.clear();
            break;
        case '2':
            memory2W.clear();
            memorySet.clear();
            break;
        case '4':
            memory4W.clear();
            memorySet.clear();
            break;
        case 'F':
            memoryFA.clear();
            break;
    }
}

/**
 * @brief cache::read
 * Description: Method used to read data from cache or memory from the
 * given byte address and returns a cacheInfo object to report results,
 * it sends the access request to the correct helper function based on the
 * mapping type of the cache
 * @param byteAddress
 * @return
 */
cacheInfo cache::read(int byteAddress) {
    cacheResult.write = false;
    //Direct request to the appropriate function
    switch(mapping) {
        case 'D':
            directMapped(byteAddress);
            break;
        case '2':
            twoWay(byteAddress);
            break;
        case '4':
            fourWay(byteAddress);
            break;
        case 'F':
            fullyAssociative(byteAddress);
            break;
    }
    return cacheResult;
}

/**
 * @brief cache::write
 * Description: Method used to write data to the cache and memory from the
 * given byte address and returns a cacheInfo object to report results,
 * it sends the access request to the correct helper function based on the
 * mapping type of the cache
 * @param byteAddress
 * @return
 */
cacheInfo cache::write(int byteAddress) {
    cacheResult.write = true;
    //Direct request to the appropriate function
    switch(mapping) {
        case 'D':
            directMapped(byteAddress);
            break;
        case '2':
            twoWay(byteAddress);
            break;
        case '4':
            fourWay(byteAddress);
            break;
        case 'F':
            fullyAssociative(byteAddress);
            break;
    }
    return cacheResult;
}

/**
 * @brief cache::directMapped
 * Description: Helper function to process direct mapped cache memory read
 * and write requests
 * @param byteAddress
 */
void cache::directMapped(int byteAddress) {
    //Calculate necessary tag and block address information
    blockCount = cacheSize / blockSize;
    blockAddress = (byteAddress / blockSize) % blockCount;
    tag = byteAddress / (blockSize * blockCount);
    cacheResult.comparisons = 1;

    //Perform memory operation and report
    if(memoryDM.contains(blockAddress) && memoryDM.value(blockAddress) == tag)
        cacheResult.hit = true;
    else {
        memoryDM.insert(blockAddress, tag);
        cacheResult.hit = false;
    }
}

/**
 * @brief cache::twoWay
 * Description: Helper function to process 2W mapped cache memory read
 * and write requests
 * @param byteAddress
 */
void cache::twoWay(int byteAddress) {
    //Calculate necessary tag and block address information
    setCount = cacheSize / (blockSize * 2);
    blockAddress =(byteAddress / blockSize) % setCount;
    tag = byteAddress / (blockSize * setCount);
    cacheResult.comparisons = blockCount / setCount;

    //Perform memory operation and report
    if(memory2W.contains(blockAddress)) {
        memorySet = memory2W.value(blockAddress);
        if(memorySet.contains(tag)) {
            memorySet.removeOne(tag);
            cacheResult.hit = true;
        } else {
            cacheResult.hit = false;
            if(memorySet.size() > 1)
                memorySet.pop_back();
        }
        memorySet.push_front(tag);
        memory2W[blockAddress] = memorySet;
    } else {
        cacheResult.hit = false;
        memorySet.clear();// = new QLinkedList<int>();
        memorySet.push_front(tag);
        memory2W[blockAddress] = memorySet;
    }
}

/**
 * @brief cache::fourWay
 * Description: Helper function to process 4W mapped cache memory read
 * and write requests
 * @param byteAddress
 */
void cache::fourWay(int byteAddress) {
    //Calculate necessary tag and block address information
    setCount = cacheSize / (blockSize * 4);
    blockAddress = (byteAddress / blockSize) % setCount;
    tag = byteAddress / (blockSize * setCount);
    cacheResult.comparisons = blockCount / setCount;

    //Perform memory operation and report
    if(memory2W.contains(blockAddress)) {
        memorySet = memory2W.value(blockAddress);
        if(memorySet.contains(tag)) {
            memorySet.removeOne(tag);
            cacheResult.hit = true;
        } else {
            cacheResult.hit = false;
            if(memorySet.size() > 3)
                memorySet.pop_back();
        }
        memorySet.push_front(tag);
        memory2W[blockAddress] = memorySet;
    } else {
        cacheResult.hit = false;
        memorySet.clear();// = new QLinkedList<int>();
        memorySet.push_front(tag);
        memory2W[blockAddress] = memorySet;
    }
}

/**
 * @brief cache::fullyAssociative
 * Description: Helper function to process FA mapped cache memory read
 * and write requests
 * @param byteAddress
 */
void cache::fullyAssociative(int byteAddress) {
    //Calculate the tag and block address information
    //In this case both the tag and block address are combined in the
    //variable blockAddress since data can go anywhere in the cache
    blockCount = cacheSize / blockSize;
    blockAddress = byteAddress / blockSize;
    cacheResult.comparisons = blockCount;

    //Perform memory operation and report
    if(memoryFA.contains(blockAddress)) {
        memoryFA.removeOne(blockAddress);
        memoryFA.push_front(blockAddress);
        cacheResult.hit = true;
    } else {
        memoryFA.push_front(blockAddress);
        if(memoryFA.size() > blockCount)
            memoryFA.removeLast();
        cacheResult.hit = false;
    }
}
