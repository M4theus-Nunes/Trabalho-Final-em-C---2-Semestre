#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

FILE *arquivo;
FILE *auxiliar;
const char *nome_arquivo = "pet.dat";

typedef struct {
  int ID;
  char Nome_prop[100];
  char Nome_pet[20];
  char Especie[50];
  char Raca[50];
  int Idade;
  float Peso;
  char Sexo;
  char Castrado;
} PetShop;

int ContarInt(int num) {
  int contador = 0;
  do {
    num = num / 10;
    contador++;
  } while (num != 0);
  // o contador vai levar a quantidade de 'casas' que o num ocupa
  return contador;
}

int ContarFloat(float num) {
  int contador = 0;
  char StringNumero[50];
  // aqui o valor float de num vai virar uma string, .2 serve pra limitar
  // somente dois zeros a direita
  sprintf(StringNumero, "%.2f", num);
  // contando quantas 'casas' essa string ocupa
  contador = strlen(StringNumero);
  return contador;
}

void Salvar(PetShop *petshop, int registros) {

  arquivo = fopen(nome_arquivo, "wb");

  fseek(arquivo, 0, SEEK_SET);
  for (int i = 0; i < registros; i++) {
    fwrite(&petshop[i], sizeof(PetShop), 1, arquivo);
  }
  fclose(arquivo);
}

void Inserir_registro(PetShop *petshop, int registros) {
  int verifica = 0, id = 0;

  printf("\n   ╔═════════════════════════╗\n");
  printf("   ║ Insira os dados do pet: ║\n");
  printf("   ╚═════════════════════════╝\n\n");

  arquivo = fopen(nome_arquivo, "ab+");

  do {
    verifica = 0;
    printf(" ID➜ ");
    scanf("%d", &id);

    for (int i = 0; i < registros; i++) {
      if (id == petshop[i].ID) {
        verifica = 1;
        printf(" ID indisponivel\n");
        getchar();
        getchar();
        break;
      }
      if (id < 0) {
        printf(" Não pode numero negativo\n");
        getchar();
        getchar();
        break;
      }
    }
  } while (verifica == 1 || id < 0);
  getchar();

  petshop[registros].ID = id;

  printf(" Nome do proprietário ➜ ");
  fgets(petshop[registros].Nome_prop, sizeof(petshop[registros].Nome_prop),
        stdin);
  petshop[registros].Nome_prop[strcspn(petshop[registros].Nome_prop, "\n")] =
      '\0';

  printf(" Nome do pet ➜ ");
  fgets(petshop[registros].Nome_pet, sizeof(petshop[registros].Nome_pet),
        stdin);
  petshop[registros].Nome_pet[strcspn(petshop[registros].Nome_pet, "\n")] =
      '\0';

  printf(" Especie ➜ ");
  fgets(petshop[registros].Especie, sizeof(petshop[registros].Especie), stdin);
  petshop[registros].Especie[strcspn(petshop[registros].Especie, "\n")] = '\0';

  printf(" Raca ➜ ");
  fgets(petshop[registros].Raca, sizeof(petshop[registros].Raca), stdin);
  petshop[registros].Raca[strcspn(petshop[registros].Raca, "\n")] = '\0';

  printf(" Idade ➜ ");
  scanf("%d", &petshop[registros].Idade);

  printf(" Peso ➜ ");
  scanf("%f", &petshop[registros].Peso);

  // verificando o sexo
  do {
    getchar();
    printf(" Sexo ➜ ");
    scanf("%c", &petshop[registros].Sexo);
    if (petshop[registros].Sexo != 'F' && petshop[registros].Sexo != 'f' &&
        petshop[registros].Sexo != 'M' && petshop[registros].Sexo != 'm') {
      printf(" Somente (M) ou (F)\n");
      getchar();
    }
  } while (petshop[registros].Sexo != 'F' && petshop[registros].Sexo != 'f' &&
           petshop[registros].Sexo != 'M' && petshop[registros].Sexo != 'm');

  // verificando se é castrado
  do {
    getchar();
    printf(" Castrado ➜ ");
    scanf("%c", &petshop[registros].Castrado);
    if (petshop[registros].Castrado != 's' &&
        petshop[registros].Castrado != 'S' &&
        petshop[registros].Castrado != 'n' &&
        petshop[registros].Castrado != 'N') {
      printf(" Somente (S) ou (N)\n");
      getchar();
    }
  } while (petshop[registros].Castrado != 's' &&
           petshop[registros].Castrado != 'S' &&
           petshop[registros].Castrado != 'n' &&
           petshop[registros].Castrado != 'N');

  printf("\n╔═══════════════════════════════════╗\n");
  printf("║   Registro inserido com sucesso   ║\n");
  printf("╚═══════════════════════════════════╝\n");

  getchar();
  getchar();

  fwrite(&petshop[registros], sizeof(PetShop), 1, arquivo);
  fclose(arquivo);
}

