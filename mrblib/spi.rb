class SPI
  MSB_FIRST = 0
  LSB_FIRST = 1

  attr_accessor :unit, :frequency, :mode, :first_bit
  attr_accessor :outputs
  attr_accessor :read_bytes

  def initialize(id = nil, **kwargs)
    self.unit = id || kwargs[:unit] || 0
    self.frequency = kwargs[:frequency] || 1_000_000
    self.mode = kwargs[:mode] || 0
    self.first_bit = kwargs[:first_bit] || MSB_FIRST

    __initialize(self.unit, self.frequency, self.mode, self.first_bit)
  end

  def setmode(**kwargs)
    self.unit = kwargs[:unit] || self.unit
    self.frequency = kwargs[:frequency] || self.frequency
    self.mode = kwargs[:mode] || self.mode
    self.first_bit = kwargs[:first_bit] || self.first_bit

    __setmode(self.unit, self.frequency, self.mode, self.first_bit)
  end

  def write(outputs)
    self.outputs =
      if outputs.is_a?(String)
        outputs
      elsif outputs.is_a?(Array)
        outputs.pack("c*")
      elsif outputs.is_a?(Integer)
        [outputs].pack("c*")
      else
        raise data_type_error
      end

    __write(self.outputs)
    nil
  end

  def read(read_bytes)
    self.read_bytes = read_bytes

    __read(read_bytes)
  end

  def transfer(outputs, additional_read_bytes = 0)
    self.outputs =
      if outputs.is_a?(String)
        outputs
      elsif outputs.is_a?(Array)
        outputs.pack("c*")
      elsif outputs.is_a?(Integer)
        [outputs].pack("c*")
      else
        raise data_type_error
      end

    additional_read_bytes.times { self.outputs += "\0" }

    __write(self.outputs)
    __read(self.outputs.size)
  end

  def data_type_error
    TypeError.new("wrong argument type (expected Integer or String or Array<String>)")
  end
end