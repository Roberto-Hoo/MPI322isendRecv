/*
 * 3.2.2 Envio e recebimento assíncrono
   O MPI também suporta rotinas MPI_Isend de envio e MPI_Irecv de recebimento assíncronos.
   Neste caso, o processo emissor envia o dado para outro processo e segue imediatamente a computação.
   O processo receptor deve conter uma rotina MPI_Irecv, mas também não aguarda sua conclusão para
   seguir a computação.

As sintaxes destas rotinas são semelhantes as das rotinas MPI_Send e MPI_Recv.

int MPI_Isend(
  const void *buf,
  int count,
  MPI_Datatype datatype,
  int dest,
  int tag, MPI_Comm comm,
  MPI_Request *request)
int MPI_Irecv(
  void *buf,
  int count,
  MPI_Datatype datatype,
  int source,
  int tag,
  MPI_Comm comm,
  MPI_Request *request)
O último argumento permite verificar os envios e recebimentos.

Vamos estudar o seguinte código.

Código: isendRecv.cc
*/
//******************************************************************************

#include <stdio.h>
// API MPI
#include <mpi.h>

int world_size; // numero total de processos
int world_rank; // ID (rank) do processo
int errorcode; // Numero do erro

MPI_Status status;
MPI_Request request;


int main() {
    // Inicializa o MPI
    MPI_Init(NULL, NULL);

    // numero total de processos
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_size < 2) {
        printf("Num. de processos deve ser maior que 1.\n");
        errorcode = -10;
        MPI_Abort(MPI_COMM_WORLD, errorcode);
    }

    // ID (rank) do processo
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // MPI_Status & MPI_Request

    if (world_rank == 0) {
        double x = 3.1416;
        MPI_Isend(&x, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &request);
    } else {
        double y = 0.0;
        MPI_Irecv(&y, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &request);
        double x = y + 1.0;
        printf("No processo %d, x = %f\n",world_rank, x);
        int recvd = 0;
        while (!recvd)
            MPI_Test(&request, &recvd, &status);
        x = y + 1;
        printf("No processo %d, x = %f , recvd = %d\n",world_rank, x,recvd);
    }

    // Finaliza o MPI
    MPI_Finalize();

    return 0;
}
    /*
     *  Rode com mpic++ E311.cpp -o E311
     *  depois mpirun -np 4 E311
     *  mpirun -np 8 -use-hwthread-cpus E311
     *  mpirun -np 9 -oversubscribe E311
     */