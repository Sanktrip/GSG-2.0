#pragma once

#define BOOST_JSON_STACK_BUFFER_SIZE 1024
#include <string>
#include <iostream>
#include <assert.h>
#include <map>
#include <queue>
#include <limits>
#include <cmath>
#include <sstream>
#include <string>
#include <boost/json/src.hpp>

class candlestick{
    private:
        double openPrice;
        double closePrice;
        double priceHigh;
        double priceLow;
        double baseVolume;
        double quoteVolume;
        bool isClosed;

    public:
        candlestick (double oP = 0, double cP = 0, double pH = 0, double pL = 0, double bV = 0, double qV = 0, bool iC = false) {
            openPrice = oP;
            closePrice = cP;
            priceHigh = pH;
            priceLow = pL;
            baseVolume = bV;
            quoteVolume = qV;
            isClosed = iC;
        };

        void setopenPrice(double p) {
            openPrice = p;
        }
        double getopenPrice(void) {
            return openPrice;
        }
        void setclosePrice(double p) {
            closePrice = p;
        }
        double getclosePrice(void) {
            return closePrice;
        }
        void setpriceHigh(double p) {
            priceHigh = p;
        }
        double getpriceHigh(void) {
            return priceHigh;
        }
        void setpriceLow(double p) {
            priceLow = p;
        }
        double getpriceLow(void) {
            return priceLow;
        } 
        void setbaseVolume(double v) {
            baseVolume = v;
        }
        double getbaseVolume(void) {
            return baseVolume;
        }
        void setquoteVolume(double v) {
            quoteVolume = v;
        }
        double getquoteVolume(void) {
            return quoteVolume;
        } 
        void setClosed(bool c) {
            isClosed = c;
        }
        bool getClosed(void) {
            return isClosed;
        }
        void printCandlestick() {
            std::cout << openPrice << std::endl;
            std::cout << priceHigh << std::endl;
            std::cout << priceLow << std::endl;
            std::cout << closePrice << std::endl;
            std::cout << baseVolume << std::endl;
            std::cout << quoteVolume << std::endl;
            std::cout << isClosed << std::endl;
        }
};

class tradeData {
    private:
        double price;
        double volume;
        int time;

    public:
        tradeData (double p, double v, int t) {
            price = p;
            volume = v;
            time = t;
        };

        void setPrice (double p) {
            price = p;
        }
        void setVolume (double v) {
            volume = v;
        }
        void setTime (int t) {
            time = t;
        }
        double getPrice () {
            return price;
        }
        double getVolume () {
            return volume;
        }
        double getTime () {
            return time;
        }
        void printTrade () {
            std::cout << " P: "<< price << "V: "<< volume << "T: "<< time << std::endl;
        }
};

tradeData setTradeData (std::string s);

std::string checkDataInterval (std::string s);

candlestick createCandlestickObject (std::string s);

std::string create_subscription_message(std::string stream);

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
            if (cp[startIndex].getClosed()) {
                startIndex++;
            }
            checkStartIndex();
        }
        void addCandlestick(candlestick candle) {
            cp[startIndex] = candle;
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
            assert(unitsTime < arraySize);
            int position = startIndex - unitsTime;
            if (position >= 0) {
                return cp[position];
            } else {
                return cp[arraySize + position];
            }
        }
        candlestick accessDataAtIndex (int index) {
            assert(index < arraySize);
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
        void printAllData (bool includeIndex = false) {
            for (int i = startIndex; i < arraySize; i++) {
                if (includeIndex) {
                    std::cout << "ID: " << i << std::endl;
                }
                printCandlestick(cp[i]);
            }
            for (int i = 0; i < startIndex; i++) {
                if (includeIndex) {
                    std::cout << "ID: " << i << std::endl;
                }
                printCandlestick(cp[i]);
            }
        }
        bool dataComplete () {
            return cp[arraySize - 1].getClosed();
        }
        bool allCandlesClosed () {
            if (dataComplete()) {
                return cp[startIndex].getClosed();
            }
            return false;
        }
};


class valueArea {
    public:
        double valueAreaUpper;
        double valueAreaLower;
};