int Excluir_registro(PetShop *petshop, int registros) {
  int id, i, j, ver = 0;

  // verificando id
  printf("\n         Digite o ID ➜ ");
  scanf("%i", &id);

  for (i = 0; i < registros; i++) {
    if (id == petshop[i].ID) {
      ver = 1;
      // move os registros restantes uma posição para trás para "remover" o
      // registro
      for (j = i; j < registros - 1; j++) {
        petshop[j] = petshop[j + 1];
      }
      printf("\n      ╔═══════════════════╗\n");
      printf("      ║ Registro removido ║\n");
      printf("      ╚═══════════════════╝\n");
      getchar();
      getchar();
      break; // sai do loop após encontrar e excluir o registro
    }
  }
  if (ver != 1) {
    printf("\n    ╔═════════════════════════╗\n");
    printf("    ║ Registro nao encontrado ║\n");
    printf("    ╚═════════════════════════╝\n");
    getchar();
    getchar();
  }
  return ver;
}

void Editar_registro(PetShop *petshop, int registros) {
  int opcao = 0, id = 0, reg = 0, existe = 0, posicao = 0, verifica = 0, i = 0;

  // digitando o ID que deseja editar e verificando se ele existe
  printf("\n         Digite o ID ➜ ");
  scanf("%i", &id);

  for (i = 0; i < registros; i++) {
    if (id == petshop[i].ID) {
      verifica = 1;
      break;
    }
  }

  if (verifica != 1) {
    printf("\n    ╔═════════════════════════╗\n");
    printf("    ║ Registro nao encontrado ║\n");
    printf("    ╚═════════════════════════╝\n");
    getchar();
    getchar();
    return;
  }

  verifica = 0;
  // se o ID for encontrado, existe 2 opções de edição
  system("clear");
  printf(" ╔═══╦════════════════════════╗\n");
  printf(" ║ 1 ║   Editar todos dados   ║\n");
  printf(" ╠═══╬════════════════════════╣\n");
  printf(" ║ 2 ║     Editar um dado     ║\n");
  printf(" ╚═══╩════════════════════════╝\n");
  do {
    printf("              ▶ ");
    scanf("%i", &opcao);
  } while (opcao < 1 || opcao > 2);
  printf("\n");

  switch (opcao) {
  case 1: {

    // caso escolha a 1, vai editar todo o registro, igual quando foi inserido
    for (i = 0; i < registros; i++) {
      if (id == petshop[i].ID) {
        posicao = i;
        break;
      }
    }

    do {
      verifica = 0;
      printf(" ID ➜ ");
      scanf("%d", &id);

      for (i = 0; i < registros; i++) {
        if (id < 0) {
          printf(" Não pode numero negativo\n");
          getchar();
          getchar();
          break;
        }
        if (id == petshop[i].ID) {
          verifica = 1;
          printf(" ID indisponivel\n");
          getchar();
          getchar();
          break;
        }
      }
    } while (id < 0 || verifica == 1);

    // inserindo os novos dados do registro

    petshop[posicao].ID = id;
    getchar();

    printf(" Nome do proprietário ➜ ");
    fgets(petshop[posicao].Nome_prop, sizeof(petshop[posicao].Nome_prop),
          stdin);
    petshop[posicao].Nome_prop[strcspn(petshop[posicao].Nome_prop, "\n")] =
        '\0';

    printf(" Nome do pet ➜ ");
    fgets(petshop[posicao].Nome_pet, sizeof(petshop[posicao].Nome_pet), stdin);
    petshop[posicao].Nome_pet[strcspn(petshop[posicao].Nome_pet, "\n")] = '\0';

    printf(" Especie ➜ ");
    fgets(petshop[posicao].Especie, sizeof(petshop[posicao].Especie), stdin);
    petshop[posicao].Especie[strcspn(petshop[posicao].Especie, "\n")] = '\0';

    printf(" Raca ➜ ");
    fgets(petshop[posicao].Raca, sizeof(petshop[posicao].Raca), stdin);
    petshop[posicao].Raca[strcspn(petshop[posicao].Raca, "\n")] = '\0';

    printf(" Idade ➜ ");
    scanf("%d", &petshop[posicao].Idade);

    printf(" Peso ➜ ");
    scanf("%f", &petshop[posicao].Peso);

    // verificando sexo
    do {
      getchar();
      printf(" Sexo ➜ ");
      scanf("%c", &petshop[posicao].Sexo);
      if (petshop[posicao].Sexo != 'F' && petshop[posicao].Sexo != 'f' &&
          petshop[posicao].Sexo != 'M' && petshop[posicao].Sexo != 'm') {
        printf(" Somente (M) ou (F)\n");
        getchar();
      }
    } while (petshop[posicao].Sexo != 'F' && petshop[posicao].Sexo != 'f' &&
             petshop[posicao].Sexo != 'M' && petshop[posicao].Sexo != 'm');

    // verificando castração
    do {
      getchar();
      printf(" Castrado ➜ ");
      scanf("%c", &petshop[posicao].Castrado);
      if (petshop[posicao].Castrado != 's' &&
          petshop[posicao].Castrado != 'S' &&
          petshop[posicao].Castrado != 'n' &&
          petshop[posicao].Castrado != 'N') {
        printf(" Somente (S) ou (N)\n");
        getchar();
      }
    } while (
        petshop[posicao].Castrado != 's' && petshop[posicao].Castrado != 'S' &&
        petshop[posicao].Castrado != 'n' && petshop[posicao].Castrado != 'N');

    printf("\n ╔════════════════════════════╗\n");
    printf(" ║ Dados editados com sucesso ║\n");
    printf(" ╚════════════════════════════╝\n");
    getchar();
    getchar();
  } break;

  case 2: {
    // caso escolha o 2, vai ser possivel escolher apenas 1 dado para edição
    system("clear");
    printf(" ╔════════════════════════════╗\n");
    printf(" ║ Escolha o dado para editar ║\n");
    printf(" ╠═══╦════════════════════════╣\n");
    printf(" ║ 1 ║           ID           ║\n");
    printf(" ╠═══╬════════════════════════╣\n");
    printf(" ║ 2 ║  Nome do Proprietario  ║\n");
    printf(" ╠═══╬════════════════════════╣\n");
    printf(" ║ 3 ║      Nome do Pet       ║\n");
    printf(" ╠═══╬════════════════════════╣\n");
    printf(" ║ 4 ║        Especie         ║\n");
    printf(" ╠═══╬════════════════════════╣\n");
    printf(" ║ 5 ║          Raca          ║\n");
    printf(" ╠═══╬════════════════════════╣\n");
    printf(" ║ 6 ║         Idade          ║\n");
    printf(" ╠═══╬════════════════════════╣\n");
    printf(" ║ 7 ║          Peso          ║\n");
    printf(" ╠═══╬════════════════════════╣\n");
    printf(" ║ 8 ║          Sexo          ║\n");
    printf(" ╠═══╬════════════════════════╣\n");
    printf(" ║ 9 ║        Castrado        ║\n");
    printf(" ╚═══╩════════════════════════╝\n");
    do {
      printf("              ▶ ");
      scanf("%i", &opcao);
    } while (opcao < 1 || opcao > 9);

    for (i = 0; i < registros; i++) {
      if (id == petshop[i].ID) {
        posicao = i;
        break;
      }
    }
    printf("\n");
    switch (opcao) {
    case 1: {

      do {
        verifica = 0;
        printf(" ID ➜ ");
        scanf("%d", &id);

        for (i = 0; i < registros; i++) {
          if (id < 0) {
            printf(" Não pode numero negativo\n");
            getchar();
            getchar();
            break;
          }
          if (id == petshop[i].ID) {
            verifica = 1;
            printf(" ID indisponivel\n");
            getchar();
            getchar();
            break;
          }
        }
      } while (id < 0 || verifica == 1);

      petshop[posicao].ID = id;
      getchar();
    } break;

    case 2: {
      getchar();
      printf("  Nome do proprietário ➜ ");
      fgets(petshop[posicao].Nome_prop, sizeof(petshop[posicao].Nome_prop),
            stdin);
      petshop[posicao].Nome_prop[strcspn(petshop[posicao].Nome_prop, "\n")] =
          '\0';
    } break;

    case 3: {
      getchar();
      printf("  Nome do pet ➜ ");
      fgets(petshop[posicao].Nome_pet, sizeof(petshop[posicao].Nome_pet),
            stdin);
      petshop[posicao].Nome_pet[strcspn(petshop[posicao].Nome_pet, "\n")] =
          '\0';
    } break;

    case 4: {
      getchar();
      printf("  Especie ➜ ");
      fgets(petshop[posicao].Especie, sizeof(petshop[posicao].Especie), stdin);
      petshop[posicao].Especie[strcspn(petshop[posicao].Especie, "\n")] = '\0';
    } break;

    case 5: {
      getchar();
      printf("  Raca ➜ ");
      fgets(petshop[posicao].Raca, sizeof(petshop[posicao].Raca), stdin);
      petshop[posicao].Raca[strcspn(petshop[posicao].Raca, "\n")] = '\0';
    } break;

    case 6: {
      getchar();
      printf("  Idade ➜ ");
      scanf("%d", &petshop[posicao].Idade);
      getchar();
    } break;

    case 7: {
      getchar();
      printf("  Peso ➜ ");
      scanf("%f", &petshop[posicao].Peso);
      getchar();
    } break;

    case 8: {
      do {
        getchar();

        // verificar sexo
        printf("  Sexo ➜ ");
        scanf("%c", &petshop[posicao].Sexo);
        if (petshop[posicao].Sexo != 'F' && petshop[posicao].Sexo != 'f' &&
            petshop[posicao].Sexo != 'M' && petshop[posicao].Sexo != 'm') {
          printf("  Somente (M) ou (F)\n");
          getchar();
        }
      } while (petshop[posicao].Sexo != 'F' && petshop[posicao].Sexo != 'f' &&
               petshop[posicao].Sexo != 'M' && petshop[posicao].Sexo != 'm');
      getchar();
    } break;

    case 9: {
      do {
        getchar();

        // verificar castração
        printf("  Castrado ➜ ");
        scanf("%c", &petshop[posicao].Castrado);
        if (petshop[posicao].Castrado != 's' &&
            petshop[posicao].Castrado != 'S' &&
            petshop[posicao].Castrado != 'n' &&
            petshop[posicao].Castrado != 'N') {
          printf("  Somente (S) ou (N)\n");
          getchar();
        }
      } while (petshop[posicao].Castrado != 's' &&
               petshop[posicao].Castrado != 'S' &&
               petshop[posicao].Castrado != 'n' &&
               petshop[posicao].Castrado != 'N');
      getchar();
    } break;
    }

    printf("\n  ╔══════════════════════════╗\n");
    printf("  ║ Dado editado com sucesso ║\n");
    printf("  ╚══════════════════════════╝\n");
    getchar();

  } break;
  }
}

