// #include <fstream>
// #include <iostream>
// #include <list>
// #include <map>
// #include <sstream>
// #include <unordered_map>
// #include <vector>
// using namespace std;
//
// struct Ticket {
//     string place;
//     string userName;
//     long id;
//     string row;
//     Ticket(const string& inName, const long& inId, const string& inPlace, const string& inRow): userName(inName), id(inId), place(inPlace), row(inRow){}
// };
//
// class Airplane {
//     string flight;
//     string date;
//     int numSeat, maxSeat;
//     vector<int> booking;
//     vector<list<unordered_map<long, Ticket*>>> linkedLists;
//     map<int, string> pricing;
//
// public:
//     Airplane(const string& inDate, const string& inFlight, const int& inSeatNum, const map<int, string> inPricing):
//     date(inDate), flight(inFlight), numSeat(inSeatNum), pricing(inPricing) {
//
//         maxSeat = pricing.rbegin()->first;
//         for (int i=0; i <maxSeat; i++) {
//             booking.push_back(i);
//             linkedLists.emplace_back();
//         }
//     }
//     void removeNumber(vector<long>& v, int numberToRemove) {
//         for (auto it = v.begin(); it != v.end(); ) {
//             if (*it == numberToRemove)
//                 it = v.erase(it);
//             else ++it;
//
//         }
//     }
//     string getPrice(const string& row) {
//         auto it = pricing.lower_bound(stoi(row) + 1); //+1 for edge case
//
//         return it->second;
//     }
//
//     void book(string seat, string userName, const long id) {
//         unordered_map<long, Ticket*> data;
//
//         string row = seat.substr(0, seat.size() - 1);
//         string place = seat.substr(seat.size() - 1);
//
//
//         auto* ticket = new Ticket(userName, id, place, row); //made ticket
//
//         if (!linkedLists[stoi(row) - 1].empty()) {
//             auto& lists = linkedLists[stoi(row) - 1];
//             for (auto& map : lists) {
//                 for (auto& entry : map) {
//                     if (entry.second->place == place) {
//                         cout << "Sorry, this place is already booked. Try another one." << endl;
//                         delete ticket; // Clean up to avoid memory leak
//                         return;
//                     }
//                 }
//             }
//         }
//
//
//         if (!linkedLists[stoi(row) - 1].front().empty() && (linkedLists[stoi(row) - 1].front().find(ticket->id) != linkedLists[stoi(row) - 1].front().end())) {
//             cout << "Sorry, this place is already booked. Try another one." << endl;
//             delete ticket;
//             return;
//         }
//         data[ticket->id] = ticket; //made a map
//         //add this as a list element
//         linkedLists[stoi(row) - 1].push_front(data); //-1 because it starts with zero
//         cout << "Confirmed with ID " << id << endl;
//
//     }
//
//     bool isFindDelete(const long& ticketId, unordered_map<string, vector<long>>& userIds){
//         bool found = false;
//         for (size_t i = 0; i < booking.size(); ++i) {
//             int startIndex = booking[i];
//
//             if (!linkedLists[startIndex].empty()) {
//                 auto& lists = linkedLists[startIndex];
//
//                 for (auto it = lists.begin(); it != lists.end(); ) {
//                     auto& map = *it;
//                     auto mapIt = map.find(ticketId);
//                     if (mapIt != map.end()) {
//                         found = true;
//
//                         string price = getPrice(to_string(startIndex));
//                         const Ticket* ticket = mapIt->second;
//                         cout << "Confirmed " << price << " refund for " << ticket->userName << endl;
//                         map.erase(mapIt);
//                         removeNumber(userIds[ticket->userName], ticketId);
//                         break;
//
//                     } else ++it;
//                 }
//                 if (found) {
//                     break;
//                 }
//             }
//         }
//         return found;
//     }
//
//     unordered_map<long, Ticket*>::const_iterator view(const long& ticketId) const {
//         for (size_t i = 0; i < booking.size(); ++i) {
//             int startIndex = booking[i];
//
//             if (!linkedLists[startIndex].empty()) {
//                 auto& lists = linkedLists[startIndex];
//
//                 for (auto it = lists.begin(); it != lists.end(); ) {
//                     auto mapIt = it->find(ticketId);
//                     if (mapIt != it->end())
//                         return mapIt;
//                     else ++it;
//                 }
//             }
//         }
//         return {};
//     }
//
//     void view() {
//         for (size_t i = 0; i < booking.size(); ++i) {
//             int startIndex = booking[i];
//
//             if (!linkedLists[startIndex].empty()) {
//                 auto& lists = linkedLists[startIndex];
//
//                 for (auto& map : lists) {
//                     for (auto& entry : map) {
//                         const Ticket* ticket = entry.second;
//                         string price = getPrice(to_string(startIndex));
//
//                         cout << ticket->row << ticket->place<< " " << ticket->userName << " " << price << endl;
//                     }
//                 }
//             }
//         }
//     }
//
//     void check() {
//         for (size_t i = 0; i < booking.size(); ++i) {
//             vector takenPlaces(numSeat, false);
//             int startIndex = booking[i];
//
//             if (linkedLists[startIndex].empty()) {
//                 for (char i = 'A'; i < 'A' + numSeat; i++) {
//                     string price = getPrice(to_string(startIndex));
//                     cout << startIndex + 1<< i << " " << price << endl;
//                 }
//             }
//             else {
//                 auto& lists = linkedLists[startIndex];
//                 for (auto& map : lists) {
//                     for (auto& entry : map) {
//                         takenPlaces[entry.second->place[0] - 'A'] = true;
//                     }
//                 }
//                 for (char i = 'A'; i < 'A' + numSeat; i++) {
//                     if (!takenPlaces[i - 'A']) {
//                         string price = getPrice(to_string(startIndex));
//                         cout << startIndex + 1 << i << " " << price << endl;
//                     }
//                 }
//             }
//         }
//     }
// };
//
// struct pair_hash {
//         size_t operator () (const pair<string, string>& p) const {
//             size_t h1 = hash<std::string>{}(p.first);
//             size_t h2 = hash<std::string>{}(p.second);
//             return h1 ^ (h2 << 1);
//         }
//     };
//
// class Airport {
// public:
//     unordered_map<pair<string, string>, Airplane*, pair_hash> planes;
//     Airport() {}
//     void addPlane(string inFlight, string inDate, int inSeatNum, map<int, string> inPricing) {
//
//         planes[make_pair(inDate, inFlight)] = new Airplane (inDate, inFlight, inSeatNum, inPricing);
//     }
// };
//
// class FileReader {
//     string path = "C:/Users/kovko/CLionProjects/OPD_Assignment1/config.txt";
//     string date;
//     string flightNo;
//     int numSeat;
//     int numRow;
//     string price, seats;
//     string endSeat;
//     map<int, string> pricing;
//     Airport& airport;
// public:
//     FileReader(Airport& inAirport): airport(inAirport) {}
//     void read() {
//         ifstream inputFile(path);
//
//         if (!inputFile) {
//             cerr << "No file has been found: " << path << endl;
//             return;
//         }
//
//         string line;
//         istringstream iss(line);
//         getline(inputFile, line);
//
//         while (getline(inputFile, line)) {
//             istringstream iss(line);
//             pricing.clear();
//             iss >> date >> flightNo >> numSeat;
//
//             while (iss >> seats >> price) {
//                 endSeat = seats.substr(seats.find('-') + 1);
//                 pricing[stoi(endSeat)] = price;
//             }
//
//             airport.addPlane(flightNo, date, numSeat, pricing);
//         }
//         inputFile.close();
//     }
// };
//
//
// class Helper {
//     unordered_map<string, vector<long>> userIds;
//     Airport airport;
//     long id;
// public:
//     Helper (Airport& myAirport): airport(myAirport), id(1){}
//
//     void check(const string& date, const string& flight) {
//         bool found = false;
//         for (const auto& pair : airport.planes) {
//             const auto& key = pair.first;
//             Airplane* airplane = pair.second;
//             if (key.first == date && key.second == flight) {
//                 airplane->check();
//                 found = true;
//             }
//         }
//         if (!found)
//             cout << "Sorry, no airplane in this day with this flight number :(" << endl;
//     }
//
//     void book(const string& date, const string& flight, const string& place, const string& userName) {
//         auto plane = airport.planes[make_pair(date, flight)];
//         if (plane==NULL)
//             cout << "Sorry, no airplane in this day with this flight number to book a ticket :(" << endl;
//         else {
//             plane->book(place, userName, id);
//             userIds[userName].push_back(id);
//             id++;
//         }
//     }
//
//     void returnTicket(const long& id){
//         bool found = false;
//         for (auto& pair : airport.planes) {
//             const auto& key = pair.first;
//             Airplane* airplane = pair.second;
//             found = airplane->isFindDelete(id, userIds);
//         }
//         if (!found) cout << "No such ID" << endl;
//     }
//
//     void view(const long& id) {
//         bool found = false;
//         for (const auto& pair : airport.planes) {
//             const auto& key = pair.first;
//             Airplane* airplane = pair.second;
//             auto mapIt = airplane->view(id);
//             if (mapIt != nullptr) {
//                 cout << "Flight " << key.first << ", " << key.second
//                      << ", seat " << mapIt->second->row << mapIt->second->place
//                      << ", price " << airplane->getPrice(to_string(stoi(mapIt->second->row) - 1)) << ", " << mapIt->
//                      second->userName << endl;
//                 found = true;
//             }
//         }
//
//         if (!found) {
//             cout << "No such ticket has been found." << endl;
//         }
//     }
//
//     void view(const string& date, const string& flight) {
//         bool found = false;
//         for (const auto& pair : airport.planes) {
//             const auto& key = pair.first;
//             Airplane* airplane = pair.second;
//             if (key.first == date && key.second == flight){
//                 airplane->view();
//                 found = true;
//             }
//         }
//         if (!found) cout << "No such a flight" << endl;
//     }
//
//     void view(const string& userName) {
//         int k = 1;
//         if (userIds.find(userName) != userIds.end()) {
//             const vector<long>& ids = userIds[userName];
//
//             for (long id : ids) {
//                 cout << k << ". ";
//                 view(id);
//                 k++;
//             }
//         } else cout << "Sorry, it seems like nobody with such a name booked a place" << endl;
//
//     }
// };
//
// int main()
// {
//     Airport myAirport;
//     FileReader file(myAirport);
//     file.read();
//     Helper helper(myAirport);
//     //helper.check("0703.2023", "PA345");
//     helper.book("07.03.2023", "PA345", "10A", "Alla");
//     helper.book("07.03.2023", "PA345", "11A", "Oliver");
//     helper.book("07.03.2023", "PA345", "12A", "Oliver");
//     helper.book("11.03.2023", "NM678", "2D", "Alla");
//     //helper.view("All"); //delete user?
//     //helper.view(6);
//     //helper.returnTicket(2);
//     //helper.view("Oliver");
//     //helper.view("0.03.2023", "LB567");
//     return 0;
// }


