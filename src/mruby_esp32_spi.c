#include <mruby.h>
#include <mruby/data.h>
#include <mruby/variable.h>
#include <mruby/string.h>
#include <mruby/array.h>
#include <mruby/presym.h>

#include <stdio.h>
#include <string.h>

#if ESP_PLATFORM
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "esp_log.h"
#else
#define ESP_LOGI(tag, fmt, ...)  printf(fmt, __VA_ARGS__)
#endif

#ifdef CONFIG_IDF_TARGET_ESP32
#define PIN_NUM_MISO 19
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  18
#define PIN_NUM_CS   5
#endif

#define TAG ("mruby-esp32-spi")

typedef struct spi_t {
  mrb_int frequency;
  mrb_int unit;
  mrb_int mode;
  mrb_int first_bit;
  mrb_state *mrb;
#ifdef ESP_PLATFORM
  spi_device_handle_t *device_handle;
#endif
} spi_t;

static void mrb_spi_free(mrb_state *mrb, void *p);

static const struct mrb_data_type mrb_spi = {
  "mrb_mruby_esp32_spi", mrb_spi_free
};

#if ESP_PLATFORM
static void
mrb_spi_pre_transfer_callback(spi_transaction_t *t) {
}
#endif

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

#ifdef ESP_PLATFORM
  esp_err_t ret;
  spi_device_handle_t device_handle;
  spi_bus_config_t bus_config = {
    .miso_io_num = PIN_NUM_MISO,
    .mosi_io_num = PIN_NUM_MOSI,
    .sclk_io_num = PIN_NUM_CLK,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .max_transfer_sz = 0
  };
  spi_device_interface_config_t device_config = {
    .clock_speed_hz = spi->frequency,
    .mode = spi->mode,
    .spics_io_num = PIN_NUM_CS,
    .queue_size = 7,
    .pre_cb = mrb_spi_pre_transfer_callback,
  };

  ret = spi_bus_initialize(VSPI_HOST, &bus_config, SPI_DMA_CH_AUTO);
  ESP_ERROR_CHECK(ret);

  ret = spi_bus_add_device(VSPI_HOST, &device_config, &device_handle);
  ESP_ERROR_CHECK(ret);

  spi->device_handle = device_handle;
#endif

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

#ifdef ESP_PLATFORM
#endif

  return self;
}

static mrb_value
mrb_spi_read(mrb_state *mrb, mrb_value self) {
  spi_t *spi = (spi_t *) DATA_PTR(self);
  mrb_int read_bytes = 0;

  mrb_get_args(mrb, "i", &read_bytes);
  ESP_LOGI(TAG, "read(%d)", read_bytes);

#ifdef ESP_PLATFORM
#endif

  return self;
}

static mrb_value
mrb_spi_write(mrb_state *mrb, mrb_value self) {
  spi_t *spi = (spi_t *) DATA_PTR(self);
  char *data;
  mrb_int len;

  mrb_get_args(mrb, "s", &data, &len);
  ESP_LOGI(TAG, "write(%s)", data);

#ifdef ESP_PLATFORM
  esp_err_t ret;
  spi_transaction_t transaction = {
    .length = len * 8,
    .tx_buffer = data,
    .user = (void*)0,
  };
  ret = spi_device_polling_transmit(spi->device_handle, &transaction);
  assert(ret == ESP_OK);
#endif

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
