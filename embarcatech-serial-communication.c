#include <stdio.h>
#include <stdlib.h>

// lib para a matriz de LEDs
#include "libs/matrix.h"

// libs para o display
#include "libs/ssd1306.h"
#include "libs/font.h"

// macros para os botões
#define A_BUTTON_GPIO_PIN 5
#define B_BUTTON_GPIO_PIN 6
#define DEBOUNCING_TIME_MS 500

// string recebida via serial
char serial_input[1024];

// Configurações da PIO
PIO pio;
uint offset;
uint sm;

// protótipos de funções
void led_matrix_setup();
void draw_matrix_if_number(char *serial_input);
uint32_t matrix_rgb(double r, double g, double b);
void display_setup(ssd1306_t *ssd);

int main()
{
  stdio_init_all();

  // inicializa a estrutura de dados que será enviada para o display
  ssd1306_t ssd;
  display_setup(&ssd);
  
  // inicializa a matriz de LEDs
  led_matrix_setup();
  
  while (true)
  {
    // lê entrada do monitor serial
    scanf("%1024s", serial_input);

    // se a entrada for um número, acende os LEDs correspondentes na matriz
    // se a entrada não for um número, apaga todos os LEDs da matriz
    draw_matrix_if_number(serial_input);

    // limpa o display antes de cada iteração
    ssd1306_fill(&ssd, false);

    // desenha uma string
    ssd1306_draw_string(&ssd, serial_input, 0, 0);

    // atualiza o display
    ssd1306_send_data(&ssd);

    sleep_ms(1000);
  }
}

void display_setup(ssd1306_t *ssd) {
  // inicializa a interface I2C com frequência de 400Khz
  i2c_init(I2C_PORT, 400 * 1000);

  // configura os pinos GPIO que serão os SDA e SCL da interface
  gpio_set_function(I2C_SDA_GPIO_PIN, GPIO_FUNC_I2C);
  gpio_set_function(I2C_SCL_GPIO_PIN, GPIO_FUNC_I2C);

  // adiciona resistores pull-up internos nos pinos SDA e SCL
  gpio_pull_up(I2C_SDA_GPIO_PIN);
  gpio_pull_up(I2C_SCL_GPIO_PIN);

  // inicializa o hardware do display
  ssd1306_init(ssd, WIDTH, HEIGHT, false, I2C_ADDRESS, I2C_PORT);

  // configura o display para receber os dados
  ssd1306_config(ssd);

  // envia os dados da estrutura para o display
  ssd1306_send_data(ssd);

  // limpa o display ao iniciar
  ssd1306_fill(ssd, false);
  ssd1306_send_data(ssd);
}

void led_matrix_setup() {
    // configura a PIO
  pio = pio0;
  set_sys_clock_khz(128000, false);
  offset = pio_add_program(pio, &led_matrix_program);
  sm = pio_claim_unused_sm(pio, true);
  led_matrix_program_init(pio, sm, offset, MATRIX_GPIO_PIN);

  // limpa a matriz de LEDs
  for (int j = 0; j < FRAME_DIMENSION; j++)
  {
    for (int k = 0; k < FRAME_DIMENSION; k++)
    {
      pio_sm_put_blocking(pio, sm, matrix_rgb(
        no_number[FRAME_DIMENSION - 1 - j][(j + 1) % 2 == 0 ? k : FRAME_DIMENSION - k - 1], 
        no_number[FRAME_DIMENSION - 1 - j][(j + 1) % 2 == 0 ? k : FRAME_DIMENSION - k - 1], 
        no_number[FRAME_DIMENSION - 1 - j][(j + 1) % 2 == 0 ? k : FRAME_DIMENSION - k - 1]
      ));
    }
  }
}

void draw_matrix_if_number(char *serial_input) {
  if (*serial_input >= '0' && *serial_input <= '9')
  {
    for (int i = 0; i < FRAME_DIMENSION; i++)
    {
      for (int j = 0; j < FRAME_DIMENSION; j++)
      { 
        pio_sm_put_blocking(pio, sm, matrix_rgb(0.0, 0.0, numbers[*serial_input-48][FRAME_DIMENSION - 1 - i][(i + 1) % 2 == 0 ? j : FRAME_DIMENSION - j - 1]));
      }
    }
  }
  else {
    for (int i = 0; i < FRAME_DIMENSION; i++)
    {
      for (int j = 0; j < FRAME_DIMENSION; j++)
      {
        pio_sm_put_blocking(pio, sm, matrix_rgb(
          no_number[FRAME_DIMENSION - 1 - i][(i + 1) % 2 == 0 ? j : FRAME_DIMENSION - j - 1],
          no_number[FRAME_DIMENSION - 1 - i][(i + 1) % 2 == 0 ? j : FRAME_DIMENSION - j - 1],
          no_number[FRAME_DIMENSION - 1 - i][(i + 1) % 2 == 0 ? j : FRAME_DIMENSION - j - 1]
        ));
      }
    }
  } 
}

uint32_t matrix_rgb(double r, double g, double b){
    unsigned char R, G, B;
    R = r * 255;
    G = g * 255;
    B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}