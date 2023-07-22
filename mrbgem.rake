MRuby::Gem::Specification.new('mruby-esp32-spi') do |spec|
  spec.license = 'MIT'
  spec.author  = 'yuuu'
  spec.summary = 'SPI class on ESP32'

  spec.add_dependency('mruby-pack', :core => 'mruby-pack')
end
