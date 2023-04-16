# This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

require 'kaitai/struct/struct'

unless Gem::Version.new(Kaitai::Struct::VERSION) >= Gem::Version.new('0.9')
  raise "Incompatible Kaitai Struct Ruby API: 0.9 or later is required, but you have #{Kaitai::Struct::VERSION}"
end

class GucefPubsubMsgcontainerHeader < Kaitai::Struct::Struct
  def initialize(_io, _parent = nil, _root = self)
    super(_io, _parent, _root)
    _read
  end

  def _read
    @magic_text = @_io.read_bytes(10)
    raise Kaitai::Struct::ValidationNotEqualError.new([80, 85, 66, 83, 85, 66, 77, 83, 71, 83].pack('C*'), magic_text, _io, "/seq/0") if not magic_text == [80, 85, 66, 83, 85, 66, 77, 83, 71, 83].pack('C*')
    @version = @_io.read_u1
    @options = @_io.read_u4le
    self
  end
  attr_reader :magic_text
  attr_reader :version
  attr_reader :options
end
