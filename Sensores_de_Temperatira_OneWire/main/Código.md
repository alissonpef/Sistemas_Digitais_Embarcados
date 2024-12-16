Vamos revisar o exemplo e incluir as etapas completas da função **`fazScan`**, com a verificação se o sensor já foi detectado, o uso do `break`, e o **print** dos resultados detalhados.

Os dois sensores ainda têm endereços **`01010`** e **`01011`** (5 bits), e a função seguirá o fluxo completo.

---

### **Passo 1: Configuração inicial**
1. A função inicializa:
   - **`quantidade`**: 0 (nenhum sensor detectado).
   - **`v`**: Zera o buffer do endereço atual.
   - **`ultimo_conflito`**: -1 (nenhum conflito encontrado ainda).

2. O comando `SEARCH_ROM` é enviado para começar a busca.

---

### **Passo 2: Identificando o primeiro sensor**

#### **Bit 1 (posição 0):**
- Sensores enviam:
  - Sensor 1: `0`
  - Sensor 2: `0`
- Resultado:
  - **Normal:** `0`
  - **Complemento:** `1` → Sem conflito.
- Escolha: `0`.
- Atualiza o buffer: **`v = 0`**.

#### **Bit 2 (posição 1):**
- Sensores enviam:
  - Sensor 1: `1`
  - Sensor 2: `1`
- Resultado:
  - **Normal:** `1`
  - **Complemento:** `0` → Sem conflito.
- Escolha: `1`.
- Atualiza o buffer: **`v = 01`**.

#### **Bit 3 (posição 2):**
- Sensores enviam:
  - Sensor 1: `0`
  - Sensor 2: `0`
- Resultado:
  - **Normal:** `0`
  - **Complemento:** `1` → Sem conflito.
- Escolha: `0`.
- Atualiza o buffer: **`v = 010`**.

#### **Bit 4 (posição 3):**
- Sensores enviam:
  - Sensor 1: `1`
  - Sensor 2: `1`
- Resultado:
  - **Normal:** `1`
  - **Complemento:** `0` → Sem conflito.
- Escolha: `1`.
- Atualiza o buffer: **`v = 0101`**.

#### **Bit 5 (posição 4):**
- Sensores enviam:
  - Sensor 1: `0`
  - Sensor 2: `1`
- Resultado:
  - **Normal:** `0`
  - **Complemento:** `0` → Conflito.
- Escolha inicial: `0`.
- Atualiza o buffer: **`v = 01010`**.

---

### **Verificação e salvamento**
1. Verifica se o endereço **`01010`** já foi detectado:
   ```cpp
   bool ja_detectado = false;
   for (int i = 0; i < quantidade; i++) {
       if (memcmp(enderecos[i], v, 8) == 0) {
           ja_detectado = true;
           break;
       }
   }
   ```
   - O vetor `enderecos` está vazio, então **`ja_detectado = false`**.

2. Salva o endereço **`01010`**:
   ```cpp
   if (!ja_detectado) {
       for (int i = 0; i < 8; i++) {
           enderecos[quantidade][i] = v[i];
       }
       quantidade++;
   }
   ```
   - **`quantidade`** é incrementado para 1.

3. Imprime as informações do sensor:
   ```plaintext
   Sensor 1:
   Endereco completo: 01010
   ```

4. Atualiza **`ultimo_conflito = 4`** (posição do último conflito).

---

### **Passo 3: Identificando o segundo sensor**
1. Volta ao **bit 4** e escolhe o outro caminho:
   - Escolha: `1`.
   - Atualiza o buffer: **`v = 01011`**.

2. Verifica se o endereço **`01011`** já foi detectado:
   - Compara com `enderecos[0]` (**`01010`**).
   - Não é igual, então **`ja_detectado = false`**.

3. Salva o endereço **`01011`**:
   - Armazena em `enderecos[1]`.
   - **`quantidade`** é incrementado para 2.

4. Imprime as informações do sensor:
   ```plaintext
   Sensor 2:
   Endereco completo: 01011
   ```

---

### **Passo 4: Finalização**
1. Não há mais conflitos:
   - **`conflito_atual = -1`**.
   - O `break` é executado, encerrando o loop.

2. Imprime o total de sensores encontrados:
   ```plaintext
   Sensores encontrados: 2
   ```

---

### **Resumo final do resultado**
```plaintext
*** fazScan ***
Sensor 1:
Endereco completo: 01010

Sensor 2:
Endereco completo: 01011

Sensores encontrados: 2
```

