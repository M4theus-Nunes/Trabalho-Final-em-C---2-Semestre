#include "biblioteca.h"

int main() {
  PetShop *petshop, *auxiliar;
  int opcao, registros = 0, ver;

  // contando arquivos.

  arquivo = fopen(nome_arquivo, "rb+");
  if (arquivo == NULL) {
    printf("    ╔═══════════════╗\n");
    printf("    ║ Arquivo Vazio ║\n");
    printf("    ╚═══════════════╝\n");
    registros = 0;
    getchar();
  } else {

    fseek(arquivo, 0, SEEK_END);

    registros = ftell(arquivo) / sizeof(PetShop);

    rewind(arquivo);

    petshop = (PetShop *)malloc(registros * sizeof(PetShop));

    for (int i = 0; i < registros; i++) {
      PetShop aux;
      fread(&aux, sizeof(PetShop), 1, arquivo);
      petshop[i] = aux;
    }

    fclose(arquivo);
  }

  do {
    arquivo = fopen (nome_arquivo, "ab+");
    system("clear");
    printf("╔═══════════════════════════════╗\n");
    printf("║        Registros - 0%i         ║\n", registros);
    printf("╠═══════════════════════════════╣\n");
    printf("║      O que deseja fazer?      ║\n");
    printf("╠═══╦═══════════════════════════╣\n");
    printf("║ 1 ║     Inserir registro      ║\n");
    printf("╠═══╬═══════════════════════════╣\n");
    printf("║ 2 ║     Excluir registro      ║\n");
    printf("╠═══╬═══════════════════════════╣\n");
    printf("║ 3 ║      Editar registro      ║\n");
    printf("╠═══╬═══════════════════════════╣\n");
    printf("║ 4 ║     Mostrar registros     ║\n");
    printf("╠═══╬═══════════════════════════╣\n");
    printf("║ 0 ║     Encerrar programa     ║\n");
    printf("╚═══╩═══════════════════════════╝\n");
    do {
      printf("              ▶ ");
      scanf("%i", &opcao);
    } while (opcao < 0 || opcao > 4);

    if (opcao == 1) {
      if (registros == 0) {
        petshop = malloc(1 * sizeof(PetShop));
      }
      petshop = realloc(petshop, (registros + 1) * sizeof(PetShop));
      Inserir_registro(petshop, registros);
      registros++;
      Salvar(petshop, registros);
    }
    if (opcao == 2) {
      if (registros != 0) {
        ver = Excluir_registro(petshop, registros);
        if (ver == 1) {
          registros--;
          Salvar(petshop, registros);
        }
      } else {
        printf("\n        ╔═══════════════╗\n");
        printf("        ║ Arquivo Vazio ║\n");
        printf("        ╚═══════════════╝\n");
        getchar();
        getchar();
      }
    }
    if (opcao == 3) {
      if (registros != 0) {
        Editar_registro(petshop, registros);
        Salvar(petshop, registros);
      } else {
        printf("\n        ╔═══════════════╗\n");
        printf("        ║ Arquivo Vazio ║\n");
        printf("        ╚═══════════════╝\n");
        getchar();
        getchar();
      }
    }
    if (opcao == 4) {
      if (registros != 0) {

        Mostrar_registro(petshop, registros);
        Salvar(petshop, registros);
      } else {
        printf("\n        ╔═══════════════╗\n");
        printf("        ║ Arquivo Vazio ║\n");
        printf("        ╚═══════════════╝\n");
        getchar();
        getchar();
      }
    }
    if (opcao == 0)
      Encerrar_programa();
  } while (opcao != 0);
  fclose(arquivo);
}