#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <unordered_map>
#include <vector>
using namespace std;

struct Ticket {
    int row;
    char seat;
    string userName;
};

class PayFolder {
    vector<Ticket> tickets;
    unordered_map<string, vector<int>> userIds;
};

class Airplane {
    int numSeat, maxRow;
public:
    vector<bool> availability;
    Airplane(int inNumSeat, int inMaxRow): numSeat(inNumSeat), maxRow(inMaxRow) {
        availability.resize(maxRow * numSeat, false);
    }

    void check() {
        for (int i= 0; i < maxRow * numSeat; i++) {
            if (!availability[i]) {
                cout << i / numSeat + 1 << static_cast<char>(i % numSeat + 'A') << endl;
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
    void addPlane(const string& inFlight, const string& inDate, const int& inSeatNum, const int& maxRow) {

        planes[make_pair(inDate, inFlight)] = new Airplane (inSeatNum, maxRow);
    }
};

class FileReader {
    string path = "C:/Users/kovko/CLionProjects/OPD_Assignment1.2.0/config.txt";
    string date;
    string flightNo;
    int numSeat = 0;
    int numRow;
    string price, seats;
    string endSeat;
    map<int, string> pricing;
    Airport& airport;

public:
    FileReader(Airport& inAirport) : airport(inAirport) {}

    void read() {
        ifstream inputFile(path);

        if (!inputFile) {
            cerr << "No file has been found: " << path << endl;
            return;
        }

        string line;
        istringstream iss(line);
        getline(inputFile, line);

        // Process each line in the file
        while (getline(inputFile, line)) {
            istringstream iss(line);
            pricing.clear(); // Clear the map for each new flight entry

            if (!(iss >> date >> flightNo >> numSeat)) {
                cerr << "Error reading date, flightNo, or numSeat from line: " << line << endl;
                continue;
            }

            // Process the seat and price pairs
            while (iss >> seats >> price) {
                size_t dashPos = seats.find('-');
                if (dashPos == string::npos) {
                    cerr << "Invalid seat range format in line: " << line << endl;
                    continue;
                }
                endSeat = seats.substr(dashPos + 1);

                int endSeatNumber = stoi(endSeat);
                pricing[endSeatNumber] = price;

            }

            if (!pricing.empty()) {
                airport.addPlane(flightNo, date, numSeat, pricing.rbegin()->first);
            }
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

    void check(const string& date, const string& flight) {
        bool found = false;
        for (const auto& pair : airport.planes) {
            const auto& key = pair.first;
            Airplane* airplane = pair.second;
            if (key.first == date && key.second == flight) {
                airplane->check();
                found = true;
            }
        }
        if (!found)
            cout << "Sorry, no airplane in this day with this flight number :(" << endl;
    }
};

int main()
{
    Airport myAirport;
    FileReader file(myAirport);
    file.read();
    Helper helper(myAirport);
    helper.check("01.03.2023", "TI678");
    cout << "Hello, World!" << endl;
    return 0;
}
