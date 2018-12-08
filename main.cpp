#include <iostream>
#include <map>
#include <sstream>
#include <set>
#include <iomanip>

class Date
        {
public:
    Date (int new_year, int new_month, int new_day)
    {

        if (new_month < 1 || new_month > 12)
        {
            throw std::out_of_range("Month value is invalid: " + std::to_string(new_month));
        } else if (new_day < 1 || new_day > 31)
        {
            throw std::out_of_range("Day value is invalid: " + std::to_string(new_day));
        }
        year = new_year;
        month = new_month;
        day = new_day;
    }

    int GetYear() const
    {
        return year;
    };

    int GetMonth() const
    {
        return month;
    };

    int GetDay() const
    {
        return day;
    };

private:
    int year; // can be less than zero
    int month;
    int day;
};

std::ostream& operator << (std::ostream& stream, const Date& date)
{
    stream << std::setfill('0') << std::setw(4) << std::to_string(date.GetYear());
    stream << "-";
    stream << std::setfill('0') << std::setw(2) << std::to_string(date.GetMonth());
    stream << "-";
    stream << std::setfill('0') << std::setw(2) << std::to_string(date.GetDay());
    return stream;
}

bool operator < (const Date& lhs, const Date& rhs)
{
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

Date ParseDate(const std::string& date)
{
    std::istringstream date_stream(date);
    bool flag = true;

    int year;
    flag = flag && (date_stream >> year);
    flag = flag && (date_stream.peek() == '-');
    date_stream.ignore(1);

    int month;
    flag = flag && (date_stream >> month);
    flag = flag && (date_stream.peek() == '-');
    date_stream.ignore(1);

    int day;
    flag = flag && (date_stream >> day);
    flag = flag && date_stream.eof();

    if (!flag)
    {
        throw std::logic_error("Wrong date format: " + date);
    }
    return Date(year, month, day);
}

class Database
        {
public:
    void AddEvent(const Date& date, const std::string& event)
    {
        if (!event.empty())
        {
            records[date].insert(event);
        }
    }

    bool DeleteEvent(const Date& date, const std::string& event)
    {
        if (records.count(date) > 0 && records[date].count(event) > 0)
        {
            records[date].erase(event);
            return true;
        }
        return false;
    }

    int  DeleteDate(const Date& date)
    {
        int to_del = 0;
        if (records.count(date) > 0)
        {
            to_del = records.at(date).size();
            records.erase(date);
        }
        return to_del;
    }

    std::set<std::string> Find(const Date& date) const
    {
        std::set<std::string> result;
        if (records.count(date) > 0)
        {
            result = records.at(date);
        }
        return result;
    }


    void Print() const
    {
        for (const auto& r : records)
        {
            for (auto s : r.second)
            {
                std::cout << r.first << ' ' << s << std::endl;
            }
        }
    }

private:
    std::map<Date, std::set<std::string>> records;
};

int main()
{
        try
        {
            Database db;
            std::string command;

            while (getline(std::cin, command))
            {
                if(command == "") continue;

                std::stringstream input(command);

                std::string operation;
                input >> operation;

                std::map<std::string, char> operations_codes = {{"Add", 'A'}, {"Del",'D'}, {"Find", 'F'}, {"Print",'P'}};

                char operation_code = operations_codes[operation];

                switch(operation_code)
                {
                    case 'A' :
                    {
                        std::string date_string, event;
                        input >> date_string >> event;
                        const Date date = ParseDate(date_string);
                        if (event != "" || event != " ") {
                            db.AddEvent(date, event);
                        }
                        break;
                    }
                    case 'D' :
                    {
                        std::string date_string, event;
                        input >> date_string;
                        if (!input.eof()) {
                            input >> event;
                        }

                        const Date date = ParseDate(date_string);

                        if (event.empty()) {
                            const int num_of_events_to_del = db.DeleteDate(date);
                            std::cout << "Deleted " << num_of_events_to_del << " events" << std::endl;
                        } else {
                            if (db.DeleteEvent(date, event)) {
                                std::cout << "Deleted successfully" << std::endl;
                            } else {
                                std::cout << "Event not found" << std::endl;
                            }
                        }
                        break;
                    }
                    case 'F' :
                    {
                        std::string date_string, event;
                        input >> date_string >> event;
                        const Date date = ParseDate(date_string);
                        std::set<std::string> events = db.Find(date);

                        for (std::string e : events) {
                            std::cout << e << std::endl;
                        }
                        break;
                    }
                    case 'P' :
                    {
                        db.Print();
                        break;
                    }
                    default:
                        std::cout << "Unknown command: " << operation << std::endl;
                        return 0;
                }
            }
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << std::endl;
            return 0;
        }
    return 0;
}
