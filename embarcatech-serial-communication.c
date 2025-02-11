#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "libs/ssd1306.h"
#include "libs/font.h"
#define I2C_PORT i2c1
#define I2C_SDA_GPIO_PIN 14
#define I2C_SCL_GPIO_PIN 15
#define I2C_ADDRESS 0x3C

int main()
{
  // inicializa a interface I2C com frequência de 400Khz
  i2c_init(I2C_PORT, 400 * 1000);

  // configura os pinos GPIO que serão os SDA e SCL da interface
  gpio_set_function(I2C_SDA_GPIO_PIN, GPIO_FUNC_I2C);
  gpio_set_function(I2C_SCL_GPIO_PIN, GPIO_FUNC_I2C);

  // adiciona resistores pull-up internos nos pinos SDA e SCL
  gpio_pull_up(I2C_SDA_GPIO_PIN);
  gpio_pull_up(I2C_SCL_GPIO_PIN);

  // inicializa a estrutura de dados que será enviada para o display
  ssd1306_t ssd;

  // inicializa o hardware do display
  ssd1306_init(&ssd, WIDTH, HEIGHT, false, I2C_ADDRESS, I2C_PORT);

  // configura o display para receber os dados
  ssd1306_config(&ssd);

  // envia os dados da estrutura para o display
  ssd1306_send_data(&ssd);

  // limpa o display ao iniciar
  ssd1306_fill(&ssd, false);
  ssd1306_send_data(&ssd);

  while (true)
  {
    // limpa o display antes de cada iteração
    ssd1306_fill(&ssd, false);

    // desenha a borda retangular
    //ssd1306_rect(&ssd, 0, 0, 128, 64, true, false);

    // desenha uma string
    ssd1306_draw_string(&ssd, "cause this is  thrilleeeeeeer thriller night", 0, 0);

    // atualiza o display
    ssd1306_send_data(&ssd);

    sleep_ms(1000);
  }
}