# Sistemas Digitais Embarcados

**Professor:** Fábio Rodrigues de la Rocha

## 📚 Ementa

Este repositório contém materiais, exercícios e exemplos práticos relacionados à disciplina de **Sistemas Digitais Embarcados**, com foco no projeto de hardware e software utilizando microcontroladores e sistemas de tempo real.

### Conteúdo abordado:

1. **Projeto de Hardware com Microcontroladores**  
   - Integração com dispositivos de armazenamento: RAM, Flash e IDE.  
   - Interfaces com periféricos como displays LCD e teclado.  

2. **Interfaces com Sistemas Analógicos e Comunicação Digital**  
   - Redes de comunicação: CAN, LIN, RS485 e I2C.  
   - Redes sem fio: Wi-Fi e Bluetooth.  

3. **Desenvolvimento de Software de Tempo Real**  
   - Diagramas de estado e programação para sistemas embarcados.  
   - Uso da linguagem C em sistemas operacionais de tempo real (uCOS II).  

4. **Práticas Avançadas em Sistemas Embarcados**  
   - Particionamento entre software e hardware.  
   - Uso avançado de memórias e interfaces seriais (I2C, SPI).  
   - Desenvolvimento e integração de dispositivos de comunicação sem fio.  

## 🎯 Objetivos da Disciplina

- **Objetivo Geral:** Capacitar os estudantes a projetar e desenvolver sistemas eletrônicos baseados em microcontroladores.  
- **Objetivos Específicos:**  
  - Identificar os requisitos de projetos de sistemas embarcados.  
  - Realizar o particionamento entre hardware e software.  
  - Escolher e utilizar ferramentas adequadas de desenvolvimento.  
  - Implementar sistemas de tempo real e compreender suas aplicações.

## 💂️ Estrutura do Repositório

- **/Sensores_de_Temperatura_OneWire**: Projetos relacionados à medição de temperatura utilizando o protocolo OneWire.  
- **/Trena_Digital**: Projeto de uma trena digital utilizando sensores embarcados.  
- **/Introdução**: Trabalhos introdutórios realizados na disciplina.

## 🛠️ Ferramentas Recomendadas

- **Linguagem de Programação:** C e C++.
- **IDE:** ESP-IDF (Espressif IoT Development Framework).
- **Protocolos de Comunicação:** CAN, LIN, RS485, I2C, Wi-Fi e Bluetooth.
- **Sistemas Operacionais:** FreeRTOS ou outros compatíveis com ESP-IDF.

## 🔧 Como Utilizar

1. **Clonar o Repositório:**

   ```bash
   git clone https://github.com/alissonpef/Sistemas_Digitais_Embarcados.git
   ```

2. **Configurar o Ambiente ESP-IDF:** Certifique-se de que o ESP-IDF está instalado e configurado no seu sistema. Para instruções, consulte a [documentação oficial](https://docs.espressif.com/projects/esp-idf/en/latest/).

3. **Navegar pelos Diretórios:** Explore os diretórios para encontrar os materiais de estudo e projetos relacionados.

4. **Compilar e Executar os Projetos:** Utilize as ferramentas de compilação integradas no ESP-IDF. Por exemplo:

   ```bash
   idf.py build
   idf.py flash
   idf.py monitor
   ```

5. **Implementar no Hardware:** Certifique-se de que o dispositivo ESP32 ou outro compatível está conectado corretamente e use os comandos acima para gravar o firmware.

## 📢 Contato

Dúvidas? Entre em contato comigo por e-mail:

- **Alisson Pereira Ferreira:** [alissonpef@gmail.com](mailto:alissonpef@gmail.com)

Aproveite o aprendizado! 🚀

