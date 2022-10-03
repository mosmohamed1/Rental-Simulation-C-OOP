#include <iostream>
#include <iomanip>

using namespace std;
class DayOfYear
{
private:
    unsigned short int dayOfYear;
    unsigned short int year;
    const unsigned static short days[];

public:

    DayOfYear(short dayOfYear = 1, short year = 365)
    {
        this->dayOfYear = dayOfYear;
        this->year = year;
    }

    DayOfYear(int year, int month, int day)
    {

        this->dayOfYear = days[month - 1] + day;

        this->year = year;
    }


    unsigned short int get_dayOfYear()
    {
        return this->dayOfYear;
    }

    DayOfYear operator++()
    {
        short newYear;
        short newDayOfYear;
        if (this->dayOfYear % 365 == 0)
        {
            newYear = this->year + 1;
            newDayOfYear = 1;
        }
        newYear = this->year;
        newDayOfYear = this->dayOfYear + 1;
        return DayOfYear(newDayOfYear, newYear);
    }

    friend ostream& operator<<(ostream& out, const DayOfYear& dt);
    friend istream& operator>>(istream& in, DayOfYear& d);
};
ostream& operator<<(ostream& out, const DayOfYear& dt)
{
    int day, month, year;
    for (int Month = 0; Month <= 12; Month++) {

        if (dt.dayOfYear <= dt.days[Month]) {

            month = Month;
            day = dt.dayOfYear - dt.days[Month - 1];
            break;
        }

        year = dt.year;
    }
    out << year << '-' << month << '-' << day;
    return out;
}
istream& operator>>(istream& in, DayOfYear& dt)
{
    int Day, Month, Year;
    in >> Year;
    in.ignore(1, '-');
    in >> Month;
    in.ignore(1, '-');
    in >> Day;
    dt.year = Year;
    dt.dayOfYear = dt.days[Month - 1] + Day;
    return in;
}
const unsigned short DayOfYear::days[13] = { 0,31,59,90,120,151,181,212,243,273,304,335,366 };
class Vehicle
{
private:
    int vehicle_no;
    string model;
    float price24h;

public:
    Vehicle(int vehicle_no, string model, float price24h)
    {
        this->vehicle_no = vehicle_no;
        this->model = model;
        this->price24h = price24h;
    }

    ~Vehicle()
    {

    }

    int get_vehicle_no()
    {
        return this->vehicle_no;
    }

    string get_model()
    {
        return this->model;
    }

    float get_price(float days)
    {
        return this->price24h * days;
    }

    virtual void print() = 0;
    friend class Bike;
    friend class EMotorVehicle;
    friend class EBike;
    friend class EScooter;
    friend  class Rental;
};
class Bike : public Vehicle
{
public:

    Bike(int vehicle_no, string model) :Vehicle(vehicle_no, model, price24h)
    {
        this->vehicle_no = vehicle_no;
        this->model = "City";
        this->price24h = 9.99;
    }
    virtual void print()
    {
        cout << "SCHEDULE FOR" << " : " << this->get_model() << this->get_vehicle_no() << "(Bike)" << endl;
    }
};

class EMotorVehicle : public Vehicle
{
public:

    EMotorVehicle(int vehicle_no, string model, float price24h) :Vehicle(vehicle_no, model, price24h)
    {
        this->vehicle_no = vehicle_no;
        this->model = model;
        this->price24h = price24h;
    }

