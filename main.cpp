#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <set>
#include <iomanip>

using namespace std;

class Date {
public:
    Date() {
        year = 1;
        month = 1;
        day = 1;
    }

    Date (int new_year, int new_month, int new_day) {

        if (new_month < 1 || new_month > 12){
            throw out_of_range("Month value is invalid: " + to_string(new_month));
        } else if (new_day < 1 || new_day > 31) {
            throw out_of_range("Day value is invalid: " + to_string(new_day));
        }
        year = new_year;
        month = new_month;
        day = new_day;
    }

    int GetYear() const {
        return year;
    };
    int GetMonth() const {
        return month;
    };
    int GetDay() const {
        return day;
    };

private:
    int year = 1; // can be less than zero
    int month = 1;
    int day = 1;
};

ostream& operator<<(ostream& stream, const Date& date){
    stream << setfill('0') << setw(4) << to_string(date.GetYear());
    stream << "-";
    stream << setfill('0') << setw(2) << to_string(date.GetMonth());
    stream << "-";
    stream << setfill('0') << setw(2) << to_string(date.GetDay());
    return stream;
}

bool operator<(const Date& lhs, const Date& rhs){
    if (lhs.GetYear() != rhs.GetYear()){
        return lhs.GetYear() < rhs.GetYear();
    }
    else if (lhs.GetMonth() != rhs.GetMonth()){
        return lhs.GetMonth() < rhs.GetMonth();
    }
    else {
        return lhs.GetDay() < rhs.GetDay();
    }
}

Date ParseDate(const string& date) {
    istringstream date_stream(date);
    bool ok = true;

    int year;
    ok = ok && (date_stream >> year);
    ok = ok && (date_stream.peek() == '-');
    date_stream.ignore(1);

    int month;
    ok = ok && (date_stream >> month);
    ok = ok && (date_stream.peek() == '-');
    date_stream.ignore(1);

    int day;
    ok = ok && (date_stream >> day);
    ok = ok && date_stream.eof();

    if (!ok) {
        throw logic_error("Wrong date format: " + date);
    }
    return Date(year, month, day);
}

class Database {
public:
    void AddEvent(const Date& date, const string& event){
        if (!event.empty()){
            records[date].insert(event);
        }
    }

    bool DeleteEvent(const Date& date, const string& event){
        if (records.count(date) > 0 && records[date].count(event) > 0) {
            records[date].erase(event);
            return true;
        }
        return false;
    }

    int  DeleteDate(const Date& date){
        int to_del = 0;
        if (records.count(date) > 0){
            to_del = records.at(date).size();
            records.erase(date);
        }
        return to_del;
    }

    set<string> Find(const Date& date) const{
        set<string> result;
        if (records.count(date) > 0){
            result = records.at(date);
        }
        return result;
    }


    void Print() const{
        for (const auto& r : records){
            for (auto s : r.second){
                cout << r.first << ' ' << s << endl;
            }
        }
    }

private:
    map<Date, set<string>> records;
};

int main() {
        try {
            Database db;
            string command;

            while (getline(cin, command)) {
                if(command == "") continue;

                stringstream input(command);

                string operation;
                input >> operation;

                if (operation == "Add") {
                    string date_string, event;
                    input >> date_string >> event;
                    const Date date = ParseDate(date_string);
                    if (event != "" || event != " ") {
                        db.AddEvent(date, event);
                    }
                } else if (operation == "Del") {
                    string date_string, event;
                    input >> date_string;
                    if (!input.eof()) {
                        input >> event;
                    }
                    const Date date = ParseDate(date_string);
                    if (event.empty()) {
                        const int num_of_events_to_del = db.DeleteDate(date);
                        cout << "Deleted " << num_of_events_to_del << " events" << endl;
                    } else {
                        if (db.DeleteEvent(date, event)) {
                            cout << "Deleted successfully" << endl;
                        } else {
                            cout << "Event not found" << endl;
                        }
                    }
                } else if (operation == "Find") {
                    string date_string, event;
                    input >> date_string >> event;
                    const Date date = ParseDate(date_string);
                    set<string> events = db.Find(date);

                    for (string e : events) {
                        cout << e << endl;
                    }
                } else if (operation == "Print") {
                    db.Print();
                } else {
                    cout << "Unknown command: " << operation << endl;
                    return 0;
                }
            }
        }
        catch (exception& e){
            cout << e.what() << endl;
            return 0;
        }
    return 0;
}
