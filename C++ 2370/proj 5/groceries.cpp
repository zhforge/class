#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>
#include <list>
#include "split.h"

using namespace std;

int find_item_idx(int item_id);
int find_cust_idx(int cust_id);

// classes
class Payment{
protected:
    double amount;
public:
    virtual string print_detail() = 0;

    friend class Order;
};

class Credit : public Payment{
private:
    string card_number;
    string expiration;
public:
    Credit(double amount, string card_number, string expiration) {
        this->amount = amount;
        this->card_number = card_number;
        this->expiration = expiration;
    }
    string print_detail() override {
        ostringstream OSS;
        OSS << "Amount: $" << fixed << setprecision(2) << amount
            << ", Paid by Credit card " << card_number << ", exp. " << expiration;
        return OSS.str();
    }
};

class PayPal : public Payment{
private:
    string paypal_id;
public:
    PayPal(double amount, string paypal_id) {
        this->amount = amount;
        this->paypal_id = paypal_id;
    }
    string print_detail() override {
        ostringstream OSS;
        OSS << "Amount: $" << fixed << setprecision(2) << amount
            << ", Paid by Paypal ID: " << paypal_id;
        return OSS.str();
    }
};

class WireTransfer : public Payment{
private:
    string bank_id;
    string account_id;
public:
    WireTransfer(double amount, string bank_id, string account_id) {
        this->amount = amount;
        this->bank_id = bank_id;
        this->account_id = account_id;
    }
    string print_detail() override {
        ostringstream OSS;
        OSS << "Amount: $" << fixed << setprecision(2) << amount
            << ", Paid by Wire transfer from Bank ID " << bank_id << ", Account# " << account_id;
        return OSS.str();
    }
};

class Item {
private:
    int item_id;
    string description;
    double price;
public:
    Item(int item_id, string description, double price) {
        this->item_id = item_id;
        this->description = description;
        this->price = price;
    }

    friend class LineItem;
    friend int find_item_idx(int item_id);
};
vector<Item> item_list;

class LineItem {
private:
    int item_id;
    int qty;
public:
    LineItem(int item_id, int qty) {
        this->item_id = item_id;
        this->qty = qty;
    }
    friend bool operator<(const LineItem& item1, const LineItem& item2) {
        return item1.item_id < item2.item_id;
    }
    double sub_total() {
        return item_list[find_item_idx(item_id)].price * qty;
    }
    string print_detail() {
        ostringstream OSS;
        OSS << "        Item " << item_id << ": \"" << item_list[find_item_idx(item_id)].description
            << "\", " << qty << " @ " << fixed << setprecision(2)
            << item_list[find_item_idx(item_id)].price;
        return OSS.str();
    }

    friend class Order;
};

class Customer {
private:
    int cust_id;
    string name;
    string street;
    string city;
    string state;
    string zip;
    string phone;
    string email;
public:
    Customer(int cust_id, string name, string street, string city, 
             string state, string zip, string phone, string email) {
        this->cust_id = cust_id;
        this->name = name;
        this->street = street;
        this->city = city;
        this->state = state;
        this->zip = zip;
        this->phone = phone;
        this->email = email;
    }
    string print_detail() {
        ostringstream OSS;
        OSS << "Customer ID #" << cust_id << ":" << endl
            << name << ", ph. " << phone << ", email: " << email << endl
            << street << endl
            << city << ", " << state << " " << zip;
        return OSS.str();
    }

    friend class Order;
    friend int find_cust_idx(int cust_id);
};
vector<Customer> customer_list;

class Order {
private:
    int order_id;
    string order_date;
    int cust_id;
    vector<LineItem> line_items;
    Payment* payment;
public:
    Order(int order_id, string order_date, int cust_id, vector<LineItem> line_items, Payment* payment) {
        this->order_id = order_id;
        this->order_date = order_date;
        this->cust_id = cust_id;
        this->line_items = line_items;
        payment->amount = total();
        this->payment = payment;
    }
    ~Order() {
        //delete payment;
    }
    /*
    Order(const Order& origObject) {
        payment = new Payment;
        *payment = *(origObject.payment);
    }
    */
    double total() {
        double total = 0;
        for (auto line_item : line_items) {
            total += line_item.sub_total();
        }
        return total;
    }
    string print_order() {
        ostringstream OSS;
        OSS << "===========================" << endl
            << "Order #" << order_id << ", Date: " << order_date << endl
            << payment->print_detail() << endl << endl
            << customer_list[find_cust_idx(cust_id)].print_detail() << endl << endl
            << "Order Detail:" << endl;
        for (auto line_item : line_items) {
            OSS << line_item.print_detail() << endl;
        }
        return OSS.str();
    }
};
list<Order> orders;