    virtual bool is_streetLegal() = 0;
    virtual void print()
    {

        if (!this->is_streetLegal()) cout << "(not street legal)" << endl;
        else cout << "SCHEDULE FOR" << " : " << this->model << this->vehicle_no;

    }


};
class EBike : public EMotorVehicle
{
    EBike(int vehicle_no, string model, float price24h) :EMotorVehicle(vehicle_no, model, price24h)
    {
        this->vehicle_no = vehicle_no;
        this->model = "Trekky";
        this->price24h = 29.99;
    }
    bool is_streetLegal()
    {
        return true;
    }
    virtual void print()
    {
        EMotorVehicle::print();
        cout << "(EBike)" << endl;

    }
};
class EScooter : public EMotorVehicle
{
private:
    bool streetLegal;
    EScooter(int vehicle_no, string model, float price24h, bool streeLegal) :EMotorVehicle(vehicle_no, model, price24h)
    {
        this->vehicle_no = vehicle_no;
        this->model = "Scooty";
        this->price24h = 19.99;
        this->streetLegal = false;
    }
    bool is_streetLegal()
    {
        return this->streetLegal;
    }
    virtual void print()
    {
        EMotorVehicle::print();
        cout << "(EScooter)" << endl;

    }
};
class Rental : public Vehicle
{
private:
    const int no = 500;
    DayOfYear from;
    int days;
    string customer;
    static int last_no;
public:
    Rental(string customer, DayOfYear from, int days) : no(Rental::last_no++), Vehicle(vehicle_no, model, price24h)
    {
        this->customer = customer;
        this->from = from;
        this->days = days;
    }
    int get_days()
    {
        return(this->days);
    }
    DayOfYear get_from()
    {
        return this->from;
    }
    DayOfYear get_until()
    {
        DayOfYear until(this->from);
        int rentalDays = this->days;
        while (rentalDays--)
        {
            until = ++until;
        }
        return until;

    }
    void print()
    {
        cout << "\n" << get_from() << " to " << get_until() << " rental no" << last_no << " for " << this->customer;
    }
    friend class Schedule;
};
int Rental::last_no = 0;
class Schedule : public Rental
{
private:
    Vehicle* vehicle;
    Rental* year[365];
    Schedule* next;
public:
    Schedule(Vehicle* vehicle) : Rental(customer, from, days)
    {
        this->vehicle = vehicle;
        this->next = NULL;
        for (int i = 0; i < 365; i++) {
            this->year[i] = NULL;
        }
    }
    Vehicle* get_vehicle() {
        return this->vehicle;
    }

    Schedule* get_next() {
        return this->next;
    }

    void set_next(Schedule* next) {
        this->next = next;
    }

    bool isFree(DayOfYear date, int days) {

        for (int i = date.get_dayOfYear() - 1; i < date.get_dayOfYear() - 1 + days; i++) {
            if (this->year[i] != NULL) {
                return false;
            }
        }
        return true;
    }
    float book(string person, DayOfYear date, int days) {
        Rental* rental = new Rental(person, date, days);
        for (int i = 0; i < days; i++) {
            this->year[date.get_dayOfYear() - 1 + i] = rental;
        }

        return this->vehicle->get_price(days);
    }

    void print(DayOfYear date) {
        this->vehicle->print();

        if (this->year[date.get_dayOfYear() - 1] != NULL) {
            cout << endl;
            this->year[date.get_dayOfYear() - 1]->print();
        }
    }
    void print() {
        cout << "SCHEDULE FOR ";
        vehicle->print();
        for (int i = 0; i < 365; i++) {
            if (this->year[i] != NULL && this->year[i]->get_from().get_dayOfYear() == i + 1) {
                cout << endl;
                this->year[i]->print();
                cout << ", total: " << this->get_vehicle()->get_price(this->year[i]->get_days()) << " EUR";
            }
        }
    }
    friend class Agency;
};
class Agency : public Schedule
{
private:

    float profitPercent;
    float profit;
    Schedule* head;
    Schedule* load;

protected:
    Schedule* searchFor(int sf)
    {
        for (Schedule* p = head; p < load; ++p)
        {
            if ((p->get_vehicle())->get_vehicle_no() == sf)
            {
                return p;
            }
        }
    }
public:
    Agency() : Schedule(vehicle)
    {
        this->profitPercent = 0.2;
        this->profit = 0.00;
        this->head = NULL;
        this->load = NULL;
    }
    float get_profit()
    {
        return this->profit;
    }
    void set_profitPercentage(float sp)
    {
        profitPercent = sp;
    }
    void add(Vehicle* s) {

        if (this->head == NULL) {
            head = new Schedule(s);
            load = head;
        }

        else
        {
            load->set_next(new Schedule(s));
            load = load->get_next();

        }


    }

