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


