#include <iostream>
#include <map>
#include <sstream>
#include <regex>
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
        return lhs.GetYear()<rhs.GetYear();
    }
    else if (lhs.GetMonth() != rhs.GetMonth()){
        return lhs.GetMonth()<rhs.GetMonth();
    }
    else {
        return lhs.GetDay()<rhs.GetDay();
    }
}

class Database {
public:
    void AddEvent(const Date& date, const string& event){
        if (!event.empty()){
            records[date].insert(event);
        }
    }

    bool DeleteEvent(const Date& date, const string& event){
        if (records[date].erase(event)>0)
            return true;
        else
            return false;
    }

    int  DeleteDate(const Date& date){
        int to_del = 0;
        if (records.count(date)>0){
            to_del = records.at(date).size();
            records.erase(date);
        }
        return to_del;
    }

    set<string> Find(const Date& date) const{
        set<string> result;
        if (records.count(date)>0){
            result = records.at(date);
        }
        return result;
    }


    void Print() const{
        for (const auto& r : records){
            //if(r.first.GetYear()<0) continue;
            for (auto s : r.second){
                cout << r.first << ' ' << s << endl;
            }
        }
    }

private:
    map<Date, set<string>> records;
};

istream& operator>>(istream& stream, Date& date){
    int y, m, d;

    set<char> allowed_symbols = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-' };

    string date_string;
    stream >> date_string;

    for (char c : date_string){
        if (allowed_symbols.count(c) == 0){
            throw runtime_error("Wrong date format: " + date_string);
        }
    }

    vector<int> dashes(3, 0);
    int current_index = 0;
    bool isDash = true;

    for (int i = 0; i < date_string.size(); i++){
        if (date_string[i] == '-'){
            isDash = true;
            if (current_index > 2)
                throw runtime_error("Wrong date format: " + date_string);
            ++dashes[current_index];
        }
        else {
            if (isDash){
                ++current_index;
                isDash = false;
            }
        }
    }

    if (dashes[0] > 1 || dashes[1] == 0 || dashes[1]>2 || dashes[2]==0 || dashes[2]>3)
        throw runtime_error("Wrong date format: " + date_string);

    stringstream istream(date_string);

    istream >> y;
    istream.ignore(1);
    istream >> m;
    istream.ignore(1);
    istream >> d;

    date = Date(y, m, d);
    return stream;
}

int main() {
    Database db;

    string command;
    while (getline(cin, command)) {
        // Считайте команды с потока ввода и обработайте каждую
        if (command.empty() || command == " ") continue;
        stringstream input(command);
        string operation;
        string event;
        Date date;

        try{
            input >> operation;

            if (operation == "Add"){
                input >> date >> event;
                if (event != "" || event != " "){
                    db.AddEvent(date, event);
                }
            }
            else if (operation == "Del"){
                input >> date >> event;
                if (event.empty()){
                    int num_to_del = db.DeleteDate(date);
                    cout << "Deleted " << num_to_del << " events" << endl;
                }
                else
                {
                    if (db.DeleteEvent(date, event)){
                        cout << "Deleted successfully" << endl;
                    }
                    else{
                        cout << "Event not found" << endl;
                    }
                }
            }
            else if (operation == "Find"){
                input >> date;
                set<string> events = db.Find(date);

                for (string e : events){
                    cout << e << endl;
                }
            }
            else if (operation == "Print"){
                db.Print();
            }
            else {
                cout << "Unknown command: " << operation << endl;
                return 0;
            }
        }
        catch (out_of_range& ofr){
            cout << ofr.what() << endl;
            return 0;
        }
        catch (runtime_error& re){
            cout << re.what() << endl;
            return 0;
        }
        //catch (logic_error& lr) {
          //  cout << lr.what() << endl;
        //}
        catch (exception& ex){
            cout << ex.what() << endl;
            return 0;
        }
    }

    return 0;
}