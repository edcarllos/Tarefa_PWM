#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "Tarefa_PWM.pio.h"

#define PINO_SERVO 22  // Define o pino do servomotor
#define FREQUENCIA_PWM 50 // Frequência do PWM em Hz
#define PERIODO_PWM_US 20000 // Período do PWM em microssegundos (20ms)
#define DIVISOR_RELOGIO 64.0f // Divisor do clock para controle de tempo
#define INCREMENTO 5 // Incremento em microssegundos para movimentação suave
#define ATRASO_MOVIMENTO 10 // Tempo de espera entre cada incremento (ms)

// Função para definir o pulso do servo conforme o ângulo desejado
void definir_pulso_servo(PIO pio, uint estado_maquina, uint32_t largura_pulso_us) {
    uint32_t relogio_sistema = clock_get_hz(clk_sys);
    uint32_t ciclos_pulso = (relogio_sistema / DIVISOR_RELOGIO) * largura_pulso_us / 1000000;
    uint32_t ciclos_total = (relogio_sistema / DIVISOR_RELOGIO) * 20000 / 1000000; // Período total de 20ms
    uint32_t ciclos_low = ciclos_total - ciclos_pulso; // Tempo LOW restante

    pio_sm_put_blocking(pio, estado_maquina, ciclos_pulso); // Envia tempo HIGH
    pio_sm_put_blocking(pio, estado_maquina, ciclos_low);   // Envia tempo LOW
}


int main() {
    stdio_init_all();
    sleep_ms(2000); // Pequeno atraso para estabilização

    PIO pio = pio0;
    uint estado_maquina = pio_claim_unused_sm(pio, true);
    uint deslocamento = pio_add_program(pio, &Tarefa_PWM_program);
    
    pio_sm_config configuracao = Tarefa_PWM_program_get_default_config(deslocamento);
    sm_config_set_clkdiv(&configuracao, DIVISOR_RELOGIO);
    sm_config_set_out_pins(&configuracao, PINO_SERVO, 1);
    sm_config_set_set_pins(&configuracao, PINO_SERVO, 1);
    pio_gpio_init(pio, PINO_SERVO);
    gpio_set_dir(PINO_SERVO, GPIO_OUT);

    pio_sm_init(pio, estado_maquina, deslocamento, &configuracao);
    pio_sm_set_enabled(pio, estado_maquina, true);

    // Movimentação para posições fixas com tempo de espera
    definir_pulso_servo(pio, estado_maquina, 2400); // Define para 180 graus
    sleep_ms(5000);
    definir_pulso_servo(pio, estado_maquina, 1470); // Define para 90 graus
    sleep_ms(5000);
    definir_pulso_servo(pio, estado_maquina, 500);  // Define para 0 graus
    sleep_ms(5000);

    // Movimentação suave entre 0 e 180 graus
    while (1) {
        // De 0 para 180 graus
        for (uint32_t pulso = 500; pulso <= 2400; pulso += INCREMENTO) {
            definir_pulso_servo(pio, estado_maquina, pulso);
            sleep_ms(ATRASO_MOVIMENTO);
        }

        sleep_ms(500); // Pequena pausa antes de inverter o movimento

        // De 180 para 0 graus
        for (uint32_t pulso = 2400; pulso >= 500; pulso -= INCREMENTO) {
            definir_pulso_servo(pio, estado_maquina, pulso);
            sleep_ms(ATRASO_MOVIMENTO);
        }

        sleep_ms(500); // Pequena pausa antes de reiniciar o ciclo
    }
}