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

// #define ESP_LOGI(tag, fmt, ...)  printf(fmt, __VA_ARGS__)

#define TAG ("mruby-esp32-spi")

enum {
  KWARGS_UNIT = 0,
  KWARGS_MODE,
  KWARGS_FIRST_BIT,
  KWARGS_FREQUENCY,
  KWARGS_NUM,
};

typedef struct spi_t {
  mrb_int id;
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

static const mrb_sym mrb_spi_setmode_kw_names[KWARGS_NUM] = {
  MRB_SYM(unit),
  MRB_SYM(mode),
  MRB_SYM(first_bit),
  MRB_SYM(frequency)
};

static void
mrb_spi_set_keyword_values(spi_t *spi, mrb_value *kw_values) {
  spi->unit = mrb_undef_p(kw_values[KWARGS_UNIT]) ? 0 : mrb_fixnum(kw_values[KWARGS_UNIT]);
  spi->mode = mrb_undef_p(kw_values[KWARGS_MODE]) ? 1 : mrb_fixnum(kw_values[KWARGS_MODE]);
  spi->first_bit = mrb_undef_p(kw_values[KWARGS_FIRST_BIT]) ? 2 : mrb_fixnum(kw_values[KWARGS_FIRST_BIT]);
  spi->frequency = mrb_undef_p(kw_values[KWARGS_FREQUENCY]) ? 3 : mrb_fixnum(kw_values[KWARGS_FREQUENCY]);
}

static void
mrb_spi_get_init_args(mrb_state *mrb, spi_t *spi) {
  mrb_bool given;
  mrb_value kw_values[KWARGS_NUM];
  const mrb_kwargs kw = { KWARGS_NUM, 0, mrb_spi_setmode_kw_names, kw_values, NULL };

  mrb_get_args(mrb, "|i?:", &spi->id, &given, &kw);

  if (!given) { spi->id = 0; }
  mrb_spi_set_keyword_values(spi, kw_values);
}

static void
mrb_spi_get_setmode_args(mrb_state *mrb, spi_t *spi) {
  mrb_value kw_values[KWARGS_NUM];
  const mrb_kwargs kw = { KWARGS_NUM, 0, mrb_spi_setmode_kw_names, kw_values, NULL };

  mrb_get_args(mrb, ":", &kw);
  mrb_spi_set_keyword_values(spi, kw_values);
}

static void
mrb_spi_free(mrb_state *mrb, void *p) {
  spi_t *spi = (spi_t *)p;

  mrb_free(mrb, p);
}

static mrb_value
mrb_spi_init(mrb_state *mrb, mrb_value self) {
  spi_t *spi = mrb_malloc(mrb, sizeof(spi_t));


  mrb_spi_get_init_args(mrb, spi);
  ESP_LOGI(
    TAG,
    "initialize(%d, unit: %d, mode: %d, first_bit: %d, frequency: %d)",
    spi->id, spi->unit, spi->mode, spi->first_bit, spi->frequency
  );

  mrb_data_init(self, spi, &mrb_spi);

  return self;
}

static mrb_value
mrb_spi_setmode(mrb_state *mrb, mrb_value self) {
  spi_t *spi = (spi_t *) DATA_PTR(self);

  mrb_spi_get_setmode_args(mrb, spi);
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
  mrb_value obj;
  mrb_value str;

  mrb_get_args(mrb, "o", &obj);
  switch (mrb_type(obj)) {
  case MRB_TT_INTEGER:
    ESP_LOGI(TAG, "write(%d)", mrb_fixnum(obj));
    break;
  case MRB_TT_ARRAY:
    RARRAY_LEN(obj);
    str = mrb_ary_join(mrb, obj, mrb_str_new_cstr(mrb, ", "));
    ESP_LOGI(TAG, "write([%s])", mrb_string_cstr(mrb, str));
    break;
  case MRB_TT_STRING:
    ESP_LOGI(TAG, "write(\"%s\")", mrb_str_to_cstr(mrb, obj));
    break;
  default:
    mrb_raise(mrb, E_TYPE_ERROR, "wrong argument type (expected Integer or String or Array<String>)");
    break;
  }

  return self;
}

static mrb_value
mrb_spi_transfer(mrb_state *mrb, mrb_value self) {
  spi_t *spi = (spi_t *) DATA_PTR(self);
  mrb_bool given;
  mrb_int additional_read_bytes = 0;
  mrb_value obj;
  mrb_value str;

  mrb_get_args(mrb, "o|i", &obj, &additional_read_bytes, &given);
  if (!given) { additional_read_bytes = 0; }

  switch (mrb_type(obj)) {
  case MRB_TT_INTEGER:
    ESP_LOGI(TAG, "transfer(%d, %d)", mrb_fixnum(obj), additional_read_bytes);
    break;
  case MRB_TT_ARRAY:
    RARRAY_LEN(obj);
    str = mrb_ary_join(mrb, obj, mrb_str_new_cstr(mrb, ", "));
    ESP_LOGI(TAG, "transfer([%s], %d)", mrb_string_cstr(mrb, str), additional_read_bytes);
    break;
  case MRB_TT_STRING:
    ESP_LOGI(TAG, "transfer(\"%s\", %d)", mrb_str_to_cstr(mrb, obj), additional_read_bytes);
    break;
  default:
    mrb_raise(mrb, E_TYPE_ERROR, "wrong argument type (expected Integer or String or Array<String>)");
    break;
  }

  return self;
}

void
mrb_mruby_esp32_spi_gem_init(mrb_state* mrb) {
  struct RClass *client_class = mrb_define_class(mrb, "SPI", mrb->object_class);

  mrb_define_method(mrb, client_class, "_initialize", mrb_spi_init, MRB_ARGS_ANY());
  mrb_define_method(mrb, client_class, "_setmode", mrb_spi_setmode, MRB_ARGS_ANY());
  mrb_define_method(mrb, client_class, "_read", mrb_spi_read, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, client_class, "_write", mrb_spi_write, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, client_class, "_transfer", mrb_spi_transfer, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
}

void
mrb_mruby_esp32_spi_gem_final(mrb_state* mrb) {
}
