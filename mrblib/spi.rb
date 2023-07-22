class SPI
  def initialize(id = 0, **kwargs)
    _initialize(id, **kwargs)
  end

  def setmode(**kwargs)
    _setmode(**kwargs)
  end

  def write(obj)
    _write(obj)
  end

  def read(obj)
    _read(obj)
  end

  def transfer(obj, additional_read_bytes = 0)
    _transfer(obj, additional_read_bytes)
  end
end