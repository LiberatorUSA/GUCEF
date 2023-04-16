# This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

require 'kaitai/struct/struct'

unless Gem::Version.new(Kaitai::Struct::VERSION) >= Gem::Version.new('0.9')
  raise "Incompatible Kaitai Struct Ruby API: 0.9 or later is required, but you have #{Kaitai::Struct::VERSION}"
end

class GucefPubsubMsgcontainer < Kaitai::Struct::Struct
  def initialize(_io, _parent = nil, _root = self)
    super(_io, _parent, _root)
    _read
  end

  def _read
    @header = GucefPubsubMsgcontainerHeader.new(@_io)
    @pubsubmsg = []
    i = 0
    begin
      _ = GucefPubsubMsg.new(@_io, self, @_root)
      @pubsubmsg << _
      i += 1
    end until _io.pos == (((_io.size - (msg_offset_idx_size * 4)) - 4) - 10)
    @footer = GucefPubsubMsgcontainerFooter.new(@_io)
    self
  end
  class GucefPubsubMsg < Kaitai::Struct::Struct
    def initialize(_io, _parent = nil, _root = self)
      super(_io, _parent, _root)
      _read
    end

    def _read
      if (_root.header.options & (1 << 1)) != 0
        case (_root.header.options & (1 << 2))
        when 0
          @msg_date_time = @_io.read_u4le
        else
          @msg_date_time = @_io.read_u8le
        end
      end
      if (_root.header.options & (1 << 3)) != 0
        @msgid = GucefCoreVariant.new(@_io)
      end
      if (_root.header.options & (1 << 4)) != 0
        @msg_index = GucefCoreVariant.new(@_io)
      end
      if (_root.header.options & (1 << 5)) != 0
        @primary_payload_included = GucefCoreVariant.new(@_io)
      end
      if (_root.header.options & (1 << 6)) != 0
        @nr_of_kv_pairs = @_io.read_u4le
      end
      if (_root.header.options & (1 << 6)) != 0
        @key_value_pairs = []
        (nr_of_kv_pairs).times { |i|
          @key_value_pairs << KvPair.new(@_io, self, @_root)
        }
      end
      if (_root.header.options & (1 << 7)) != 0
        @nr_of_metadata_kv_pairs = @_io.read_u4le
      end
      if (_root.header.options & (1 << 7)) != 0
        @metadata_key_value_pairs = []
        (nr_of_metadata_kv_pairs).times { |i|
          @metadata_key_value_pairs << KvPair.new(@_io, self, @_root)
        }
      end
      self
    end
    attr_reader :msg_date_time
    attr_reader :msgid
    attr_reader :msg_index
    attr_reader :primary_payload_included
    attr_reader :nr_of_kv_pairs
    attr_reader :key_value_pairs
    attr_reader :nr_of_metadata_kv_pairs
    attr_reader :metadata_key_value_pairs
  end
  class KvPair < Kaitai::Struct::Struct
    def initialize(_io, _parent = nil, _root = self)
      super(_io, _parent, _root)
      _read
    end

    def _read
      @key = GucefCoreVariant.new(@_io)
      @value = GucefCoreVariant.new(@_io)
      self
    end
    attr_reader :key
    attr_reader :value
  end
  def msg_offset_idx_size
    return @msg_offset_idx_size unless @msg_offset_idx_size.nil?
    _pos = @_io.pos
    @_io.seek((_io.size - 14))
    @msg_offset_idx_size = @_io.read_u4le
    @_io.seek(_pos)
    @msg_offset_idx_size
  end
  attr_reader :header
  attr_reader :pubsubmsg
  attr_reader :footer
end
