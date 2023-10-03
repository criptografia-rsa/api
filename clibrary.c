#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <string.h>

char* hello(char* name, int age){
    printf("My name is %s and i am %d years old\n", name, age);
    return "Ola" ;
}

int preCodificar(char letra){
    //Declarando nossa referência da ACSII | Intervalo : 32-126
    char caracteres[95] = {
        ' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?',
        '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
        'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_',
        '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
        'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~'
    };

    //Intera o array e verifica se o caracter i é igual ao passado
    for (int i = 0; i < 95; i++)
    {
        if(letra == caracteres[i]){
            return i + 32;
        } 
    }
    return -1;
}

void fastExpoMod(mpz_t base, mpz_t expoente, mpz_t n, mpz_t lastRest ){
    //Declarando expoente temporário
    mpz_t expoente_temp;
    mpz_init(expoente_temp);
    mpz_set(expoente_temp, expoente);

    mpz_t binary_inverse[10000], binary[10000], resto[10000];

    //Transforma o numero para binario (Invertido)
    int i = 0;
    while (mpz_cmp_ui(expoente_temp, 0) > 0)
    {   
        mpz_t expo_mod_2; 
        mpz_inits(expo_mod_2, binary_inverse[i], NULL);

        //Calcula expoente % 2
        mpz_mod_ui(expo_mod_2, expoente_temp, 2);

        //Atribuição
        mpz_set(binary_inverse[i], expo_mod_2);

        //Calcula expoente/2
        mpz_div_ui(expoente_temp, expoente_temp, 2);
        i++;
    }

    mpz_set(resto[0], base);
    
    //Correção do binário & Aplicação da Exponenciação Modular Rápida
    for (int k = 1; k < i; k++)
    {   
        mpz_init(binary[k]);
        mpz_set(binary[k], binary_inverse[i - k - 1]);
        
        //Calculando resto[k] = resto[k - 1]² mod n
        mpz_mul(resto[k], resto[k - 1], resto[k - 1]);
        mpz_mod(resto[k], resto[k], n);

        if(mpz_cmp_ui(binary[k], 1) == 0){
            //resto[k] = (resto[k]*base) % n;
            mpz_mul(resto[k], resto[k], base);
            mpz_mod(resto[k], resto[k], n);
        }
    }
    
    mpz_set(lastRest, resto[i-1]);

    return;
}

void inverso_modular(mpz_t d, mpz_t e_temp, mpz_t k_temp){
    //Declarando os vetores necessários
    mpz_t resto[100000],  quociente[100000] , m[100000], n[100000], aux, mod_k;

    //Inicialização dos dois primeiros elementos de resto[], m[] e n[]
    mpz_inits(resto[0], resto[1],  m[0], m[1],   n[0], n[1], aux, mod_k, NULL);
    mpz_inits(quociente[0], quociente[1], NULL);

    //Atribuindo os valores conhecidos para o funcionamento da solução 
    mpz_set(resto[0], e_temp); mpz_set(resto[1], k_temp);
    mpz_set_ui(m[0], 1); mpz_set_ui(m[1], 0);
    mpz_set_ui(n[0], 0); mpz_set_ui(n[1], 1);
    mpz_set(mod_k, k_temp);

    //Calcular a mod b
    mpz_mod(aux, e_temp, k_temp);

    int tam = 0;
    while (mpz_cmp_ui(aux, 0) != 0)
    {   
        //Inicializando os novos elementos
        mpz_inits(resto[tam + 2], quociente[tam+2], m[tam+2], n[tam+2], NULL);

        //Calcula o novo quociente do array quociente[]
        mpz_tdiv_q(quociente[tam+2], resto[tam], resto[tam+1]);

        //Calcula o  novo resto do array resto[]
        mpz_mod(resto[tam + 2], resto[tam], resto[tam+1]);

        //Calculando o produto mq e nq
        mpz_t produto_mq,produto_nq;
        mpz_inits(produto_mq, produto_nq, NULL);

        mpz_mul(produto_mq, m[tam+1], quociente[tam+2]);
        mpz_mul(produto_nq, n[tam+1], quociente[tam+2]);

        //Calcula o novo m do array m[]
        mpz_sub(m[tam+2], m[tam], produto_mq );
        mpz_sub(n[tam+2], n[tam], produto_nq );

        mpz_set(e_temp, k_temp);
        mpz_set(k_temp, resto[tam+2]);
    
        //Calcular a mod b
        mpz_mod(aux, e_temp, k_temp);
        tam++;
    }

    // Verificar se m[index+1] é negativo
    while (mpz_cmp_ui(m[tam + 1], 0) < 0) {
        // Adicionar mod_k a m[index+1]
        mpz_add(m[tam + 1], m[tam + 1], mod_k);
    }

    mpz_set(d, m[tam+1]);
    
    return;
}

char letra_correspondente(mpz_t index){
    //Caracteres definidos
    char caracteres[95] = {
        ' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?',
        '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
        'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_',
        '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
        'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~'
    };
    
    long int intIndex = mpz_get_si(index);

    //Caso caracter estiver fora do nosso escopo
    if(intIndex -32 > 94 || intIndex < 0 ){
        return 'X';
    } 
   
    return caracteres[intIndex - 32];
}

