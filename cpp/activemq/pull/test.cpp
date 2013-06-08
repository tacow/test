#include <activemq/library/ActiveMQCPP.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <iostream>

using namespace activemq::core;
using namespace activemq::library;
using namespace cms;
using namespace std;

int main(int argc, char* argv[]) {
    try {
        ActiveMQCPP::initializeLibrary();
        ActiveMQConnectionFactory* connectionFactory = new ActiveMQConnectionFactory("tcp://127.0.0.1:61616", "admin", "admin");
        Connection* connection = connectionFactory->createConnection();
        connection->start();
        Session* session = connection->createSession(Session::AUTO_ACKNOWLEDGE);
        Destination* destination = session->createQueue("TEST1");
        MessageConsumer* consumer = session->createConsumer(destination);

        for(int i = 0; i < 100; ++i){
            TextMessage* message = (TextMessage*)consumer->receive();
            string text = message->getText();
            cout << text << endl;
            delete message;
        }

        delete destination;
        delete consumer;
        session->close();
        delete session;
        connection->close();
        delete connection;
        delete connectionFactory;
        ActiveMQCPP::shutdownLibrary();
    }
    catch (CMSException& e) {
        e.printStackTrace();
    }
}

