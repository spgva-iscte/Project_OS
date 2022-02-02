# Second Part
This part is organized in 2 classes: Cliente.c (client), Servidor.c (sever).

## Consulta.h
We create a new sturcuture called Consulta (appointment). This structured requires an int which is the represent the types of the appointment, a vector of char that is a description of the appointment and an int which is the PID of the appointment. 

## Cliente.c

### Criar_Consulta (create_appointment)
Asks the client the information needed to book an appointment. The PID is given by the function getpid(). After it will run the funtion pedir_consulta.

### Pedir_Consulta (book_appointment)
If the file "PedidoConsulta.txt" exists it will wait 10 seconds, if it does not exist it will create it and write the information about that appointment there. 

### Ler_PID (read_PID)
It reads the PID of the server which is written in the file "SrvConsultas.pid" and send a sign SIGURS1 to the server.

### Handler_SIGHUP
If it receives SIGHUP sign, the client will receive a message saying that the appointment has started and the file "PedidoConsulta.txt" will be deleted. The s variable is meant to see if the client received SIGHUP before receiving a SIGTERM.

### Handler_SIGTERM
If the client received a SIGTERM sign after having received a SIGHUP sign, it will terminate the appointment with success.

### Handler_SIGUSR2
If the client receives a SIGUSR2 sign, the file "PedidoConsulta.txt" will be deleted and let the client know that the appointment he had tried to book is not possible.

### Handler_SIGINT
If it receives a SIGNINT sign, it will let the client know that they canceled the appointment, delete the "PedidoConsulta.txt" and terminate the program.

## Servidor.c

### iniciar_lista_consultas.c
This function will put every appointment with the type '-1'. When the type is '-1' we consider the appointment does not exist. It is the same as if the space occupied by that appointment was free. It will put the different counters from each type of appointment at 0 and right the PID of the server at the file "SrvConsultas.pid".

### handler_SIGUSR1
When it receives a sign SIGUSR1 it will read the information written in "PedidoConsulta.txt" and puts that info in a new object of the type Consulta. Then it will check if there is any appointment in the list which the type is -1. If there isn't it will send a SIGUSR2 to the client and increases the counter of lost appointments. If there is it will book an appointment in the first empty room.

