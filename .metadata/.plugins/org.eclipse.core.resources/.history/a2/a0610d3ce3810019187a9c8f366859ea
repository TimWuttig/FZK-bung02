simple Txc
{
    parameters:
        bool sendMsgOnInit = default(false);
        int limit = default(2);
        volatile double delay @unit(s);
        @display("i=block/routing");
    gates:
        input in[];
        output out[];
}

simple Tic extends Txc {
    parameters:
        sendMsgOnInit = true;
        @display("i=,cyan");
}
    
simple Toc extends Txc {
    parameters:
        sendMsgOnInit = false;
        @display("i=,gold");
}    

network Tictoc
{
    @display("bgb=400,415");
    submodules:
        tic[6]: Tic;
	connections:
	    tic[0].out++ --> {  delay = 100ms; } --> tic[1].in++;
	    tic[0].in++ <-- {  delay = 100ms; } <-- tic[1].out++;
	
	    tic[1].out++ --> {  delay = 100ms; } --> tic[2].in++;
	    tic[1].in++ <-- {  delay = 100ms; } <-- tic[2].out++;
	
	    tic[1].out++ --> {  delay = 100ms; } --> tic[4].in++;
	    tic[1].in++ <-- {  delay = 100ms; } <-- tic[4].out++;
	
	    tic[3].out++ --> {  delay = 100ms; } --> tic[4].in++;
	    tic[3].in++ <-- {  delay = 100ms; } <-- tic[4].out++;
	
	    tic[4].out++ --> {  delay = 100ms; } --> tic[5].in++;
	    tic[4].in++ <-- {  delay = 100ms; } <-- tic[5].out++;
}
