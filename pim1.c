#ifdef __WIN32
#define limpar_tela "cls"
#else
#define limpar_tela "clear"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100
 

typedef struct pedido
{
	
	int id;
	int id_cliente;
	char nome[MAX];
	float preco;
} t_pedido;
 
 

typedef struct cliente
{

	int id;
	char nome[MAX];
} t_cliente;
 
 

 

char menu();
 

void cadastrar_pedido();


void listar_pedidos();
 

t_pedido *obter_pedido(FILE *arq_pedidos, int id_pedido);
 

t_cliente *obter_cliente(FILE *arq_clientes, int id_cliente);
 

void pesquisar_pedido();
 

int existe_pedido(FILE *arq_pedidos, int id_pedido);
 

int existe_cliente(FILE *arq_clientes, int id_cliente);
 

void atualizar_pedidos(FILE *arq_pedidos, t_pedido *pedido_alugado);
 

void fazer_pedido();
 

void entregar_pedido();
 

void excluir_pedido();
 

 
int str_somente_numeros(char str[]);
 
 
int main(int argc, char *argv[])
{
	char resp;
 
	
	while(1)
	{
		
		resp = menu();
 
		
		if(resp == '1')
			cadastrar_pedido();
		else if(resp == '2')
			listar_pedidos();
		else if(resp == '3')
			pesquisar_pedido();
		else if(resp == '4')
			fazer_pedido();
		else if(resp == '5')
			entregar_pedido();
		else if(resp == '6')
			excluir_pedido();
		else if(resp == '0') 
			break;
		else
		{
			printf("\nOpcao invalida! Pressione <Enter> para continuar...");
			scanf("%*c");
			
			fseek(stdin, 0, SEEK_END); 
		}
		system(limpar_tela);
	}
 
	printf("\nTchau!;-)\n");
	return 0;
}
 
 

char menu()
{
	char resp[2];
 
	printf("\n\n1 - Cadastrar um novo prato ao menu\n");
	printf("2 - Menu de pratos\n");
	printf("3 - Pesquisar por prato\n");
	printf("4 - Solicitar um prato\n");
	printf("5 - Entregar um pedido\n");
	printf("6 - Excluir prato\n");
	printf("0 - Voltar pro menu inicial\n");
	printf("Digite o numero da opcao: ");
	scanf("%1s%*c", resp); 
 
	fseek(stdin, 0, SEEK_END); 
 

	return resp[0];
}
 
 
int str_somente_numeros(char str[])
{
	int i = 0;
	int len_str = strlen(str);
 
	for(i = 0; i < len_str; i++)
	{
		if(str[i] < '0' || str[i] > '9')
			return 0;
	}
	return 1;
}
 
 



void cadastrar_pedido()
{
	
	FILE *arq_pedidos = fopen("pedidos.bin", "a+b");
 
	
	if(arq_pedidos == NULL)
	{
		printf("\nFalha ao abrir arquivo(s)!\n");
		exit(1); 
	}
	t_pedido pedido;
 

	int cont_bytes = 0;
 
	
	fseek(arq_pedidos, 0, SEEK_END);
	
	cont_bytes = ftell(arq_pedidos);
 
	if(cont_bytes == 0)
	{
	
		pedido.id = 1;
	}
	else
	{
		t_pedido ultimo_pedido;
 
	
		fseek(arq_pedidos, cont_bytes - sizeof(t_pedido), SEEK_SET);
 
		
		fread(&ultimo_pedido, sizeof(t_pedido), 1, arq_pedidos);
 
		
		pedido.id = ultimo_pedido.id + 1;
	}

	printf("\nDigite o nome do prato: ");
	scanf("%99[^\n]%*c", pedido.nome);
 
	
	fseek(stdin, 0, SEEK_END); 
 
	do
	{
		char str_preco[5];
		float float_preco;
		int somente_numeros = 1;
 
		
		printf("Digite o preco do prato: ");
		scanf("%4s%*c", str_preco);
 
		fseek(stdin, 0, SEEK_END); 
 
		
		somente_numeros = str_somente_numeros(str_preco);
		
		if(somente_numeros == 1)
		{
		
		
			int int_preco;
			
			sscanf(str_preco, "%d", &int_preco);
			float_preco = int_preco / 100.0;
			pedido.preco = float_preco;
			break;
		}
	}
	while(1);
 
	
	pedido.id_cliente = -1;
 
	
	fseek(arq_pedidos, 0, SEEK_END);
	
	fwrite(&pedido, sizeof(t_pedido), 1, arq_pedidos);
 
	
	fclose(arq_pedidos);
 
	printf("\nPrato \"%s\" cadastrado com sucesso!\n", pedido.nome);
	printf("\nPressione <Enter> para continuar...");
	scanf("%*c"); 
 
	
	fseek(stdin, 0, SEEK_END); 
}
 
 

