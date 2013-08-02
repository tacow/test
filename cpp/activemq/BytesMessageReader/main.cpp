#include <activemq/library/ActiveMQCPP.h>
#include <activemq/core/ActiveMQConnectionFactory.h>

using namespace activemq::core;
using namespace activemq::library;
using namespace cms;

int main(int argc, char* argv[]) {
    if (argc < 4) {
        printf("%s [host:port] [queuename] [filename]\n", argv[0]);
        return 0;
    }
    const char* amqHost = argv[1];
    const char* queueName = argv[2];
    const char* filename = argv[3];

    try {
        ActiveMQCPP::initializeLibrary();
        char connStr[1024];
        snprintf(connStr, 1024, "tcp://%s", amqHost);
        ActiveMQConnectionFactory* connectionFactory = new ActiveMQConnectionFactory(connStr);
        Connection* connection = connectionFactory->createConnection();
        connection->start();
        Session* session = connection->createSession(Session::AUTO_ACKNOWLEDGE);
        Destination* destination = session->createQueue(queueName);
        MessageConsumer* consumer = session->createConsumer(destination);

        Message* message = consumer->receive();
        BytesMessage* bytesMessage = dynamic_cast<BytesMessage*>(message);
        if (bytesMessage) {
            FILE* f = fopen(filename, "w");
            if (f) {
                fwrite(bytesMessage->getBodyBytes(), 1, bytesMessage->getBodyLength(), f);
                fclose(f);
            } else {
                printf("Can't open file\n");
            }
        }
        delete message;

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
    return 0;
}

