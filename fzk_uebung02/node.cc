#include <omnetpp.h>
#include <string.h>
#include <prior_message_m.h>

using namespace omnetpp;

class Node : public cSimpleModule {
    private:
        bool master;
        int prior_num[4] = {};
        cMessage *mastertimer;
        cMessage *delaytimer;
        PriorMessage *tmp;
        int master_id;
        cOutVector delayVector;

    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual void masterBehavior(cMessage *msg);
        virtual void slaveBehavior(cMessage *msg);
        virtual void finish() override;
};

Define_Module(Node);

void Node::initialize(){
    master = getParentModule()->par("master");
    mastertimer = new cMessage("timer");
    delaytimer = new cMessage("timer");

    if(master){
        scheduleAt(0.0, mastertimer);
    }
}

void Node::handleMessage(cMessage *msg) {
    if(master){
        masterBehavior(msg);
    }else{
        slaveBehavior(msg);
    }
}

void Node::masterBehavior(cMessage *msg){
    if(msg == mastertimer){
        int n = getParentModule()->gateSize("gate");
        for(int i = 0; i < n; i++){
            int prior = intuniform(0, 3);
            prior_num[prior]++;
            PriorMessage *myMsg = new PriorMessage("MasterMsg");
            myMsg->setPriority(prior);
            send(myMsg, "gate$o");
            EV << "MASTER: Sended message with prio: " << prior << " to slave" << (i+1) << "\n";
        }

        scheduleAt(simTime()+ 0.08, mastertimer);
    }else {
        delayVector.record(simTime() - msg->getTimestamp());
    }
}

void Node::slaveBehavior(cMessage *msg){
    if(msg == delaytimer){
        cMessage *answer = tmp->dup();
        send(answer, "gate$o");
    }else {
        tmp = (PriorMessage*) msg;

        int prior = tmp->getPriority();
        simtime_t priorDelay = uniform(0, prior * 0.01);
        simtime_t processingDelay = normal(0.0002, 0.0001);
        simtime_t delay = priorDelay + processingDelay;
        if(delay.dbl() < 0) delay = 0.0;
        scheduleAt(simTime() + delay, delaytimer);
    }
}

void Node::finish(){
    if(master){
        recordScalar("Priority 0", prior_num[0]);
        recordScalar("Priority 1", prior_num[1]);
        recordScalar("Priority 2", prior_num[2]);
        recordScalar("Priority 3", prior_num[3]);
    }
}