class volumeData {
    private:
        double totalVolume;
        std::queue<tradeData> trades;
        int frontDataTime;
    public:
        volumeData () {
            totalVolume = 0;
            frontDataTime = INT_MAX;
        }
        double getPrice () {
            return trades.front().getPrice();
        }
        double getTotalVolume () {
            return totalVolume;
        }
        int getFrontTime () {
            return frontDataTime;
        }
        void addTradeData (tradeData t) {
            totalVolume += t.getVolume();
            if (trades.empty()) {
                frontDataTime = t.getTime();
            }
            trades.push(t);
        }
        void removeTradeData () {
            tradeData t = trades.front();
            totalVolume -= t.getVolume();
            trades.pop();
            if (!trades.empty()) {
                frontDataTime = trades.front().getTime();
            } else {
                frontDataTime = INT_MAX;
                assert(totalVolume == 0);
            }
        }
        void printVolumeData (bool condensed) {
            std::cout << "TV :"<< totalVolume << std::endl;
            std::cout << "FT: "<< frontDataTime << std::endl;
            if (!condensed) {
                auto copy = trades;
                std::cout << "Q:";
                while (!copy.empty()) {
                    copy.front().printTrade();
                    copy.pop();
                }
            }
        }
};

class volumeProfile {
    private:
        double priceIncrement;
        double POC;
        valueArea VA;
        double profileTotalVolume;
        double minVol;
        int dataSize;
        bool rolling;
        std::map<double, volumeData> profile;

    public:
        volumeProfile (double precision, int dataSeriesSize, bool isRollingProfile = true, double minimumVolume = 0) {
            priceIncrement = precision;
            dataSize = dataSeriesSize;
            minVol = minimumVolume;
            rolling = isRollingProfile;
            POC = 0;
            profileTotalVolume = 0;
        }
        void addTrade (std::string json) {
            tradeData tD = setTradeData(json);
            if (tD.getVolume() < minVol) {
                return;
            }
            profileTotalVolume += tD.getVolume();
            double key;
            key = (std::round(tD.getPrice() / priceIncrement) * priceIncrement);

            if (auto check = profile.find(key); check != profile.end()) {
                (check->second).addTradeData(tD);
                if (check->second.getTotalVolume() >= profile.at(POC).getTotalVolume()) {
                    POC = key;
                }
            } else {
                volumeData vD;
                vD.addTradeData(tD);
                profile.emplace(key, vD);
                if (POC == 0 || profile.at(POC).getTotalVolume() <= vD.getTotalVolume()) {
                    POC = key;
                }
            }
            if (rolling) {
                removeTrades(tD.getTime());
            }
        }
        void removeTrades (int currentTime) {
            for (auto it = profile.begin(); it != profile.end(); ++it) {
                while ((it->second).getFrontTime() < (currentTime - dataSize)) {
                    it->second.removeTradeData();
                }
            }
        }
        double getPOC () {
            return POC;
        }
        valueArea getValueArea () {
            valueArea vA;
            double volSumFront = 0;
            double volSumBack = 0;
            double percentvol16 = profileTotalVolume / 6.0;
            for (auto rit = profile.rbegin(); rit != profile.rend(); ++rit) {
                volSumBack += rit->second.getTotalVolume();
                if (volSumBack >= percentvol16) {
                    vA.valueAreaUpper = rit->first;
                    break;
                }
            }
            for (auto it = profile.begin(); it != profile.end(); ++it) {
                volSumFront += it->second.getTotalVolume();
                if (volSumFront >= percentvol16) {
                    vA.valueAreaLower = it->first;
                    break;
                }
            }
            return vA;
        }
        void printProfile (bool condensed) {
            if (!condensed) {
                for (auto rit = profile.rbegin(); rit != profile.rend(); ++rit) {
                    std::cout << rit->first << std::endl;
                    (rit->second).printVolumeData(false);
                }
            } else {
                for (auto rit = profile.rbegin(); rit != profile.rend(); ++rit) {
                    std::cout << rit->first << std::endl;
                    (rit->second).printVolumeData(true);
                }
            }
        }
};

// not finished
class timeProfile {
    private:
        int range;
        
        int POC; // point of control

    public:

};

