#include "assignment2.h"

double calcAverageWaitingTime(struct Simulation * S)
{
	double totalarrivaltime = 0, totaldeparturetime = 0;
	int size = S->eventQueue.currSize;
	struct Node * holder = S->eventQueue.front;
	
	while (holder != NULL)
	{
		totalarrivaltime += holder->data.arrivalTime;
		totaldeparturetime += holder->data.departureTime;
		holder = holder->next;		
	}	
	freeQueue(&(S->eventQueue));
	freeQueue(&(S->buffer));
	return (totaldeparturetime - totalarrivaltime)/size;
			
}

struct Simulation initSimulation(double arrivalRate, double serviceTime, double simTime)
{
	struct Simulation S;
	S.currTime = 0;
	S.arrivalRate = arrivalRate;
	S.serviceTime = serviceTime;
	S.timeForNextArrival = getRandTime(arrivalRate);
	S.timeForNextDeparture = S.timeForNextArrival + serviceTime;
	S.totalSimTime = simTime;
	S.buffer = initQueue();
	S.eventQueue = initQueue();
	S.e = ARRIVAL;
	return S;
}

double runSimulation(double arrivalRate, double serviceTime, double simTime)
{
	struct Simulation sim = initSimulation(arrivalRate, serviceTime, simTime);
	struct Data d;
 
	while (sim.currTime < sim.totalSimTime)
	{		
		if (sim.timeForNextArrival < sim.timeForNextDeparture)
		{
			sim.e = ARRIVAL;
		}
		else
		{
			sim.e = DEPARTURE;
		}

		if (sim.e == ARRIVAL)
		{	
			sim.currTime = sim.timeForNextArrival;
			d.arrivalTime = sim.timeForNextArrival;
			enqueue(&(sim.buffer), d);
			sim.timeForNextArrival += getRandTime(arrivalRate); //updates time for next arrival
		}
		
		else if (sim.e == DEPARTURE)
		{
			sim.currTime = sim.timeForNextDeparture;
			sim.buffer.front->data.departureTime = sim.timeForNextDeparture;	
			d = dequeue(&(sim.buffer));
			enqueue(&(sim.eventQueue), d);
			
			if (sim.buffer.currSize == 0)
			{
				sim.timeForNextDeparture = sim.timeForNextArrival + serviceTime;
			}
			else
			{
				sim.timeForNextDeparture += serviceTime;
			}
			//printf("Arrival Time: %f\n", sim.eventQueue.rear->data.arrivalTime);
			//printf("Departure Time: %f\n", sim.eventQueue.rear->data.departureTime);
		}	
	}
	return calcAverageWaitingTime(&sim);
}


