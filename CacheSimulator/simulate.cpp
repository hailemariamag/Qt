//Alazar Hailemariam
//ECE-2500 Project 3

#include <QCoreApplication>
#include <QDebug>
#include <QStringList>
#include <QDataStream>
#include <QFile>
#include "cache.h"
#include <QTextStream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //Prepare File-I/O, Change file names here
    QFile fileIn("test.trace");           //      <----- Input File
    QFile fileOut("trace.output");        //      <----- Output File


    if (!fileIn.open(QIODevice::ReadOnly | QIODevice::Text))    {
        qDebug() << "Could not open file.";
        return 0;
    }
    fileOut.open(QIODevice::WriteOnly);
    QTextStream inFile(&fileIn);
    QTextStream outFile(&fileOut);

    //Initialize variables
    char mapping[4] = {'D', '2', '4', 'F'};
    QString mapType[4] = {"DM", "2W", "4W", "FA"};
    QStringList input;
    cacheInfo cacheResult;
    int cacheToMem, memToCache;
    double hitRate;
    int operationCount;

    //Read input file into qstringlist
    while(!inFile.atEnd())
        input.append(inFile.readLine());

    //Triple Nested forloop to switch between different cache size, block size and mappings
    for(int cacheSize = 1024; cacheSize <= 65536/*65536*/; cacheSize *= 4) {
        for(int blockSize = 8; blockSize <= 64; blockSize *= 2) {
            for(int mapIndex = 0; mapIndex < 4; mapIndex++) {
                memToCache = 0;
                cacheToMem = 0;
                hitRate = 0;
                operationCount = 0;
                //Initialize cache with current configurations
                cache Cache(cacheSize, blockSize, mapping[mapIndex]);
                //For loop to keep executing commands until input is exhausted
                for (QStringList::iterator it = input.begin(); it != input.end(); ++it) {
                    operationCount++;
                    QString line = *it;
                    QStringList inputType = line.split(" ");
                    bool hex;
                    //Determine operation type and call correct cache access function on input
                    //Also keep track of individual hits, and data transfers between cache and memory
                    if(inputType[0] == "R") {
                        int byteAddress = inputType[1].toInt(&hex, 16);
                        cacheResult = Cache.read(byteAddress);
                        if(!cacheResult.hit)
                            memToCache += blockSize;
                        else
                            hitRate++;
                    } else {
                        int byteAddress = inputType[1].toInt(&hex, 16);
                        cacheResult = Cache.write(byteAddress);
                        if(!cacheResult.hit)
                            memToCache += blockSize;
                        else
                            hitRate++;
                        cacheToMem += blockSize;
                    }
                }
                //Output results to file for a single cache configuration that has completed
                //all the specified input commands before moving to a new cache configuration
                hitRate /= operationCount;
                outFile << cacheSize << "\t" << blockSize << "\t" << mapType[mapIndex];
                outFile.setRealNumberPrecision(2);
                outFile.setRealNumberNotation(QTextStream::FixedNotation);
                outFile<< "\t" << hitRate;
                outFile.setRealNumberNotation(QTextStream::SmartNotation);
                outFile << "\t" << memToCache << "\t" << cacheToMem << "\t" << cacheResult.comparisons << "\n";
            }
        }
    }
    fileIn.close();
    fileOut.close();
    return 0;
}
