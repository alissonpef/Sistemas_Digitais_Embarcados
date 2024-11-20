#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_system.h"
#include "esp_timer.h"
#include "esp_log.h"

#define PINO_TRIG 4 // Saída para enviar o pulso ultrassônico
#define PINO_ECHO 2 // Entrada para receber o sinal de eco
#define UART_NUM UART_NUM_0
#define BUF_SIZE (1024)

float valor_calibrado = 0.00;
float distancia_atual = 0.00;
volatile bool program_running = true;
SemaphoreHandle_t xMutex = NULL;
TaskHandle_t teste;

// Inicializa o sensor ultrassônico
void init_hcsr04()
{
    gpio_set_direction(PINO_TRIG, GPIO_MODE_OUTPUT);
    gpio_set_direction(PINO_ECHO, GPIO_MODE_INPUT);
}

// Configuração da UART
void uart_setup()
{
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
    uart_param_config(UART_NUM, &uart_config);
    uart_driver_install(UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 0, NULL, 0);
}

// Função para medir a distância em centímetros
float medir_distancia()
{
    gpio_set_level(PINO_TRIG, 1); // Envia um pulso de 10us
    esp_rom_delay_us(10);
    gpio_set_level(PINO_TRIG, 0); // Desliga o pulso

    while (gpio_get_level(PINO_ECHO) == 0)
    {
    }
    float start_time = esp_timer_get_time();

    while (gpio_get_level(PINO_ECHO) == 1)
    {
    }
    float end_time = esp_timer_get_time();

    // Convertemos microsegundos em segundos, multiplicar pela velocidade do som e dividimos por 2 (ida e volta)
    float distancia = (((end_time - start_time) / 1000000) * 340) / 2;

    return distancia * 100; // Converte para centimetros
}

// Exibe o menu na tela
void exibir_menu()
{
    printf("\033[2J\033[H");
    printf("                    MENU\n\n");
    printf("Para realizar a calibragem você precisa instalar o\n");
    printf("sensor na posição final dele. Após, entrar com a\n");
    printf("opção 1 no menu.\n");
    printf("[1] - Captura a altura para calibragem.\n");
    printf("[2] - Mostrar altura atual\n");
    printf("[0] - Reinicia\n");
    printf("\n");
}

// Tarefa para exibir o menu e capturar a escolha do usuário
void menu_task(void *arg)
{
    uint8_t data[2];
    int escolha = -1;
    float atual = 0;

    exibir_menu();
    printf("Distância Calibrada: %.2f cm\n", valor_calibrado);
    printf("Altura atual: %.2f cm\n", distancia_atual);

    while (program_running)
    {
        int len = uart_read_bytes(UART_NUM, data, 2, portMAX_DELAY);

        if (len > 0)
        {
            escolha = data[0] - '0'; // Converte o caractere de ASCII para inteiro
        }

        if (escolha >= 0 && escolha <= 9)
        {
            switch (escolha)
            {
            case 0:
                program_running = false;
                break;
            case 1:
                xSemaphoreTake(xMutex, portMAX_DELAY);
                valor_calibrado = medir_distancia();
                distancia_atual = 0.00;
                xSemaphoreGive(xMutex);

                exibir_menu();
                printf("Distância Calibrada: %.2f cm\n", valor_calibrado);
                printf("Altura atual: %.2f cm\n\n", distancia_atual);
                break;
            case 2:
                atual = medir_distancia();
                xSemaphoreTake(xMutex, portMAX_DELAY);
                distancia_atual = valor_calibrado - atual;
                if (distancia_atual < 0)
                {
                    distancia_atual = 0;
                }
                xSemaphoreGive(xMutex);

                exibir_menu();
                printf("Distância Calibrada: %.2f cm\n", valor_calibrado);
                printf("Altura atual: %.2f cm\n", distancia_atual);
                break;
            default:
                printf("\nOpção inválida. Tente novamente.\n");
                break;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(250));
    }
    vTaskDelete(teste);
}

// Tarefa para medir a distância continuamente
void sensor_task(void *arg)
{
    while (program_running)
    {
        float distancia = medir_distancia();
        if (distancia > 0)
        {
            xSemaphoreTake(xMutex, portMAX_DELAY);
            distancia_atual = distancia;
            xSemaphoreGive(xMutex);

            printf("Distância atual medida: %.2f cm\n", distancia);
            fflush(stdout);
        }
        vTaskDelay(pdMS_TO_TICKS(2500));
    }
}

void app_main(void)
{
    init_hcsr04();
    uart_setup();

    xMutex = xSemaphoreCreateMutex();
    if (xMutex == NULL)
    {
        printf("Erro ao criar mutex\n");
        return;
    }

    xTaskCreatePinnedToCore(menu_task, "menu_task", 8192, NULL, 5, &teste, 0);
    xTaskCreatePinnedToCore(sensor_task, "sensor_task", 8192, NULL, 5, NULL, 1);
}
