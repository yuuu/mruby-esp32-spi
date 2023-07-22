#include <mruby.h>
#include <mruby/data.h>
#include <mruby/variable.h>
#include <mruby/string.h>
#include <mruby/array.h>
#include <mruby/presym.h>

#include <stdio.h>
#include <string.h>

#ifdef ESP_PLATFORM
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#else
#define ESP_LOGI(tag, fmt, ...)  printf(fmt, __VA_ARGS__)
#endif

#define TAG ("mruby-esp32-spi")

typedef struct spi_t {
  mrb_int frequency;
  mrb_int unit;
  mrb_int mode;
  mrb_int first_bit;
  mrb_state *mrb;
} spi_t;

static void mrb_spi_free(mrb_state *mrb, void *p);

static const struct mrb_data_type mrb_spi = {
  "mrb_mruby_esp32_spi", mrb_spi_free
};

static void
mrb_spi_free(mrb_state *mrb, void *p) {
  spi_t *spi = (spi_t *)p;

  mrb_free(mrb, p);
}

static mrb_value
mrb_spi_init(mrb_state *mrb, mrb_value self) {
  spi_t *spi = mrb_malloc(mrb, sizeof(spi_t));

  mrb_get_args(mrb, "iiii", &spi->unit, &spi->frequency, &spi->mode, &spi->first_bit);

  ESP_LOGI(
    TAG,
    "initialize(unit: %d, mode: %d, first_bit: %d, frequency: %d)",
    spi->unit, spi->mode, spi->first_bit, spi->frequency
  );

  mrb_data_init(self, spi, &mrb_spi);

  return self;
}

static mrb_value
mrb_spi_setmode(mrb_state *mrb, mrb_value self) {
  spi_t *spi = (spi_t *) DATA_PTR(self);

  mrb_get_args(mrb, "iiii", &spi->unit, &spi->frequency, &spi->mode, &spi->first_bit);

  ESP_LOGI(
    TAG,
    "setmode(unit: %d, mode: %d, first_bit: %d, frequency: %d)",
    spi->unit, spi->mode, spi->first_bit, spi->frequency
  );

  return self;
}

static mrb_value
mrb_spi_read(mrb_state *mrb, mrb_value self) {
  spi_t *spi = (spi_t *) DATA_PTR(self);
  mrb_int read_bytes = 0;

  mrb_get_args(mrb, "i", &read_bytes);
  ESP_LOGI(TAG, "read(%d)", read_bytes);

  return self;
}

static mrb_value
mrb_spi_write(mrb_state *mrb, mrb_value self) {
  spi_t *spi = (spi_t *) DATA_PTR(self);
  mrb_value data;

  mrb_get_args(mrb, "S", &data);

  return self;
}

void
mrb_mruby_esp32_spi_gem_init(mrb_state* mrb) {
  struct RClass *client_class = mrb_define_class(mrb, "SPI", mrb->object_class);

  mrb_define_method(mrb, client_class, "__initialize", mrb_spi_init, MRB_ARGS_REQ(4));
  mrb_define_method(mrb, client_class, "__setmode", mrb_spi_setmode, MRB_ARGS_REQ(4));
  mrb_define_method(mrb, client_class, "__read", mrb_spi_read, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, client_class, "__write", mrb_spi_write, MRB_ARGS_REQ(1));
}

void
mrb_mruby_esp32_spi_gem_final(mrb_state* mrb) {
}
