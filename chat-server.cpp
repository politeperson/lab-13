#include "server.h"

int main(){
	My_Server hola(500);//pongo como maximo numero de caracteres 500
	//*********************************
	//recuerda el orden para crear un server
	//iniciar la libreria
	//conectar el puerto
	//crear el socket
	//aceptar al cliente
	//recibir mensaje y reenviar
	//apagar el server
	//limpiar el server
	//*********************************
	hola.init_Components();
	hola.connect_port();
	hola.create_socket();
	hola.listen_socket();
	hola.accept_client();
	while(hola.receive_and_echo_message()){}
   	hola.shutdown_server();
    hola.cleanup_server();


	return 0;
}