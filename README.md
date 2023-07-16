# mruby-esp32-spi

SPI library for mruby-esp32.

## Installation

You need [esp-idf v5.0](https://docs.espressif.com/projects/esp-idf/en/release-v5.0/esp32/index.html) to use this mrbgems.

Add the line below to your `build_config.rb`:

```ruby
  conf.gem :github => 'yuuu/mruby-esp32-spi'
```

## Examples

```ruby
spi = SPI.new
spi = SPI.new(unit: 1, mode: 0, frequency:10_000_000, first_bit: SPI::MSB_FIRST)

spi.setmode(unit: 1, mode: 0, frequency:10_000_000, first_bit: SPI::MSB_FIRST)

spi.write(0x30, 0x20, 0x00)
spi.write("\x30\xa2")
spi.write(0x30)

data = spi.read( 32 )

s = spi.transfer([0x30, 0x20], 1)

```

## LICENSE

MIT License.
