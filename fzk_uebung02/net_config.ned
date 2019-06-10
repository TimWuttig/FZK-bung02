simple Node
{
    gates:
        inout gate;
}

simple StampNode
{
    gates:
        inout innergate;
        inout outergate[];
}

module NodeCmp
{
    parameters:
        bool master = default(false);
    gates:
        inout gate[];
    submodules:
        layer2: Node;
        layer1: StampNode;
    connections:
        layer1.innergate <--> layer2.gate;
        for i=0..sizeof(gate)-1{
        	layer1.outergate++ <--> gate++;
        }
}

network MasterSlave
{
    types:
        channel Channel extends ned.DelayChannel
        {
            delay = 100ms;
        }
    submodules:
        nodeCmp[3]: NodeCmp;
    connections:
        nodeCmp[0].gate++ <--> Channel <--> nodeCmp[1].gate++;
        nodeCmp[0].gate++ <--> Channel <--> nodeCmp[2].gate++;
        nodeCmp[1].gate++ <--> Channel <--> nodeCmp[2].gate++;
}

