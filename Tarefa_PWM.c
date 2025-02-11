#include <stdio.h>

#include "Tarefa_PWM.pio.h"
#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "pico/stdlib.h"

#define SERVO_PIN 22
#define PWM_FREQUENCY 50
#define CLOCK_DIVIDER 64.0f

void set_servo_pulse(PIO pio, uint sm, uint32_t pulse_width_us) {
  uint32_t system_clock = clock_get_hz(clk_sys);
  uint32_t ticks = (system_clock / CLOCK_DIVIDER) * pulse_width_us / 1000000;
  pio_sm_put_blocking(pio, sm, ticks);
}

int main() {
  stdio_init_all();
  sleep_ms(2000);

  PIO pio = pio0;
  uint sm = pio_claim_unused_sm(pio, true);
  uint offset = pio_add_program(pio, &Tarefa_PWM_program);

  pio_sm_config c = Tarefa_PWM_program_get_default_config(offset);
  sm_config_set_clkdiv(&c, CLOCK_DIVIDER);
  sm_config_set_out_pins(&c, SERVO_PIN, 1);
  sm_config_set_set_pins(&c, SERVO_PIN, 1);
  pio_gpio_init(pio, SERVO_PIN);
  gpio_set_dir(SERVO_PIN, GPIO_OUT);

  pio_sm_init(pio, sm, offset, &c);
  pio_sm_set_enabled(pio, sm, true);

  while (1) {
    set_servo_pulse(pio, sm, 2400);  // 180 graus
    sleep_ms(5000);
    set_servo_pulse(pio, sm, 1470);  // 90 graus
    sleep_ms(5000);
    set_servo_pulse(pio, sm, 500);  // 0 graus
    sleep_ms(5000);
  }
}
