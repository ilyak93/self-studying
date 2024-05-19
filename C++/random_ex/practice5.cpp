#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

class Message {
public:
    Message() {msg = "";}
    Message(string text) : msg(text.data()),  id(snum++){}
    const string& get_text() {
        return msg;
    }
    bool operator<(const Message& other) const{
        return this->id - other.id  < 0 ? true : false;
    }
private:
    static int snum;
    string msg;
    int id;
};

int Message::snum = 0;

class MessageFactory {
public:
    MessageFactory() {}
    Message create_message(const string& text) {
        return Message(text);
    }
};

class Recipient {
public:
    Recipient() {}
    void receive(const Message& msg) {
        messages_.push_back(msg);
    }
    void print_messages() {
        fix_order();
        for (auto& msg : messages_) {
            cout << msg.get_text() << endl;
        }
        messages_.clear();
    }
private:
    void fix_order() {
        sort(messages_.begin(), messages_.end());
    }
    vector<Message> messages_;
};

class Network {
public:
    static void send_messages(vector<Message> messages, Recipient& recipient) {
        // simulates the unpredictable network, where sent messages might arrive in unspecified order
        random_shuffle(messages.begin(), messages.end());
        for (auto msg : messages) {
            recipient.receive(msg);
        }
    }
};



int main() {
    MessageFactory message_factory;
    Recipient recipient;
    vector<Message> messages;
    string text;
    int i = 0;
    while (getline(cin, text)) {
        messages.push_back(message_factory.create_message(text));
        i++;
        if(i >= 4) break;
    }
    Network::send_messages(messages, recipient);
    recipient.print_messages();
}
