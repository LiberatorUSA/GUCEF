# This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

require 'kaitai/struct/struct'

unless Gem::Version.new(Kaitai::Struct::VERSION) >= Gem::Version.new('0.9')
  raise "Incompatible Kaitai Struct Ruby API: 0.9 or later is required, but you have #{Kaitai::Struct::VERSION}"
end

class GucefPubsubMsgcontainerFooter < Kaitai::Struct::Struct
  def initialize(_io, _parent = nil, _root = self)
    super(_io, _parent, _root)
    _read
  end

  def _read
    @msg_offset_idx = []
    (mois).times { |i|
      @msg_offset_idx << @_io.read_u4le
    }
    @msg_offset_idx_size = @_io.read_u4le
    @magic_text = @_io.read_bytes(10)
    raise Kaitai::Struct::ValidationNotEqualError.new([80, 85, 66, 83, 85, 66, 77, 83, 71, 83].pack('C*'), magic_text, _io, "/seq/2") if not magic_text == [80, 85, 66, 83, 85, 66, 77, 83, 71, 83].pack('C*')
    self
  end
  def mois
    return @mois unless @mois.nil?
    _pos = @_io.pos
    @_io.seek((_io.size - 14))
    @mois = @_io.read_u4le
    @_io.seek(_pos)
    @mois
  end
  attr_reader :msg_offset_idx
  attr_reader :msg_offset_idx_size
  attr_reader :magic_text
end
