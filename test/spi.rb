assert('SPI.new') do
  spi = SPI.new()
  assert_equal(spi.unit, 0)
  assert_equal(spi.frequency, 1_000_000)
  assert_equal(spi.mode, 0)
  assert_equal(spi.first_bit, SPI::MSB_FIRST)

  spi = SPI.new(1)
  assert_equal(spi.unit, 1)
  assert_equal(spi.frequency, 1_000_000)
  assert_equal(spi.mode, 0)
  assert_equal(spi.first_bit, SPI::MSB_FIRST)

  spi = SPI.new(1, unit: 2, frequency: 2_000_000, mode: 1, first_bit: SPI::LSB_FIRST)
  assert_equal(spi.unit, 1)
  assert_equal(spi.frequency, 2_000_000)
  assert_equal(spi.mode, 1)
  assert_equal(spi.first_bit, SPI::LSB_FIRST)

  spi = SPI.new(unit: 2, frequency: 2_000_000, mode: 1, first_bit: SPI::LSB_FIRST)
  assert_equal(spi.unit, 2)
  assert_equal(spi.frequency, 2_000_000)
  assert_equal(spi.mode, 1)
  assert_equal(spi.first_bit, SPI::LSB_FIRST)

  spi = SPI.new(frequency: 3_000_000)
  assert_equal(spi.unit, 0)
  assert_equal(spi.frequency, 3_000_000)
  assert_equal(spi.mode, 0)
  assert_equal(spi.first_bit, SPI::MSB_FIRST)
end

assert('SPI#setmode') do
  spi = SPI.new()
  spi.setmode(unit: 2, frequency: 2_000_000, mode: 1, first_bit: SPI::LSB_FIRST)
  assert_equal(spi.unit, 2)
  assert_equal(spi.frequency, 2_000_000)
  assert_equal(spi.mode, 1)
  assert_equal(spi.first_bit, SPI::LSB_FIRST)

  spi = SPI.new()
  spi.setmode(frequency: 2_000_000)
  assert_equal(spi.unit, 0)
  assert_equal(spi.frequency, 2_000_000)
  assert_equal(spi.mode, 0)
  assert_equal(spi.first_bit, SPI::MSB_FIRST)
end

assert('SPI#write') do
  spi = SPI.new()
  ret = spi.write("writedata")
  assert_nil(ret)
  assert_equal("writedata", spi.outputs)

  ret = spi.write(0x12)
  assert_nil(ret)
  assert_equal("\x12", spi.outputs)

  ret = spi.write([0x34, 0x56])
  assert_nil(ret)
  assert_equal("\x34\x56", spi.outputs)

  assert_raise(TypeError) do
    spi.write(nil)
    assert_nil(ret)
  end
end

assert('SPI#read') do
  spi = SPI.new()
  ret = spi.read(64)
  assert_equal(64, spi.read_bytes)
end

assert('SPI#transfer') do
  spi = SPI.new()
  ret = spi.transfer("writedata")
  assert_equal("writedata", spi.outputs)

  ret = spi.transfer(0x12)
  assert_equal("\x12", spi.outputs)

  ret = spi.transfer([0x34, 0x56])
  assert_equal("\x34\x56", spi.outputs)

  ret = spi.transfer([0x34, 0x56], 3)
  assert_equal("\x34\x56\x00\x00\x00", spi.outputs)

  assert_raise(TypeError) do
    spi.write(nil)
    assert_nil(ret)
  end
end
