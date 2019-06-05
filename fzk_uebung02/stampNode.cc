#include <omnetpp.h>
#include <string.h>
#include <prior_message_m.h>

using namespace omnetpp;

class StampNode : public cSimpleModule {
    private:


    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual void finish() override;
};

Define_Module(StampNode);

void StampNode::handleMessage(cMessage *msg) {
    msg->setTimestamp();
    send(msg->dup(), "gate$o");
}