void listar_clientes()
{
	
	FILE *arq_clientes = fopen("clientes.bin", "rb");
 
	
	if(arq_clientes == NULL)
	{
		printf("\nFalha ao abrir arquivo(s) ou ");
		printf("Nenhum cliente cadastrado.\n");
		printf("\nPressione <Enter> para continuar...");
		scanf("%*c"); 
 
		
		fseek(stdin, 0, SEEK_END); 
		return;
	}
 
	
	int encontrou_clientes = 0;
	t_cliente cliente;
 
	printf("\nListando todos os clientes...\n");
	
	while(1)
	{
		size_t result = fread(&cliente, sizeof(t_cliente), 1, arq_clientes);
 
		
		if(result == 0)
			break;
 
	
		encontrou_clientes = 1;
 
		
		printf("\nID do cliente: %d\n", cliente.id);
		printf("Nome do cliente: %s\n", cliente.nome);
	}
 
	if(encontrou_clientes == 0)
		printf("\nNenhum cliente cadastrado.\n");
 
	fclose(arq_clientes);
 
	printf("\nPressione <Enter> para continuar...");
	scanf("%*c");
 
	
	fseek(stdin, 0, SEEK_END); 
}
 
 

t_cliente *obter_cliente(FILE *arq_clientes, int id_cliente)
{
	
	rewind(arq_clientes);
 
	t_cliente *cliente;
 

	cliente = (t_cliente *)malloc(sizeof(t_cliente));
	while(1)
	{
 
		
		size_t result = fread(cliente, sizeof(t_cliente), 1, arq_clientes);
 
		
		if(result == 0)
		{
			free(cliente); 
			return NULL;
		}
		
		if(cliente->id == id_cliente)
			break;
	}
	return cliente;
}
 
 

void listar_pedidos()
{
	
 
	
	FILE *arq_pedidos = fopen("pedidos.bin", "rb");
	FILE *arq_clientes = fopen("clientes.bin", "rb");
 
	
	if(arq_pedidos == NULL)
	{
		printf("\nFalha ao abrir arquivo ou ");
		printf("Nenhum pedido cadastrado.\n");
		printf("\nPressione <Enter> para continuar...");
		scanf("%*c"); 
 
		
		fseek(stdin, 0, SEEK_END); 
		return;
	}
	
	int encontrou_pedidos = 0;
	printf("\nListando todos os pratos...\n");
	
	t_pedido pedido;
	while(1)
	{
		
		size_t result = fread(&pedido, sizeof(t_pedido), 1, arq_pedidos);
 
		
		if(result == 0)
			break;
 
	
		encontrou_pedidos = 1;
 
		
		printf("\nID do prato: %d\n", pedido.id);
		printf("Nome do prato: %s\n", pedido.nome);
		printf("Preco: %.2f\n", pedido.preco);
 
	
		if(pedido.id_cliente != -1)
		{
		
			if(arq_clientes == NULL)
			{
				printf("\nFalha ao abrir arquivo!\n");
				fclose(arq_pedidos); 
				exit(1); 
			}
			t_cliente *cliente = obter_cliente(arq_clientes, pedido.id_cliente);
			printf("Ja foi pedido antes? Sim. Cliente: %s\n", cliente->nome);
			free(cliente); 
		}
		else
			printf("Ja foi pedido? Nao\n");
	}
 
	
	if(encontrou_pedidos == 0)
		printf("\nNenhum pedido cadastrado.\n");
 
	
	if(arq_clientes != NULL)
		fclose(arq_clientes); 
	fclose(arq_pedidos);
 
	printf("\nPressione <Enter> para continuar...");
	scanf("%*c");
 
	
	fseek(stdin, 0, SEEK_END); 
}
 
 

