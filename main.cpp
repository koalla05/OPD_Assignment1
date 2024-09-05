#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <unordered_map>
#include <vector>
using namespace std;

class Airplane {
    //string flight;
    string date;
    int numSeat;
    //vector<int> booking;
    map<string, string> pricing;
public:
    Airplane(string inDate, int inSeatNum, map<string, string> inPricing):
    date(inDate),  numSeat(inSeatNum), pricing(inPricing){}
};

class Ticket {
    string userName;
    long id;
};

class Airport {
    struct pair_hash {
        size_t operator () (const pair<string, string>& p) const {
            size_t h1 = hash<std::string>{}(p.first);
            size_t h2 = hash<std::string>{}(p.second);

            return h1 ^ (h2 << 1);
        }
    };

    unordered_map<pair<string, string>, Airplane*, pair_hash> planes;
public:
    Airport() {}
    void addPlane(string inFlight, string inDate, int inSeatNum, map<string, string> inPricing) {

        planes[make_pair(inDate, inFlight)] = new Airplane (inDate, inSeatNum, inPricing);
    }
};



class FileReader {
    string path = "C:/Users/kovko/CLionProjects/OPD_Assignment1/config.txt";
    string date;
    string flightNo;
    int numSeat;
    int numRow;
    string price, seats;
    string endSeat;
    map<string, string> pricing;
    Airport& airport;
public:
    FileReader(Airport& inAirport): airport(inAirport) {}
    void read() {
        ifstream inputFile(path);

        if (!inputFile) {
            cerr << "No file has been found: " << path << endl;
            return;
        }

        string line;
        istringstream iss(line);
        getline(inputFile, line);

        while (getline(inputFile, line)) {
            istringstream iss(line);
            pricing.clear();
            iss >> date >> flightNo >> numSeat;

            while (iss >> seats >> price) {
                endSeat = seats.substr(seats.find('-') + 1);
                pricing[endSeat] = price;
            }

            airport.addPlane(flightNo, date, numSeat, pricing);
        }
        inputFile.close();
    }
};

int main()
{
    Airport myairport;
    FileReader file(myairport);
    file.read();
    return 0;
}
