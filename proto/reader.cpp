#include <iostream>
#include <fstream>
#include <string>
#include "addressbook.pb.h"

using namespace std;

// 迭代打印地址簿中的所有联系人
void ListPeople(const tutorial::AddressBook& address_book) {
    for (int i = 0; i < address_book.people_size(); i++) {
        const tutorial::Person& person = address_book.people(i);

        cout << "Person ID: " << person.id() << endl;
        cout << "  Name: " << person.name() << endl;
        if (!person.email().empty()) {
            cout << "  E-mail address: " << person.email() << endl;
        }

        for (int j = 0; j < person.phones_size(); j++) {
            const tutorial::Person::PhoneNumber& phone_number = person.phones(j);

            switch (phone_number.type()) {
                case tutorial::Person::MOBILE:
                    cout << "  Mobile phone #: ";
                    break;
                case tutorial::Person::HOME:
                    cout << "  Home phone #: ";
                    break;
                case tutorial::Person::WORK:
                    cout << "  Work phone #: ";
                    break;
            }
            cout << phone_number.number() << endl;
        }
    }
}

int main() {
    // 验证protobuf版本
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    tutorial::AddressBook address_book;

    // 从文件读取地址簿
    fstream input("address_book.data", ios::in | ios::binary);
    if (!address_book.ParseFromIstream(&input)) {
        cerr << "Failed to parse address book." << endl;
        return -1;
    }

    ListPeople(address_book);

    // 清理protobuf
    google::protobuf::ShutdownProtobufLibrary();

    return 0;
} 