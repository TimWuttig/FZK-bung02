#include <omnetpp.h>
#include <string.h>
#include <prior_message_m.h>

using namespace omnetpp;

class StampNode : public cSimpleModule {
    private:
        int master_id;
        int slave_id;
        bool master;
    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual void masterBehavior(cMessage *msg);
        virtual void slaveBehavior(cMessage *msg);
};

Define_Module(StampNode);

void StampNode::initialize() {
    master_id = 0;
    slave_id = 0;
    master = getParentModule()->par("master");
}

void StampNode::handleMessage(cMessage *msg) {
    if(master){
        masterBehavior(msg);
    }else{
        slaveBehavior(msg);
    }
}

void StampNode::slaveBehavior(cMessage *msg){
    const char* gate = msg->getArrivalGate()->getBaseName();
    if(strcmp(gate, "innergate") == 0){
        if(msg->getTimestamp() == 0.0)
            msg->setTimestamp();

        send(msg, "outergate$o", master_id);
    }else {
        std::string gatename = msg->getArrivalGate()->getFullName();
        unsigned start = gatename.find("[");
        unsigned end = gatename.find("]");

        master_id = std::stoi(gatename.substr(start + 1, end - start));
        send(msg, "innergate$o");
    }
}

void StampNode::masterBehavior(cMessage *msg){
    const char* gate = msg->getArrivalGate()->getBaseName();
        if(strcmp(gate, "innergate") == 0){
            if(msg->getTimestamp() == 0.0)
                msg->setTimestamp();

            send(msg, "outergate$o", slave_id);
            slave_id = (slave_id + 1) % gateSize("outergate");
        }else{
            send(msg, "innergate$o");
        }
}
