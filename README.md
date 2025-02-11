# Controle de Servo Motor com Raspberry Pi Pico e PIO

## Descrição do Projeto
Este projeto implementa o controle de um **servo motor** utilizando o **Raspberry Pi Pico** e a funcionalidade **PIO (Programmable I/O)** para geração precisa do sinal PWM. O código permite movimentar o servo entre três posições fixas e também realizar uma movimentação suave entre 0 e 180 graus.

## Estrutura do Repositório
```
TAREFA_PWM/
├── .vscode/                 # Configuração do ambiente VSCode
├── build/                   # Diretório de compilação
├── .gitignore               # Arquivo para ignorar arquivos desnecessários no Git
├── CMakeLists.txt           # Configuração do CMake para compilação
├── diagram.json             # Configuração do Wokwi
├── pico_sdk_import.cmake    # Importação do Pico SDK
├── README.md                # Documentação do projeto
├── Tarefa_PWM.c             # Código principal em C
├── Tarefa_PWM.pio           # Código PIO para geração do PWM
└── wokwi.toml               # Configuração para simulação no Wokwi
```

## Requisitos
- **Placa:** Raspberry Pi Pico W
- **SDK:** Pico SDK (versão 2.1.0 ou superior)
- **Ferramentas:** CMake, Make, Toolchain ARM
- **Simulador (Opcional):** Wokwi

## Instalação e Configuração
### 1. Clonar o repositório
```sh
git clone https://github.com/seu_usuario/TAREFA_PWM.git
cd TAREFA_PWM
```
### 2. Configurar o ambiente de desenvolvimento
Certifique-se de ter o **Pico SDK** instalado corretamente:
```sh
export PICO_SDK_PATH="/caminho/para/pico-sdk"
```
### 3. Compilar o código
```sh
mkdir build
cd build
cmake ..
make -j4
```
### 4. Gravar na placa
Coloque o **Pico em modo BOOTSEL** e copie o arquivo **.uf2** gerado:
```sh
cp Tarefa_PWM.uf2 /media/$USER/RPI-RP2/
```

## Funcionamento do Código
1. **Inicialização**: Configura o PIO e define a GPIO 22 como saída PWM.
2. **Movimentação fixa**: O servo se move para 0º, 90º e 180º, com pausa de 5 segundos.
3. **Movimentação suave**: O servo se desloca gradualmente entre 0º e 180º, com incremento de 5µs a cada 10ms.

## Código PIO (Tarefa_PWM.pio)
O código **Tarefa_PWM.pio** gera um **PWM preciso** para controlar o servo motor.

## Simulação no Wokwi
Para testar no Wokwi, basta abrir o projeto e rodar a simulação:
```sh
wokwi-cli diagram.json
```

## Autor
Desenvolvido por **Edcarllos Santos** no contexto do programa **Embarcatech**.
