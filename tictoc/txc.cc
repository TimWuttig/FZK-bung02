#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class  Txc : public cSimpleModule
{
    private:
        int counter;
        cMessage* event;
        cMessage* tictocMsg;
        cMessage* timeout;
        simtime_t tout;


    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual void forwardMsg(cMessage *msg);
};

Define_Module(Txc);

void Txc::initialize()
{
    event = new cMessage("event");
    timeout = new cMessage("timeout");

    if(par("sendMsgOnInit").boolValue() == true && getIndex()== 0){
        tictocMsg = new cMessage("tictocMsg");
        EV << "Sending initial message\n";
        send(tictocMsg, "out", 0);
        scheduleAt(simTime()+tout, timeout);
    }
    counter = par("limit");
    WATCH(counter);

    tout = 30.0;
}

void Txc::handleMessage(cMessage *msg){
    if(getIndex() == 3){
        if(msg == event){
                EV << "Received message " << tictocMsg->getName() << ", sending it out again\n";
                send(tictocMsg, "out");
                counter--;
                scheduleAt(simTime() + tout, timeout);
            }else {
                if(msg == timeout) {
                    send(new cMessage("tictocMsg"), "out", 1);
                    scheduleAt(simTime() + tout, timeout);
                }else{
                    cancelEvent(timeout);
                    if(uniform(0, 1) < 0.8){
                        EV << "Losing Message";
                        delete msg;
                        bubble("Lost the message");
                    }else{
                        EV << "Sending delay signal";
                        tictocMsg = msg;
                        simtime_t delay = par("delay");
                        scheduleAt(simTime()+delay, event);
                    }
                }
            }
    }else{
        forwardMsg(msg);
    }
}

void Txc::forwardMsg(cMessage *msg){
    int k = intuniform(0, gateSize("out") - 1);
    cMessage *dub = msg->dup();
    send(dub, "out", k);
}
