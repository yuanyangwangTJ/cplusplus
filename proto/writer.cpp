#include <iostream>
#include <fstream>
#include <string>
#include "addressbook.pb.h"

using namespace std;

// 这个函数填充一个Person消息
void PromptForAddress(tutorial::Person* person) {
    cout << "Enter person ID number: ";
    int id;
    cin >> id;
    person->set_id(id);
    cin.ignore(256, '\n');

    cout << "Enter name: ";
    getline(cin, *person->mutable_name());

    cout << "Enter email address (blank for none): ";
    string email;
    getline(cin, email);
    if (!email.empty()) {
        person->set_email(email);
    }

    while (true) {
        cout << "Enter a phone number (or leave blank to finish): ";
        string number;
        getline(cin, number);
        if (number.empty()) {
            break;
        }

        tutorial::Person::PhoneNumber* phone_number = person->add_phones();
        phone_number->set_number(number);

        cout << "Is this a mobile, home, or work phone? ";
        string type;
        getline(cin, type);
        if (type == "mobile") {
            phone_number->set_type(tutorial::Person::MOBILE);
        } else if (type == "home") {
            phone_number->set_type(tutorial::Person::HOME);
        } else if (type == "work") {
            phone_number->set_type(tutorial::Person::WORK);
        } else {
            cout << "Unknown phone type. Using default." << endl;
        }
    }
}

int main() {
    // 验证protobuf版本
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    tutorial::AddressBook address_book;

    // 添加一个联系人
    PromptForAddress(address_book.add_people());

    // 将地址簿写入文件
    fstream output("address_book.data", ios::out | ios::binary);
    if (!address_book.SerializeToOstream(&output)) {
        cerr << "Failed to write address book." << endl;
        return -1;
    }

    // 清理protobuf
    google::protobuf::ShutdownProtobufLibrary();

    return 0;
} 