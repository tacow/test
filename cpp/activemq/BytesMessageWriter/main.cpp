#include <activemq/library/ActiveMQCPP.h>
#include <activemq/core/ActiveMQConnectionFactory.h>

using namespace activemq::core;
using namespace activemq::library;
using namespace cms;

#define BUF_LEN (10 * 1024 * 1024)

int main(int argc, char* argv[]) {
    if (argc < 4) {
        printf("%s [host:port] [queuename] [filename]\n", argv[0]);
        return 0;
    }
    const char* amqHost = argv[1];
    const char* queueName = argv[2];
    const char* filename = argv[3];

    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        printf("Can't open file\n");
        return 0;
    }
    unsigned char* buf = new unsigned char[BUF_LEN];
    size_t len = fread(buf, 1, BUF_LEN, f);
    fclose(f);

    try {
        ActiveMQCPP::initializeLibrary();
        char connStr[1024];
        snprintf(connStr, 1024, "tcp://%s", amqHost);
        ActiveMQConnectionFactory* connectionFactory = new ActiveMQConnectionFactory(connStr);
        Connection* connection = connectionFactory->createConnection();
        connection->start();
        Session* session = connection->createSession(Session::AUTO_ACKNOWLEDGE);
        Destination* destination = session->createQueue(queueName);
        MessageProducer* producer = session->createProducer(destination);
        producer->setDeliveryMode(DeliveryMode::NON_PERSISTENT);

        BytesMessage* message = session->createBytesMessage(buf, len);
        producer->send(message);
        delete message;

        delete destination;
        delete producer;
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

    delete[] buf;
    return 0;
}

