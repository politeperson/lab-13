#include "client.h"

int main(){
	My_Client hola(500);//pongo como maximo numero de caracteres 500
	//*********************************
	//recuerda el orden para crear un cliente
	//iniciar la libreria
	//conectar el puerto
	//crear el socket
	//recibir mensaje y reenviar
	//apagar el cliente
	//*********************************
	hola.init_Components();
	hola.connect_port();
	hola.create_socket();
	hola.connect_server();
	hola.send_and_receive_message();
   	hola.shutdown_client();


	return 0;
}