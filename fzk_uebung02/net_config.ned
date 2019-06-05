simple Node
{
    parameters:
		bool master = default(false);
	gates:
	    inout gate;
}

simple StampNode
{
	gates:
	    inout innergate;
	    inout outergate;
}

module NodeCmp
{
	gates:
	    inout gate[];
	submodules:
	    layer2: Node;
	    layer1: StampNode;
	connections:
	    layer1.innergate <--> layer2.gate;
}

network MasterSlave{
    types:
        channel Channel extends ned.DelayChannel{
            delay = 100ms;
        }
	submodules:
	    node[3]: Node;
	connections:
	    node[0].gate++ <--> Channel <--> node[1].gate++;
	    node[0].gate++ <--> Channel <--> node[2].gate++;
	    node[1].gate++ <--> Channel <--> node[2].gate++;
}