void pesquisar_pedido()
{
	char nome[MAX];
	int encontrou_pedido = 0;
 
	
	FILE *arq_pedidos = fopen("pedidos.bin", "rb");
	FILE *arq_clientes = fopen("clientes.bin", "rb");
 
	
	if(arq_pedidos == NULL)
	{
		printf("\nFalha ao abrir arquivo(s)!\n");
		exit(1); 
	}
 
	printf("\nDigite o nome do prato: ");
	scanf("%99[^\n]%*c", nome);
 
	printf("\npedidos com o nome \"%s\":\n\n", nome);
	t_pedido pedido;
	while(1)
	{
	
		size_t result = fread(&pedido, sizeof(t_pedido), 1, arq_pedidos);
 
	
		if(result == 0)
			break;
 
		char nome_aux[MAX];
	
		strcpy(nome_aux, pedido.nome);
 
		
		if(strcmp(strupr(nome_aux), strupr(nome)) == 0)
		{
			
			printf("ID do prato: %d\n", pedido.id);
			printf("Preco: %.2lf\n", pedido.preco);
			if(pedido.id_cliente != -1)
			{
				
				if(arq_clientes == NULL)
				{
					printf("\nFalha ao abrir arquivo!\n");
					fclose(arq_pedidos); 
					exit(1); 
				}
				t_cliente *cliente = obter_cliente(arq_clientes, pedido.id_cliente);
				printf("Comprado anteriormente? Sim. Cliente: %s\n", cliente->nome);
				free(cliente); 
			}
			else
				printf("Comprado anteriormente? Nao\n");
			encontrou_pedido = 1;
			printf("\n");
		}
	}
 
	if(encontrou_pedido == 0)
		printf("Nenhum pedido encontrado.\n\n");
 
	fclose(arq_pedidos);
 
	printf("Pressione <Enter> para continuar...");
	scanf("%*c");
 
	
	fseek(stdin, 0, SEEK_END); 
}
 
 

void pesquisar_cliente()
{
	char nome[MAX];
	int encontrou_cliente = 0;
 
	
	FILE *arq_clientes = fopen("clientes.bin", "rb");
 
	
	if(arq_clientes == NULL)
	{
		printf("\nFalha ao abrir arquivo(s)!\n");
		exit(1); 
	}
 
	printf("\nDigite o nome do cliente: ");
	scanf("%99[^\n]%*c", nome);
 
	printf("\nClientes com o nome \"%s\":\n\n", nome);
	
	t_cliente cliente;
	while(1)
	{
		
		size_t result = fread(&cliente, sizeof(t_cliente), 1, arq_clientes);
 
		
		if(result == 0)
			break;
 
		char nome_aux[MAX];
	
		strcpy(nome_aux, cliente.nome);
 
		
		if(strcmp(strupr(nome_aux), strupr(nome)) == 0)
		{
			printf("ID do cliente: %d\n\n", cliente.id);
			encontrou_cliente = 1;
		}
	}
 
	if(encontrou_cliente == 0)
		printf("Nenhum cliente encontrado.\n\n");
 
	fclose(arq_clientes);
 
	printf("Pressione <Enter> para continuar...");
	scanf("%*c");
 
	fseek(stdin, 0, SEEK_END); 
}
int existe_pedido(FILE *arq_pedidos, int id_pedido)
{
	rewind(arq_pedidos);
 
	t_pedido pedido;

	while(1)
	{
 
		
		size_t result = fread(&pedido, sizeof(t_pedido), 1, arq_pedidos);
 
		
		if(result == 0)
			break;
 
		
		if(pedido.id == id_pedido)
			return 1;
	}
 
	
	return 0;
}
 
 

