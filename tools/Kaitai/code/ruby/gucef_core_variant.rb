# This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

require 'kaitai/struct/struct'

unless Gem::Version.new(Kaitai::Struct::VERSION) >= Gem::Version.new('0.9')
  raise "Incompatible Kaitai Struct Ruby API: 0.9 or later is required, but you have #{Kaitai::Struct::VERSION}"
end

class GucefCoreVariant < Kaitai::Struct::Struct

  GUCEF_DATATYPES = {
    1 => :gucef_datatypes_uint8,
    2 => :gucef_datatypes_int8,
    3 => :gucef_datatypes_uint16,
    5 => :gucef_datatypes_int16,
    7 => :gucef_datatypes_uint32,
    9 => :gucef_datatypes_int32,
    11 => :gucef_datatypes_uint64,
    13 => :gucef_datatypes_int64,
    15 => :gucef_datatypes_float32,
    17 => :gucef_datatypes_float64,
    21 => :gucef_datatypes_ascii_string,
    22 => :gucef_datatypes_utf8_string,
    23 => :gucef_datatypes_utf16_le_string,
    24 => :gucef_datatypes_utf16_be_string,
    25 => :gucef_datatypes_utf32_string,
    30 => :gucef_datatypes_boolean_int32,
    31 => :gucef_datatypes_boolean_ascii_string,
    32 => :gucef_datatypes_boolean_utf8_string,
    40 => :gucef_datatypes_binary_blob,
    41 => :gucef_datatypes_binary_bsob,
    50 => :gucef_datatypes_datetime_iso8601_ascii_string,
    51 => :gucef_datatypes_datetime_iso8601_utf8_string,
    55 => :gucef_datatypes_le_timestamp_in_secs_since_unix_epoch,
    56 => :gucef_datatypes_be_timestamp_in_secs_since_unix_epoch,
    57 => :gucef_datatypes_le_timestamp_in_ms_since_unix_epoch,
    58 => :gucef_datatypes_be_timestamp_in_ms_since_unix_epoch,
  }
  I__GUCEF_DATATYPES = GUCEF_DATATYPES.invert
  def initialize(_io, _parent = nil, _root = self)
    super(_io, _parent, _root)
    _read
  end

  def _read
    @type_id = Kaitai::Struct::Stream::resolve_enum(GUCEF_DATATYPES, @_io.read_u1)
    case type_id
    when :gucef_datatypes_datetime_iso8601_ascii_string
      @payload = GucefCoreDynamic.new(@_io)
    when :gucef_datatypes_utf16_le_string
      @payload = GucefCoreDynamic.new(@_io)
    when :gucef_datatypes_int16
      @payload = GucefInt16.new(@_io, self, @_root)
    when :gucef_datatypes_float32
      @payload = GucefFloat32.new(@_io, self, @_root)
    when :gucef_datatypes_ascii_string
      @payload = GucefCoreDynamic.new(@_io)
    when :gucef_datatypes_uint16
      @payload = GucefUint16.new(@_io, self, @_root)
    when :gucef_datatypes_uint64
      @payload = GucefUint64.new(@_io, self, @_root)
    when :gucef_datatypes_int32
      @payload = GucefInt32.new(@_io, self, @_root)
    when :gucef_datatypes_uint8
      @payload = GucefUint8.new(@_io, self, @_root)
    when :gucef_datatypes_uint32
      @payload = GucefUint32.new(@_io, self, @_root)
    when :gucef_datatypes_float64
      @payload = GucefFloat64.new(@_io, self, @_root)
    when :gucef_datatypes_be_timestamp_in_secs_since_unix_epoch
      @payload = GucefBeTimestampInSecsSinceUnixEpoch.new(@_io, self, @_root)
    when :gucef_datatypes_be_timestamp_in_ms_since_unix_epoch
      @payload = GucefBeTimestampInMsSinceUnixEpoch.new(@_io, self, @_root)
    when :gucef_datatypes_utf32_string
      @payload = GucefCoreDynamic.new(@_io)
    when :gucef_datatypes_boolean_utf8_string
      @payload = GucefCoreDynamic.new(@_io)
    when :gucef_datatypes_binary_blob
      @payload = GucefCoreDynamic.new(@_io)
    when :gucef_datatypes_le_timestamp_in_ms_since_unix_epoch
      @payload = GucefLeTimestampInMsSinceUnixEpoch.new(@_io, self, @_root)
    when :gucef_datatypes_binary_bsob
      @payload = GucefBsob.new(@_io, self, @_root)
    when :gucef_datatypes_boolean_ascii_string
      @payload = GucefCoreDynamic.new(@_io)
    when :gucef_datatypes_utf8_string
      @payload = GucefCoreDynamic.new(@_io)
    when :gucef_datatypes_int8
      @payload = GucefInt8.new(@_io, self, @_root)
    when :gucef_datatypes_boolean_int32
      @payload = GucefBooleanInt32.new(@_io, self, @_root)
    when :gucef_datatypes_le_timestamp_in_secs_since_unix_epoch
      @payload = GucefLeTimestampInSecsSinceUnixEpoch.new(@_io, self, @_root)
    when :gucef_datatypes_datetime_iso8601_utf8_string
      @payload = GucefCoreDynamic.new(@_io)
    when :gucef_datatypes_int64
      @payload = GucefInt64.new(@_io, self, @_root)
    when :gucef_datatypes_utf16_be_string
      @payload = GucefCoreDynamic.new(@_io)
    end
    self
  end
  class GucefInt64 < Kaitai::Struct::Struct
    def initialize(_io, _parent = nil, _root = self)
      super(_io, _parent, _root)
      _read
    end

    def _read
      @data = @_io.read_s8le
      self
    end
    attr_reader :data
  end
  class GucefBsob < Kaitai::Struct::Struct
    def initialize(_io, _parent = nil, _root = self)
      super(_io, _parent, _root)
      _read
    end

    def _read
      @data = @_io.read_u1
      self
    end
    attr_reader :data
  end
  class GucefBeTimestampInMsSinceUnixEpoch < Kaitai::Struct::Struct
    def initialize(_io, _parent = nil, _root = self)
      super(_io, _parent, _root)
      _read
    end

    def _read
      @data = @_io.read_u8le
      self
    end
    attr_reader :data
  end
  class GucefLeTimestampInSecsSinceUnixEpoch < Kaitai::Struct::Struct
    def initialize(_io, _parent = nil, _root = self)
      super(_io, _parent, _root)
      _read
    end

    def _read
      @data = @_io.read_u8le
      self
    end
    attr_reader :data
  end
  class GucefUint16 < Kaitai::Struct::Struct
    def initialize(_io, _parent = nil, _root = self)
      super(_io, _parent, _root)
      _read
    end

    def _read
      @data = @_io.read_u2le
      self
    end
    attr_reader :data
  end
  class GucefUint64 < Kaitai::Struct::Struct
    def initialize(_io, _parent = nil, _root = self)
      super(_io, _parent, _root)
      _read
    end

    def _read
      @data = @_io.read_u8le
      self
    end
    attr_reader :data
  end
  class GucefUint8 < Kaitai::Struct::Struct
    def initialize(_io, _parent = nil, _root = self)
      super(_io, _parent, _root)
      _read
    end

    def _read
      @data = @_io.read_u1
      self
    end
    attr_reader :data
  end
  class GucefInt8 < Kaitai::Struct::Struct
    def initialize(_io, _parent = nil, _root = self)
      super(_io, _parent, _root)
      _read
    end

    def _read
      @data = @_io.read_s1
      self
    end
    attr_reader :data
  end
  class GucefInt16 < Kaitai::Struct::Struct
    def initialize(_io, _parent = nil, _root = self)
      super(_io, _parent, _root)
      _read
    end

    def _read
      @data = @_io.read_s2le
      self
    end
    attr_reader :data
  end
  class GucefFloat64 < Kaitai::Struct::Struct
    def initialize(_io, _parent = nil, _root = self)
      super(_io, _parent, _root)
      _read
    end

    def _read
      @data = @_io.read_f8le
      self
    end
    attr_reader :data
  end
  class GucefLeTimestampInMsSinceUnixEpoch < Kaitai::Struct::Struct
    def initialize(_io, _parent = nil, _root = self)
      super(_io, _parent, _root)
      _read
    end

    def _read
      @data = @_io.read_u8le
      self
    end
    attr_reader :data
  end
  class GucefInt32 < Kaitai::Struct::Struct
    def initialize(_io, _parent = nil, _root = self)
      super(_io, _parent, _root)
      _read
    end

    def _read
      @data = @_io.read_s4le
      self
    end
    attr_reader :data
  end
  class GucefBeTimestampInSecsSinceUnixEpoch < Kaitai::Struct::Struct
    def initialize(_io, _parent = nil, _root = self)
      super(_io, _parent, _root)
      _read
    end

    def _read
      @data = @_io.read_u8le
      self
    end
    attr_reader :data
  end
  class GucefFloat32 < Kaitai::Struct::Struct
    def initialize(_io, _parent = nil, _root = self)
      super(_io, _parent, _root)
      _read
    end

    def _read
      @data = @_io.read_f4le
      self
    end
    attr_reader :data
  end
  class GucefUint32 < Kaitai::Struct::Struct
    def initialize(_io, _parent = nil, _root = self)
      super(_io, _parent, _root)
      _read
    end

    def _read
      @data = @_io.read_u4le
      self
    end
    attr_reader :data
  end
  class GucefBooleanInt32 < Kaitai::Struct::Struct
    def initialize(_io, _parent = nil, _root = self)
      super(_io, _parent, _root)
      _read
    end

    def _read
      @data = @_io.read_s4le
      self
    end
    attr_reader :data
  end
  attr_reader :type_id
  attr_reader :payload
end
