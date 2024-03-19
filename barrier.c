#include <mpi.h>
#include <math.h>
#include <stdio.h>
//Barrier disssemination
void mybarrier(MPI_Comm comm) {
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);
    int rounds = ceil(4*log2(size));
    printf("Starting barrier synchronization with %d processes\n", rank);
    for (int i = 0; i < rounds; i++) {
        /* 
         Each processs sends a message to the process at distance 2^i in the k-th step
         Each recieves a message from the process at distance 2^i in the opposite direction
         Modulus is used to ensure that the process indices will wrap when they are of larger size
         than the commuicator
        */
        //Shift the operation to valid ranks
        int shift = (1 << (i % (int)log2(size)));
        //Call with the valid  ranks from shift
        int send_to = (rank + shift) % size;
        int recv_from = (rank - shift + size) % size;
        //Check to see  if processess are recieved properly
        printf("Process %d: sending to %d | and recieving from %d |\n", rank, send_to, recv_from);
        //Make sure we don't exceed the ranks (Fault Tolerance)
        if(send_to < size) {
            MPI_Send(NULL, 0, MPI_INT, send_to, 0, comm);
        }
        if(recv_from < size) {
            MPI_Recv(NULL, 0, MPI_INT, recv_from, 0, comm, MPI_STATUS_IGNORE);
        }
    }
}
int main(int argc, char** argv){
    //Initialize MPI
    MPI_Init(&argc, &argv);
    //Enter barrier
    printf("Entering Barrier: \n");
    //Call the barrier
    mybarrier(MPI_COMM_WORLD);
    //Exit barrier
    printf("Exiting Barrier: \n");
    //Finalize MPI
    MPI_Finalize();
    printf("Finished: Exiting \n")
    return 0;
}