int existe_cliente(FILE *arq_clientes, int id_cliente)
{
	
	rewind(arq_clientes);
 
	t_cliente cliente;
	
	while(1)
	{
	
		size_t result = fread(&cliente, sizeof(t_cliente), 1, arq_clientes);
 
		
		if(result == 0)
			break;
 
		
		if(cliente.id == id_cliente)
			return 1;
	}
 
	
	return 0;
}
 
 

t_pedido *obter_pedido(FILE *arq_pedidos, int id_pedido)
{
	
	rewind(arq_pedidos);
 

	t_pedido *pedido;
 
	
	pedido = (t_pedido *)malloc(sizeof(t_pedido));
	while(1)
	{
 
		
		size_t result = fread(pedido, sizeof(t_pedido), 1, arq_pedidos);
 
		
		if(result == 0)
			break;
 
		
		if(pedido->id == id_pedido)
			return pedido;
	}
	free(pedido); 
	return NULL;
}
 
 

void atualizar_pedidos(FILE *arq_pedidos, t_pedido *pedido_alugado)
{
	
	rewind(arq_pedidos);
 
	t_pedido pedido;
	while(1) 
	{
 
		
		size_t result = fread(&pedido, sizeof(t_pedido), 1, arq_pedidos);
 
		
		if(result == 0)
			break;
 
		
		if(pedido.id == pedido_alugado->id)
		{
			
			fseek(arq_pedidos, - sizeof(t_pedido), SEEK_CUR);
			
			fwrite(pedido_alugado, sizeof(t_pedido), 1, arq_pedidos);
			break; 
		}
	}
}
 
 

void fazer_pedido()
{
	char str_id_cliente[10];
 
	
	FILE *arq_pedidos = fopen("pedidos.bin", "rb+");
	FILE *arq_clientes = fopen("clientes.bin", "rb+");
 

	if(arq_pedidos == NULL)
	{
		arq_pedidos = fopen("pedidos.bin", "wb+");
		if(arq_pedidos == NULL)
		{
			printf("\nFalha ao criar arquivo(s)!\n");
			exit(1); 
		}
	}
 
	if(arq_clientes == NULL)
	{
		arq_clientes = fopen("clientes.bin", "wb+");
		if(arq_clientes == NULL)
		{
			printf("\nFalha ao criar arquivo(s)!\n");
			exit(1); 
		}
	}
 
	
	//printf("\nDigite o ID do cliente: ");
	//scanf("%10s%*c", str_id_cliente);
 
	fseek(stdin, 0, SEEK_END); 
 
	if(str_somente_numeros(str_id_cliente) == 1)
	{

		{
			char str_id_pedido[10];
			int id_pedido;
 
			printf("\nDigite o ID do prato: ");
			scanf("%10s%*c", str_id_pedido);
 
			fseek(stdin, 0, SEEK_END); 
 
			if(str_somente_numeros(str_id_pedido) == 1)
			{
				
				sscanf(str_id_pedido, "%d", &id_pedido);
 
				
				t_pedido *pedido = obter_pedido(arq_pedidos, id_pedido);
 
				
				if(pedido != NULL)
				{
					
					if(pedido->id_cliente != -1)
						printf("\nO pedido \"%s\" Ja esta solicitado! Aguardando motorista parceiro...\n", pedido->nome);
					else
					{
				
						atualizar_pedidos(arq_pedidos, pedido); 
						printf("\nPedido \"%s\" solicitado com sucesso!\ Aguardando motorista parceiro...", pedido->nome);
					}
					free(pedido); 
				}
				else
					printf("\nNao existe pedido com o ID \"%d\".\n", id_pedido);
			}
			else
				printf("\nO ID so pode conter numeros!\n");
		}
	}
	else
		printf("\nO ID so pode conter numeros!\n");
 
	fclose(arq_pedidos);
 
	printf("\nPressione <Enter> para continuar...");
	scanf("%*c");
 
	fseek(stdin, 0, SEEK_END); 
}
 
 

