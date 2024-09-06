#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <unordered_map>
#include <vector>
using namespace std;

struct Ticket {
    string userName;
    long id;
    Ticket(const string& inName, const long& inId): userName(inName), id(inId){}
};

struct Node {
    unordered_map<Ticket*, string> data;
    Node* next;

    Node(unordered_map<Ticket*, string> inData): data(inData), next(nullptr){}
};


class Airplane {
    string flight;
    string date;
    int numSeat, maxSeat;
    vector<int> booking;
    vector<list<unordered_map<Ticket*, string>>> linkedLists;
    map<string, string> pricing;
    long id;
public:
    Airplane(const string& inDate, const string& inFlight, const int& inSeatNum, const map<string, string> inPricing):
    date(inDate), flight(inFlight), numSeat(inSeatNum), pricing(inPricing), id(1) {

        maxSeat = stoi(pricing.rbegin()->first);
        for (int i=0; i <maxSeat; i++) {
            booking.push_back(i);
            linkedLists.emplace_back();
        }
    }

    void book(string seat, string userName) {
        unordered_map<Ticket*, string> data;

        string row = seat.substr(0, seat.size() - 1);
        string place = seat.substr(seat.size() - 1);

        Ticket* ticket = new Ticket(userName, id); //made ticket

        data[ticket] = place; //made a map
        //add this as a list element
        linkedLists[stoi(row) - 1].push_front(data); //-1 because it starts with zero
        cout << "Booked row " << row << " place " << place << " id " << id << endl;
        id++;
    }
};

struct pair_hash {
        size_t operator () (const pair<string, string>& p) const {
            size_t h1 = hash<std::string>{}(p.first);
            size_t h2 = hash<std::string>{}(p.second);
            return h1 ^ (h2 << 1);
        }
    };

class Airport {
public:
    unordered_map<pair<string, string>, Airplane*, pair_hash> planes;
    Airport() {}
    void addPlane(string inFlight, string inDate, int inSeatNum, map<string, string> inPricing) {

        planes[make_pair(inDate, inFlight)] = new Airplane (inDate, inFlight, inSeatNum, inPricing);
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


class Helper {
    Airport airport;
public:
    Helper (Airport& myAirport): airport(myAirport){}
    void book(const string& date, const string& flight, const string& place, const string& userName) {
        auto plane = airport.planes[make_pair(date, flight)];
        if (plane==NULL)
            cout << "Sorry, no airplane in this day with this flight number :(" << endl;
        else {
            plane->book(place, userName);
        }
    }
};

int main()
{
    Airport myAirport;
    FileReader file(myAirport);
    file.read();
    Helper helper(myAirport);
    helper.book("01.01.2023", "JK321", "1A", "Alla");
    helper.book("01.01.2023", "JK321", "2B", "JK");
    return 0;
}