    bool isFree(int vehical_no, DayOfYear from, int days)
    {
        return searchFor(vehical_no)->isFree(from, days);
    }
    float book(int vehical_no, string customer, DayOfYear from, int days)
    {
        Schedule* sf = this->searchFor(vehical_no);

        float price = Schedule::book(customer, from, days);
        // float price = 5;


        return price + price * this->profitPercent;
    }
    int chooseVehicle()
    {
        return this->get_vehicle_no();
    }
    void print(DayOfYear from)
    {
        cout << "SCHEDULE FOR " << from << endl;
        Schedule* s = head;
        while (s != nullptr)
        {
            s->print(from);
            s = s->get_next();
        }
    }
    void print()
    {
        Schedule* s = head;
        while (s != nullptr)
        {
            s->print();
            s = s->get_next();
        }
        cout << "profit: " << fixed << setprecision(2)
            << profit << " EUR" << endl;
    }


};




int main()
{
    DayOfYear today(2021, 01, 01);

    Agency mob = Agency();
    int vehicle_no, days;
    float price;
    string costumer;
    DayOfYear from;
    float k;
    int Bikes = 0;
    int EBikes = 0;



    while (1)
    {

        cout << endl;
        cout << " MENUE" << endl;
        cout << "A end of simulation" << endl;
        cout << "B set new simulation date" << endl;
        cout << "C new rental manually" << endl;
        cout << "D print rental of today " << today << endl;
        cout << "E print rental of tomorrow " << ++today << endl;
        cout << "F print all rentals" << endl;
        cout << "G print agency profit" << endl;
        cout << "H set agency profit percent" << endl;
        cout << "I add bikes" << endl;
        cout << "J add E-bikes" << endl;
        cout << "K add E-scooters" << endl;
        cout << "L new rental simulation" << endl;
        cout << "your choice: ";

        char choice;
        cin >> choice;
        switch (choice) {
        case 'a':
            cout << "Simulation ended successfully" << endl;
            break;
        case 'b':
            cout << "input date of today: ";
            cin >> today;
            break;
        case'c':
            mob.print();
            cout << " choose vehicle no:";
            cin >> vehicle_no;
            cout << " day of rental:";
            cin >> from;
            cout << " no of days:";
            cin >> days;
            cout << " costumer name:";
            cin >> costumer;
            price = mob.book(vehicle_no, costumer, from, days);
            cout << "price : " << price << endl;

            break;
        case 'd':
            cout << "SCHDULE FOR : " << today;
            break;
        case 'e':
            cout << "SCHDULE FOR : " << ++today;
            break;
        case 'f':
            int ppk;
            cout << "agency profit percent:";
            cin >> ppk;

            break;
        case 'g':
            cout << "agency profit";
            cout << mob.get_profit();
            break;
        case'h':
            cout << "agency profit percent:";
            cin >> k;
            mob.set_profitPercentage(k / 100);

            break;

        case 'i':
            cout << " how many Bikes in this simulation? ";
            int bike_no;
            cin >> bike_no;
            for (int i = 0; i < bike_no; i++) {
                Bikes++;
                mob.add(new Bike(Bikes, "City"));
            }
            break;
        case 'j':
            cout << " how many EBikes in this simulation?";
            int Ebike_no;
            cin >> Ebike_no;
            // for (int i = 0; i < Ebike_no; i++) {
           //     EBikes++;
         //       mob.add(new EBike(EBikes, "Trokky"));
          //  }
            break;
        case 'k':
            cout << " how many EScooters  in this simulation?";
            //cin>> k;
            break;
        case 'l':

            break;
        }
        if (choice == 'a')
            break;
    }


    return 0;
}

