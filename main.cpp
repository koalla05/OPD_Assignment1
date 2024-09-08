#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <unordered_map>
#include <vector>
using namespace std;

struct Ticket {
    string place;
    string userName;
    long id;
    string row;
    Ticket(const string& inName, const long& inId, const string& inPlace, const string& inRow): userName(inName), id(inId), place(inPlace), row(inRow){}
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
    vector<list<unordered_map<long, Ticket*>>> linkedLists;
    map<string, string> pricing;

public:
    Airplane(const string& inDate, const string& inFlight, const int& inSeatNum, const map<string, string> inPricing):
    date(inDate), flight(inFlight), numSeat(inSeatNum), pricing(inPricing) {

        maxSeat = stoi(pricing.rbegin()->first);
        for (int i=0; i <maxSeat; i++) {
            booking.push_back(i);
            linkedLists.emplace_back();
        }
    }
    string getPrice(const string& row) {
        auto price = pricing.lower_bound(row)->second;
        return price;
    }
    void book(string seat, string userName, const long id) {
        unordered_map<long, Ticket*> data;

        string row = seat.substr(0, seat.size() - 1);
        string place = seat.substr(seat.size() - 1);

        Ticket* ticket = new Ticket(userName, id, place, row); //made ticket

        data[ticket->id] = ticket; //made a map
        //add this as a list element
        linkedLists[stoi(row) - 1].push_front(data); //-1 because it starts with zero
        cout << "Confirmed with ID " << id << endl;

    }

    void findDelete(const long& ticketId){
        //string price;
        for (size_t i = 0; i < booking.size(); ++i) {
            int startIndex = booking[i];

            if (!linkedLists[startIndex].empty()) {
                auto& lists = linkedLists[startIndex];

                for (auto it = lists.begin(); it != lists.end(); ) {
                    auto mapIt = it->find(ticketId);
                    if (mapIt != it->end()) {
                        string price = getPrice(to_string(startIndex));
                        const Ticket* ticket = mapIt->second;
                        cout << "Confirmed " << price << " refund for " << ticket->userName << endl;
                        it = lists.erase(it);
                    } else ++it;
                }
            }
        }
    }

    unordered_map<long, Ticket*>::const_iterator view(const long& ticketId) const {
        for (size_t i = 0; i < booking.size(); ++i) {
            int startIndex = booking[i];

            if (!linkedLists[startIndex].empty()) {
                auto& lists = linkedLists[startIndex];

                for (auto it = lists.begin(); it != lists.end(); ) {
                    auto mapIt = it->find(ticketId);
                    if (mapIt != it->end())
                        return mapIt;
                    else ++it;
                }
            }
        }
        return {};
    }

    void view() {
        for (size_t i = 0; i < booking.size(); ++i) {
            int startIndex = booking[i];

            if (!linkedLists[startIndex].empty()) {
                auto& lists = linkedLists[startIndex];

                for (auto& map : lists) {
                    for (auto& entry : map) {
                        const Ticket* ticket = entry.second;
                        string price = getPrice(to_string(startIndex));

                        cout << ticket->row << ticket->place<< " " << ticket->userName << " " << price << endl;
                    }
                }
            }
        }
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
    unordered_map<string, vector<long>> userIds;
    Airport airport;
    long id;
public:
    Helper (Airport& myAirport): airport(myAirport), id(1){}

    void book(const string& date, const string& flight, const string& place, const string& userName) {
        auto plane = airport.planes[make_pair(date, flight)];
        if (plane==NULL)
            cout << "Sorry, no airplane in this day with this flight number :(" << endl;
        else {
            plane->book(place, userName, id);
            userIds[userName].push_back(id);
            id++;
        }
    }

    void returnTicket(const long& id){
        for (auto& pair : airport.planes) {
            const auto& key = pair.first;
            Airplane* airplane = pair.second;
            airplane->findDelete(id);
        }
    }

    void view(const long& id) {
        bool found = false;
        for (const auto& pair : airport.planes) {
            const auto& key = pair.first;
            Airplane* airplane = pair.second;
            auto mapIt = airplane->view(id);
            if (mapIt != nullptr) {
                cout << "Flight " << key.first << ", " << key.second
                     << ", seat " << mapIt->second->place
                     << ", price " << airplane->getPrice(mapIt->second->row) << ", " << mapIt->second->userName << endl;
                found = true;
            }
        }

        if (!found) {
            cout << "No such ticket has been found." << endl;
        }
    }

    void view(const string& date, const string& flight) {
        for (const auto& pair : airport.planes) {
            const auto& key = pair.first;
            Airplane* airplane = pair.second;
            airplane->view();
        }
    }

    void view(const string& userName) {
        int k = 1;
        if (userIds.find(userName) != userIds.end()) {
            const vector<long>& ids = userIds[userName];

            for (long id : ids) {
                cout << k << ". ";
                view(id);
                k++;
            }
        } else cout << "Sorry, it seems like nobode with sucn a name booked a place" << endl;

    }
};

int main()
{
    Airport myAirport;
    FileReader file(myAirport);
    file.read();
    Helper helper(myAirport);
    helper.book("01.01.2023", "JK321", "1A", "Alla");
    helper.book("03.01.2023", "LM654", "40B", "Alla");
    helper.book("01.01.2023", "JK321", "1C", "V");
    //helper.returnTicket(1);
    //helper.view(2);
    //helper.view(1);
    //helper.view("01.01.2023", "JK321");
    helper.view("Alla");
    return 0;
}
