#include <string>
#include <iostream>
#include <boost/assert.hpp>
#include "JSONParser.hpp"

class cData {
    private:
    int startIndex;
    candlestick *cp;
    int arraySize;

    public:
    cData (int size) {
        arraySize = size;
        cp = new candlestick[size];
        startIndex = 0;
    }
    ~cData () {
        delete[] cp;
    }
    void addCandlestick(std::string s) {
        cp[startIndex] = createCandlestickObject(s);
        startIndex++;
        checkStartIndex();
    }
    void checkStartIndex () {
        if (startIndex >= arraySize) {
            startIndex = 0;
        }
    }
    int returnStart () {
        return startIndex;
    }
    int returnSize () {
        return arraySize;
    }
    candlestick accessDataAtTime (int unitsTime) {
        BOOST_ASSERT(unitsTime < arraySize);
        int position = startIndex - unitsTime;
        if (position >= 0) {
            return cp[position];
        } else {
            return cp[arraySize + position];
        }
    }
    candlestick accessDataAtIndex (int index) {
        BOOST_ASSERT(index < arraySize);
        return cp[index];
    }
    void printCandlestick (candlestick c) {
        std::cout << "oP:" << c.getopenPrice() << std::endl;
        std::cout << "cP:" << c.getclosePrice() << std::endl;
        std::cout << "pH:" << c.getpriceHigh() << std::endl;
        std::cout << "pL:" << c.getpriceLow() << std::endl;
        std::cout << "bV:" << c.getbaseVolume() << std::endl;
        std::cout << "qV:" << c.getquoteVolume() << std::endl;
        std::cout << "closed:" << c.getClosed() << std::endl;
    }
};