// customers
int find_cust_idx(int cust_id) {
    for (int i = 0; i < customer_list.size(); i++) {
        if (customer_list[i].cust_id == cust_id) {
            return i;
        }
    }

    throw invalid_argument("No customer found with that ID.");
}

void read_customers(const string& file) {
    ifstream inFS;
    string line;

    inFS.open(file);
    if (!inFS.is_open()) {
        cout << "Could not open file " << file << endl;
        return;
    }

    getline(inFS, line);
    while (!inFS.fail()) {
        auto fields = split(line, ',');
        customer_list.push_back(Customer(stoi(fields[0]), fields[1], fields[2],
                                fields[3], fields[4], fields[5], fields[6], fields[7]));
        getline(inFS, line);
    }
    if (!inFS.eof()) {
        cout << "Input failure before reaching end of file." << endl;
    }
}

// items
int find_item_idx(int item_id) {
    for (int i = 0; i < item_list.size(); i++) {
        if (item_list[i].item_id == item_id) {
            return i;
        }
    }

    throw invalid_argument("No item found with that ID, please try again.");
}

void read_items(const string& file) {
    ifstream inFS;
    string line;

    inFS.open(file);
    if (!inFS.is_open()) {
        cout << "Could not open file " << file << endl;
        return;
    }

    getline(inFS, line);
    while (!inFS.fail()) {
        auto fields = split(line, ',');
        item_list.push_back(Item(stoi(fields[0]), fields[1], stod(fields[2])));
        getline(inFS, line);
    }
    if (!inFS.eof()) {
        cout << "Input failure before reaching end of file." << endl;
    }
}

// orders
/* old function from part 1
void one_customer_order() {
    int cust_id;
    bool found_id = false;
    int cust_num;
    int item_id;
    int item_num;
    int total_items = 0;
    double total_price = 0.00;

    cout << "Customers: " << customer_list.size() << endl
         << "Items: " << item_list.size() << endl << endl;

    // customer login
    cout << "Enter your customer ID: ";
    cin >> cust_id;
    
    for (int i = 0; i < customer_list.size(); i++) {
        if (customer_list[i].id == cust_id) {
            found_id = true;
            cust_num = i;
            break;
        }
    }
    if (!found_id) {
        throw invalid_argument("No customer found with that ID.");
    }

    // item purchasing
    do {
        found_id = false;

        cout << "Enter the ID of the item to purchase (0 to exit): ";
        cin >> item_id;

        if (item_id == 0) {
            break;
        }

        for (int i = 0; i < item_list.size(); i++) {
            if (item_list[i].id == item_id) {
                found_id = true;
                item_num = i;
                break;
            }
        }
        if (!found_id) {
            cout << "No item found with that ID, please try again." << endl;
            continue;
        }

        // successfully found item
        total_items++;
        cout << item_list[item_num].name << ", $" << item_list[item_num].price << endl;
        total_price += item_list[item_num].price;
    }
    while (item_id != 0);

    cout << "Items purchased: " << total_items << endl
         << "Total cost: $" << fixed << setprecision(2) << total_price;
}
*/

void read_orders(const string& file) {
    ifstream inFS;
    string line1;
    string line2;
    vector<LineItem> line_items;
    int type;
    Credit* c;
    PayPal* p;
    WireTransfer* w;

    inFS.open(file);
    if (!inFS.is_open()) {
        cout << "Could not open file " << file << endl;
        return;
    }

    getline(inFS, line1);
    getline(inFS, line2);
    while (!inFS.fail()) {
        auto fields = split(line1, ',');
        auto payment = split(line2, ',');
        for (int i = 3; i < fields.size(); i ++) { // create vector of LineItems
            auto li_details = split(fields[i], '-');
            line_items.push_back(LineItem(stoi(li_details[0]), stoi(li_details[1])));
        }
        type = stoi(payment[0]);
        if (type == 1) {
            c = new Credit(0, payment[1], payment[2]);
            orders.push_back(Order(stoi(fields[1]), fields[2], stoi(fields[0]), line_items, c));
        } else
        if (type == 2) {
            p = new PayPal(0, payment[1]);
            orders.push_back(Order(stoi(fields[1]), fields[2], stoi(fields[0]), line_items, p));
        } else
        if (type == 3) {
            w = new WireTransfer(0, payment[1], payment[2]);
            orders.push_back(Order(stoi(fields[1]), fields[2], stoi(fields[0]), line_items, w));
        }

        line_items.clear();
        getline(inFS, line1);
        getline(inFS, line2);
    }
    if (!inFS.eof()) {
        cout << "Input failure before reaching end of file." << endl;
    }
}


int main() {
    read_customers("customers.txt");
    read_items("items.txt");
    read_orders("orders.txt");

    ofstream ofs("order_report.txt");
    for (auto& order : orders) {
        ofs << order.print_order() << endl;
    }
    ofs.close();
}