void entregar_pedido()
{
	char str_id_pedido[10];
	int id_pedido;
 
	
	FILE *arq_pedidos = fopen("pedidos.bin", "rb+");
 

	if(arq_pedidos == NULL)
	{
		arq_pedidos = fopen("pedidos.bin", "wb+");
		if(arq_pedidos == NULL)
		{
			printf("\nFalha ao criar arquivo(s)!\n");
			exit(1); 
		}
	}
 
	printf("\nDigite o ID do prato: ");
	scanf("%10s%*c", str_id_pedido);
 
	fseek(stdin, 0, SEEK_END); 
 
	if(str_somente_numeros(str_id_pedido) == 1)
	{
		
		sscanf(str_id_pedido, "%d", &id_pedido);
 
		
		t_pedido *pedido = obter_pedido(arq_pedidos, id_pedido);
 
		
		if(pedido != NULL)
		{
		
			if(pedido->id_cliente == -1)
				printf("\nO pedido \"%s\" ja esta disponivel!\n", pedido->nome);
			else
			{
				atualizar_pedidos(arq_pedidos, pedido); 
				printf("\Pedido \"%s\" solicitado com sucesso!\n", pedido->nome);
			}
			free(pedido); 
		}
		else
			printf("\nNao existe pedido com o ID \"%d\".\n", id_pedido);
	}
	else
		printf("\nO ID so pode conter numeros!\n");
 
	
	fclose(arq_pedidos);
 
	printf("\nPressione <Enter> para continuar...");
	scanf("%*c");
 
	fseek(stdin, 0, SEEK_END); 
}
 
 

void excluir_pedido()
{
	char str_id_pedido[10];
	int id_pedido;
 
	printf("\nDigite o ID do prato: ");
	scanf("%10s%*c", str_id_pedido);
 
	fseek(stdin, 0, SEEK_END); 
 
	
	if(str_somente_numeros(str_id_pedido) == 1)
	{
		
		sscanf(str_id_pedido, "%d", &id_pedido);
 
		
		FILE *arq_pedidos = fopen("pedidos.bin", "rb");
 
		if(arq_pedidos == NULL)
		{
			printf("\nFalha ao abrir arquivo(s)!\n");
			exit(1); 
		}
 
		
		if(existe_pedido(arq_pedidos, id_pedido) == 1)
		{
			char nome_pedido[MAX];
			
			FILE *arq_temp = fopen("temp_pedidos.bin", "a+b");
			if(arq_temp == NULL)
			{
				printf("\nFalha ao criar arquivo temporario!\n");
				fclose(arq_pedidos);
				exit(1); 
			}
			rewind(arq_pedidos); 
 
			t_pedido pedido;
			while(1) 
			{
 
				
				size_t result = fread(&pedido, sizeof(t_pedido), 1, arq_pedidos);
 
				
				if(result == 0)
					break;
 
				
				if(pedido.id != id_pedido)
				{
				
					fwrite(&pedido, sizeof(t_pedido), 1, arq_temp);
				}
				else
					strcpy(nome_pedido, pedido.nome);
			}
 
			fclose(arq_pedidos);
			fclose(arq_temp);
 
			
			if(remove("pedidos.bin") != 0)
				printf("\nErro ao deletar o arquivo \"pedidos.bin\"\n");
			else
			{
				
				int r = rename("temp_pedidos.bin", "pedidos.bin");
				if(r != 0)
				{
					printf("\nPermissao negada para renomear o arquivo!\n");
					printf("Feche esse programa bem como o arquivo \"temp_pedidos.bin\" e renomeie manualmente para \"pedidos.bin\"\n");
				}
				else
					printf("\npedido \"%s\" removido com sucesso!\n", nome_pedido);
			}
		}
		else
		{
			fclose(arq_pedidos);
			printf("\nNao existe pedido com o ID \"%d\".\n", id_pedido);
		}
	}
	else
		printf("\nO ID so pode conter numeros!\n");
 
	printf("\nPressione <Enter> para continuar...");
	scanf("%*c");
	fseek(stdin, 0, SEEK_END); 
}
