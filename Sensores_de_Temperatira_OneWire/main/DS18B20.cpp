#ifndef __DS18B20__
#define __DS18B20__

#include "DS18B20.h"
#include "delay.h"

void DS18B20::programa(void)
{
    onewire->reset();
    onewire->writeByte(0x4E);
    onewire->writeByte(0x00);
    onewire->writeByte(0x00);
    onewire->writeByte(0x7F);
}

void DS18B20::capturaBit(int posicao, char v[], int valor)
{
    unsigned char pbyte = posicao / 8;
    unsigned char pbit = posicao % 8;

    if (valor == 1)
        v[pbyte] |= (1 << pbit);
    else
        v[pbyte] &= ~(1 << pbit);
}

// Realiza uma varredura no barramento 1-Wire e detecta sensores conectados.
void DS18B20::fazScan(char enderecos[][8])
{
    printf("*** fazScan ***\n");

    int quantidade = 0;
    char v[8];
    uint8_t normal, complemento, escolha;
    int ultimo_conflito = -1; 
    
    while (true)
    {
        for (int i = 0; i < 8; i++)
        {
            v[i] = 0; // Zera o buffer para o próximo endereço.
        }

        onewire->reset();
        onewire->writeByte(SEARCH_ROM); // Envia comando para iniciar a busca.

        int conflito_atual = -1;

        for (int x = 0; x < 64; x++) // Itera por cada bit do endereço ROM.
        {
            normal = onewire->readBit();     
            complemento = onewire->readBit(); 

            if (normal == 0 && complemento == 0)
            {
                if (x == ultimo_conflito) // Mesmo ponto anterior
                {
                    escolha = 1; // Continua no próximo caminho.
                }
                else if (x > ultimo_conflito)
                {
                    escolha = 0;        // Novo caminho.
                    conflito_atual = x; // Registra o conflito atual.
                }
                onewire->escreve_bit(escolha); // Envia o bit escolhido.
                capturaBit(x, v, escolha);     // Atualiza o vetor com o bit escolhido.
            }
            else if (normal == 0 && complemento == 1)
            {
                onewire->escreve_bit(0); // Envia 0.
                capturaBit(x, v, 0);     // Atualiza o vetor com 0.
            }
            else if (normal == 1 && complemento == 0)
            {
                onewire->escreve_bit(1); // Envia 1.
                capturaBit(x, v, 1);     // Atualiza o vetor com 1.
            }
            else
            {
                break;
            }
        }

        // Verifica se o endereço já foi registrado
        bool ja_detectado = false;
        for (int i = 0; i < quantidade; i++)
        {
            if (memcmp(enderecos[i], v, 8) == 0) // Compara o endereço atual com os registrados.
            {
                ja_detectado = true; 
                break;
            }
        }

        if (!ja_detectado)
        {
            // Salva o endereço detectado.
            for (int i = 0; i < 8; i++)
            {
                enderecos[quantidade][i] = v[i];
            }
            quantidade++; 

            printf("\nSensor %d:\n", quantidade);
            printf("Codigo da Familia: %02X\n", v[0]);
            printf("Numero de Serie  : %02X %02X %02X %02X %02X %02X\n", v[6], v[5], v[4], v[3], v[2], v[1]);
            printf("CRC=             : %02X\n", v[7]);
            printf("Endereco completo: %02X %02X %02X %02X %02X %02X %02X %02X\n",
                   v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7]);
        }

        if (conflito_atual == -1)
        {
            // Não há mais conflitos, finaliza a busca.
            break;
        }

        ultimo_conflito = conflito_atual; // Atualiza o último conflito para a próxima iteração.
    }

    printf("Sensores encontrados: %d\n", quantidade);
}

void DS18B20::init(void)
{
    // So pode ser usada se existir apenas um escravo no barramento
    uint8_t serial[6], crc;
    printf("*** init***\n");

    delay_ms(500);

    printf("INIT\n");
    if (onewire->reset() == 0)
        printf("Detectou escravo na linha\n");
    else
        printf("Nao detectou escravo\n");

    onewire->writeByte(READ_ROM);
    printf("Codigo da Familia: %d\n", onewire->readByte());
    for (uint8_t x = 0; x < 6; x++)
        serial[x] = onewire->readByte();

    printf("Numero de Serie  : %d %d %d %d %d %d\n", serial[0], serial[1], serial[2], serial[3], serial[4], serial[5]);

    crc = onewire->readByte();
    printf("CRC=             : %d\n", crc);

    delay_ms(1000);
}

void DS18B20::init2(void)
{
    // Pode ser chamada se existir 1 ou mais escravos

    printf("*** init2***\n");
    delay_ms(500);

    printf("INIT\n");
    if (onewire->reset() == 0)
        printf("Detectou escravo na linha\n");
    else
        printf("Nao detectou escravo\n");

    delay_ms(1000);
}

// Usa o pino GPIO16 para fazer a comunicacao
DS18B20::DS18B20(gpio_num_t pino)
{
    DEBUG("DS18B20:Construtor\n");
    onewire = new ONEWIRE(pino);
}

char DS18B20::CRC(char end[])
{
    // calcula o CRC e retorna o resultado
    return 0;
}

float DS18B20::readTargetTemp(char vetor_64bits[])
{
    float temperatura;

    uint8_t a, b, inteira;
    float frac;

    onewire->reset();
    onewire->writeByte(MATCH_ROM);
    for (int x = 7; x >= 0; x--)
    {
        onewire->writeByte(vetor_64bits[x]);
    }
    onewire->writeByte(INICIA_CONVERSAO_TEMP);
    delay_ms(1000);
    onewire->reset();
    onewire->writeByte(MATCH_ROM);
    for (int x = 7; x >= 0; x--)
    {
        onewire->writeByte(vetor_64bits[x]);
    }
    onewire->writeByte(READ_TEMP_MEMORY);
    a = onewire->readByte();
    b = onewire->readByte();
    inteira = (b << 4) | (a >> 4);

    frac = ((a & 1) * (0.0625)) + (((a >> 1) & 1) * (0.125)) + (((a >> 2) & 1) * (0.25)) + (((a >> 3) & 1) * (0.5));

    temperatura = inteira + frac;
    return temperatura;
}
float DS18B20::readTemp(void)
{
    float temperatura;

    uint8_t a, b, inteira;
    float frac;

    onewire->reset();
    onewire->writeByte(SKIP_ROM);
    onewire->writeByte(INICIA_CONVERSAO_TEMP);
    delay_ms(1000);
    onewire->reset();
    onewire->writeByte(SKIP_ROM);
    onewire->writeByte(READ_TEMP_MEMORY);
    a = onewire->readByte();
    b = onewire->readByte();
    inteira = (b << 4) | (a >> 4);

    frac = ((a & 1) * (0.0625)) + (((a >> 1) & 1) * (0.125)) + (((a >> 2) & 1) * (0.25)) + (((a >> 3) & 1) * (0.5));

    temperatura = inteira + frac;
    return temperatura;
}

#endif
