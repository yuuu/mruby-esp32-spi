#include <mruby.h>
#include <mruby/data.h>
#include <mruby/variable.h>
#include <mruby/string.h>
#include <mruby/array.h>
#include <mruby/presym.h>

#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#define TAG ("mruby-esp32-spi")

static void mrb_spi_free(mrb_state *mrb, void *p);

static const struct mrb_data_type mrb_spi = {
  "mrb_mruby_esp32_spi", mrb_spi_free
};

typedef struct spi_t {
  mrb_state *mrb;
} spi_t;

static void
mrb_spi_free(mrb_state *mrb, void *p) {
  spi_t *spi = (spi_t *)p;

  mrb_free(mrb, p);
}

static mrb_value
mrb_spi_init(mrb_state *mrb, mrb_value self) {
  spi_t *spi = mrb_malloc(mrb, sizeof(spi_t));

  mrb_int id;
  mrb_bool given;
  mrb_sym knames[4] = { MRB_SYM(unit), MRB_SYM(mode), MRB_SYM(first_bit), MRB_SYM(frequency) };
  mrb_value kvalues[4] = { 0 };
  const mrb_kwargs kw = { 4, 0, knames, kvalues, NULL };
  mrb_get_args(mrb, "|i?:", &id, &given, &kw);

  if (!given) {
    ESP_LOGI(TAG, "initialize()");
  } else {
    ESP_LOGI(TAG, "initialize(%d)", id);
  }

  mrb_data_init(self, spi, &mrb_spi);

  return self;
}

void
mrb_mruby_esp32_spi_gem_init(mrb_state* mrb) {
  struct RClass *client_class = mrb_define_class(mrb, "SPI", mrb->object_class);

  mrb_define_method(mrb, client_class, "initialize", mrb_spi_init, MRB_ARGS_ANY());
}

void
mrb_mruby_esp32_spi_gem_final(mrb_state* mrb) {
}
