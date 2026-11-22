//Nomes: Gabriel Zini Carminitti
//Leonardo Guerra de Godoy
//Thaynan Santa Fe Martins

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct linha{
      char id[2];
      char cliente[6];
      char conta[7];
      char num_boleto[9];
      char data_vencimento[9];
      char data_pagamento[9];
      float valor;
  } linha2;

void separadados(char *str, linha2 l[], int p){
  int i;
  char valor[10];

  l[p].id[0] = str[0];
  l[p].id[1] = '\0';

  for(i=1;i<=5;i++){
    l[p].cliente[i-1]= str[i];
  }
  l[p].cliente[5] = '\0';

  for(i=6;i<=11;i++){
    l[p].conta[i-6]= str[i];
  }
  l[p].conta[6] = '\0';

  for(i=12;i<=19;i++){
    l[p].num_boleto[i-12]= str[i];
  }
  l[p].num_boleto[8] = '\0';

  for(i=20;i<=27;i++){
    l[p].data_vencimento[i-20]= str[i];
  }
  l[p].data_vencimento[8] = '\0';

  for(i=28;i<=35;i++){
    l[p].data_pagamento[i-28]= str[i];
  }
  l[p].data_pagamento[8] = '\0';

  for(i=36;i<=41;i++){
    valor[i-36]= str[i];
  }
  valor[6] = '.';
  valor[7] = str[42];
  valor[8] = str[43];
  valor[9] = '\0';
  l[p].valor = atof(valor);
}

void gera_arquivo_erro(int linha, char *mensagem){
  FILE *filerro;

  filerro = fopen("ERRO.TXT", "a");

  if (filerro == NULL) {
    printf("\nNao foi possivel criar o arquivo ERRO.TXT");
    return;
  }

  fprintf(filerro, "Inicio do erro\n");
  if (linha > 0){
    fprintf(filerro, "Linha da remessa: %d\n", linha);
  }
  fprintf(filerro, "Descricao: %s\n", mensagem);
  fprintf(filerro, "Fim do erro\n\n");
  fclose(filerro);
}

void gera_arquivo_retorno(char *data, linha2 l){
  FILE *fileretorno;
  char nomearquivo[20];
  char cabecalho[20] = "1";
  char valor[8];
  int existe = 0;

  snprintf(valor, 8, "%07d", (int)lround(l.valor * 100));

  strcpy(nomearquivo, data);
  strcat(nomearquivo, "_");
  strcat(nomearquivo, l.cliente);
  strcat(cabecalho, data);

  fileretorno = fopen(nomearquivo, "r");
  if (fileretorno != NULL) {
    fclose(fileretorno);
    existe = 1;
  }else{
    existe = 0;
  }

  if (existe == 1) {
    fileretorno = fopen(nomearquivo, "a");
  } else {
    fileretorno = fopen(nomearquivo, "w");
    fprintf(fileretorno, "%s", cabecalho);
  }

  if (fileretorno == NULL) {
    gera_arquivo_erro(0, "Nao foi possivel criar o arquivo de retorno");
    printf("\nErro");
    return;
  }

  fprintf(fileretorno, "\n%s%s%s%s%s", l.id, l.conta, l.num_boleto, l.data_pagamento, valor);
  fclose(fileretorno);
}

int main(void){
 
  char str[46], data[9];
  int i, p=0, contalinhas=0;
  linha2 *l = NULL;
  FILE *fp;
  fp = fopen("remessa.txt", "r");
  if (fp == NULL) {
    gera_arquivo_erro(0, "Nao foi possivel abrir o arquivo de remessa / Arquivo nao encontrado");
    printf("\nErro");
    return 1;
  }

  fgets(str,sizeof(str), fp);
  if(str[0] != '1'){
    gera_arquivo_erro(1, "O primeiro registro da remessa deve iniciar com 1");
    printf("\nErro");
    return 1;
  }else{
    for(i=1;i<=8;i++){
      data[i-1] = str[i];
    }
    data[8] = '\0';
  }
  printf("%s", data);

  while(fgets(str,sizeof(str), fp) != NULL){
    contalinhas++;
  }
   
  l = (linha2*) malloc(contalinhas*sizeof(linha2));

  rewind(fp);
  fgets(str,sizeof(str), fp);

  for(p=0; p<contalinhas; p++){
    fgets(str,sizeof(str), fp);
    if(str[0] != '2'){
      gera_arquivo_erro(p + 2, "Os registros apos o cabecalho devem iniciar com 2 indicando um boleto pago");
      printf("\nErro");
      return 1;
    }
    separadados(str, l, p);
  }
  fclose(fp);
   
  for(i=0; i<p; i++){
    printf("\nID = %s", l[i].id);
    printf("\nCliente = %s", l[i].cliente);
    printf("\nConta = %s", l[i].conta);
    printf("\nNumero do Boleto = %s", l[i].num_boleto);
    printf("\nData de Vencimento = %s", l[i].data_vencimento);
    printf("\nData de Pagamento = %s", l[i].data_pagamento);
    printf("\nValor = %.2f", l[i].valor);

    gera_arquivo_retorno(data, l[i]);
  }

  free(l);
  return 0;
}