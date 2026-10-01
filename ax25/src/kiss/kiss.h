#include "kissConfig.h"
#include <thread>

class KissClient 
{
public:
    KissClient(const KissConfig& config) : m_config(config) {}
    ~KissClient() = default;

    void connect();
    void disconnect();

private:
    KissConfig m_config;
    std::thread m_connectionThread;
};

class KissImplementation
{
public:
    virtual ~KissImplementation() = default;
    virtual void connect() = 0;
    virtual void disconnect() = 0;
};

class KissTcp: public KissImplementation {
public:
    KissTcp(int port);
    ~KissTcp();

    void connect() override;
    void disconnect() override;

private:
    
}