void Mostrar_registro(PetShop *petshop, int registros) {
  int reg = 0, num, tamanho;

  printf("\n");

  for (int i = 0; i < registros; i++) {

    // usando nome do proprietario como base pra escrever bonito
    int base = strlen(petshop[i].Nome_prop);

    // com a base é possivel escrever os registros bonitos e sem erro
    printf("╔");
    for (int contp = 0; contp < base + 24; contp++) {
      printf("═");
    }
    printf("╗\n");

    // exemplo, o "num" recebe a contagem de quantas 'casas' a variavel que foi
    // enviada para a funçao "ContarInt" ocupa, assim consigo saber quantos
    // espaços preciso dar exatamente
    reg++;
    printf("║ Registro - %i", reg);
    num = ContarInt(reg);
    for (int contp = 0; contp < (base + 24) - (12 + num); contp++) {
      printf(" ");
    }
    printf("║\n");

    printf("║");
    for (int contp = 0; contp < (base + 24); contp++) {
      printf(" ");
    }
    printf("║\n");

    printf("║ ID: %i", petshop[i].ID);
    num = ContarInt(petshop[i].ID);
    for (int contp = 0; contp < (base + 24) - (5 + num); contp++) {
      printf(" ");
    }
    printf("║\n");

    tamanho = strlen(petshop[i].Nome_prop);
    printf("║ Nome do proprietario: %s", petshop[i].Nome_prop);
    for (int contp = 0; contp < (base + 24) - (23 + tamanho); contp++) {
      printf(" ");
    }
    printf("║\n");

    tamanho = strlen(petshop[i].Nome_pet);
    printf("║ Nome do pet: %s", petshop[i].Nome_pet);
    for (int contp = 0; contp < (base + 24) - (14 + tamanho); contp++) {
      printf(" ");
    }
    printf("║\n");

    tamanho = strlen(petshop[i].Especie);
    printf("║ Especie: %s", petshop[i].Especie);
    for (int contp = 0; contp < (base + 24) - (10 + tamanho); contp++) {
      printf(" ");
    }
    printf("║\n");

    tamanho = strlen(petshop[i].Raca);
    printf("║ Raca: %s", petshop[i].Raca);
    for (int contp = 0; contp < (base + 24) - (7 + tamanho); contp++) {
      printf(" ");
    }
    printf("║\n");

    printf("║ Idade: %i", petshop[i].Idade);
    num = ContarInt(petshop[i].Idade);
    for (int contp = 0; contp < (base + 24) - (8 + num); contp++) {
      printf(" ");
    }
    printf("║\n");

    printf("║ Peso: %.2f Kg", petshop[i].Peso);
    num = ContarFloat(petshop[i].Peso);
    for (int contp = 0; contp < (base + 24) - (10 + num); contp++) {
      printf(" ");
    }
    printf("║\n");

    printf("║ Sexo: %c", petshop[i].Sexo);
    for (int contp = 0; contp < (base + 24) - (7 + 1); contp++) {
      printf(" ");
    }
    printf("║\n");

    printf("║ Castrado: %c", petshop[i].Castrado);
    for (int contp = 0; contp < (base + 24) - (11 + 1); contp++) {
      printf(" ");
    }
    printf("║\n");

    printf("╚");
    for (int contp = 0; contp < base + 24; contp++) {
      printf("═");
    }
    printf("╝\n");
  }

  getchar();
  getchar();
}

void Encerrar_programa() {
  // finalizando o programa
  printf("\n ╔═════════════════════════════╗\n");
  printf(" ║ Nunca é um adeus, até logo! ║\n");
  printf(" ╚═════════════════════════════╝\n");
}