char *chavePublica(char *p_string,char *q_string, char *e_string){
    printf("%s %s %s", p_string, q_string,e_string);
    //Declarando as variáveis mpz
    mpz_t p,q,e,n;
    mpz_inits(p, q,e,n, NULL);

    //Convertendo a entrada para mpz (Inteiro Arbitrariamente Grande)
    mpz_init_set_str(p, p_string, 10);
    mpz_init_set_str(q, q_string, 10);
    mpz_init_set_str(e, e_string, 10);

    //Calcula a Chave Publica 
    mpz_mul(n, p, q);

    //Converte os dados em String
    char *str_n, *str_e;
    str_n =  mpz_get_str(NULL, 10, n);
    str_e =  mpz_get_str(NULL, 10, e);

    //Concatenando as strings 
    int tam_string = strlen(str_n) + strlen(str_e) + 2;
    char *response = (char *)malloc(tam_string * sizeof(char));

    snprintf(response, tam_string, "%s %s", str_n, str_e);

    //Cria um txt com os dados 
    FILE *file;
    file = fopen("chave-publica.txt", "w");
    fprintf(file, "%s",response);
    fclose(file);
    
    //Limpa memória alocada
    mpz_clears(p, q, e,n, NULL);
    free(str_e); free(str_n);

    return response;
}




char *encriptar( char *mensagem, unsigned char *n_string, unsigned char  *e_string){
    //Declarando e Inicializando variáveis arbitrariamentre grandes 
    mpz_t n, e, base;
    mpz_inits(n, e, base, NULL);

    //Convertendo a string lida para inteiro gmp
    mpz_init_set_str(n, n_string, 10);
    mpz_init_set_str(e, e_string, 10);
    
    //Pré-Codificar
    int *array_pre_codificado = NULL;
    size_t buffer_size_1 = 4096; 
    array_pre_codificado = (int *)malloc(buffer_size_1);

    //Tamanho da String Mensagem
    int tam_max = strlen(mensagem);

    //Armazena as variaveis com valores codificados
    mpz_t array_codificado[tam_max];

    for (int i = 0; i < tam_max - 1; i++)
    {
        array_pre_codificado[i] = preCodificar(mensagem[i]);
        mpz_init(array_codificado[i]);
        mpz_set_ui(base, array_pre_codificado[i]);

        // Calcula (base^exponent) % modulus
        fastExpoMod(base,e,n,array_codificado[i]);
    }
    
    //Cria um txt com os dados 
    FILE *file;
    file = fopen("mensagem-encriptada.txt", "w");

    char *response_encriptado = NULL;
    //size_t buffer_size_r = 4096; 
    response_encriptado = (char *)malloc(buffer_size_1);


    for (int k = 0; k < tam_max -1; k++)
    {   
        gmp_fprintf(file, "%Zd ",array_codificado[k]);

        char *str = mpz_get_str(NULL, 10, array_codificado[k]);
        strcat(response_encriptado, str); 
        strcat(response_encriptado, " "); 
        free(str); 
    }
    fclose(file);

    return response_encriptado;
}

char *desencriptar(char *p_string,char *q_string, char *e_string, char *msg_encriptada){
    //Declarando e inicializando as variáveis arbitrariamente grandes 
    mpz_t p, q, e, d, k, p_menos_1, q_menos_1, n;
    mpz_inits(p, q, e, d, k, p_menos_1, q_menos_1, n, NULL);

    //Convertendo as string para variáveis arbitrariamente grandes 
    mpz_init_set_str(p, p_string, 10);
    mpz_init_set_str(q, q_string, 10);      
    mpz_init_set_str(e, e_string, 10);

    mpz_mul(n, p, q);

    //Definindo (p-1)*(q-1) 
    mpz_sub_ui(p_menos_1, p, 1);
    mpz_sub_ui(q_menos_1, q, 1);
    mpz_mul(k, p_menos_1, q_menos_1);

    //Encontrar o inverso modular entre [e] e [k] onde k = (p-1)*(q-1)
    inverso_modular(d, e, k );

    //Converter a string msg_encriptada em um array mpz 
    mpz_t valores_encriptado[255];
    char *valor_aux_string;

    //Divide os valores a cada espaço encontrado
    valor_aux_string = strtok(msg_encriptada, " ");
    
    int z = 0;
    while (valor_aux_string != NULL)
    {   
        mpz_init(valores_encriptado[z]);
        mpz_init_set_str(valores_encriptado[z], valor_aux_string, 10);

        valor_aux_string = strtok(NULL, " ");
        z++;
    }
    
    mpz_t msg_desencriptada[255]; char *txt_descodificado = NULL;
    size_t buffer_size_0 = 4096; 
    txt_descodificado = (char *)malloc(buffer_size_0);
    
    for (int in = 0; in < z  ; in++)
    {   
        mpz_init(msg_desencriptada[in]);
        
        fastExpoMod(valores_encriptado[in], d, n, msg_desencriptada[in]);
        txt_descodificado[in] = letra_correspondente(msg_desencriptada[in]);  
    }
    
    //Cria um txt com os dados 
    FILE *file_descodificado;
    file_descodificado = fopen("mensagem-desencriptada.txt", "w");
    fprintf(file_descodificado, "%s", txt_descodificado);
    fclose(file_descodificado);
    
    return txt_descodificado;
}