# This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

use strict;
use warnings;
use IO::KaitaiStruct 0.009_000;
use GucefCoreDynamic;

########################################################################
package GucefCoreVariant;

our @ISA = 'IO::KaitaiStruct::Struct';

sub from_file {
    my ($class, $filename) = @_;
    my $fd;

    open($fd, '<', $filename) or return undef;
    binmode($fd);
    return new($class, IO::KaitaiStruct::Stream->new($fd));
}

our $GUCEF_DATATYPES_UINT8 = 1;
our $GUCEF_DATATYPES_INT8 = 2;
our $GUCEF_DATATYPES_UINT16 = 3;
our $GUCEF_DATATYPES_INT16 = 5;
our $GUCEF_DATATYPES_UINT32 = 7;
our $GUCEF_DATATYPES_INT32 = 9;
our $GUCEF_DATATYPES_UINT64 = 11;
our $GUCEF_DATATYPES_INT64 = 13;
our $GUCEF_DATATYPES_FLOAT32 = 15;
our $GUCEF_DATATYPES_FLOAT64 = 17;
our $GUCEF_DATATYPES_ASCII_STRING = 21;
our $GUCEF_DATATYPES_UTF8_STRING = 22;
our $GUCEF_DATATYPES_UTF16_LE_STRING = 23;
our $GUCEF_DATATYPES_UTF16_BE_STRING = 24;
our $GUCEF_DATATYPES_UTF32_STRING = 25;
our $GUCEF_DATATYPES_BOOLEAN_INT32 = 30;
our $GUCEF_DATATYPES_BOOLEAN_ASCII_STRING = 31;
our $GUCEF_DATATYPES_BOOLEAN_UTF8_STRING = 32;
our $GUCEF_DATATYPES_BINARY_BLOB = 40;
our $GUCEF_DATATYPES_BINARY_BSOB = 41;
our $GUCEF_DATATYPES_DATETIME_ISO8601_ASCII_STRING = 50;
our $GUCEF_DATATYPES_DATETIME_ISO8601_UTF8_STRING = 51;
our $GUCEF_DATATYPES_LE_TIMESTAMP_IN_SECS_SINCE_UNIX_EPOCH = 55;
our $GUCEF_DATATYPES_BE_TIMESTAMP_IN_SECS_SINCE_UNIX_EPOCH = 56;
our $GUCEF_DATATYPES_LE_TIMESTAMP_IN_MS_SINCE_UNIX_EPOCH = 57;
our $GUCEF_DATATYPES_BE_TIMESTAMP_IN_MS_SINCE_UNIX_EPOCH = 58;

sub new {
    my ($class, $_io, $_parent, $_root) = @_;
    my $self = IO::KaitaiStruct::Struct->new($_io);

    bless $self, $class;
    $self->{_parent} = $_parent;
    $self->{_root} = $_root || $self;;

    $self->_read();

    return $self;
}

sub _read {
    my ($self) = @_;

    $self->{type_id} = $self->{_io}->read_u1();
    my $_on = $self->type_id();
    if ($_on == $GucefCoreVariant::GUCEF_DATATYPES_DATETIME_ISO8601_ASCII_STRING) {
        $self->{payload} = GucefCoreDynamic->new($self->{_io});
    }
    elsif ($_on == $GucefCoreVariant::GUCEF_DATATYPES_UTF16_LE_STRING) {
        $self->{payload} = GucefCoreDynamic->new($self->{_io});
    }
    elsif ($_on == $GucefCoreVariant::GUCEF_DATATYPES_INT16) {
        $self->{payload} = GucefCoreVariant::GucefInt16->new($self->{_io}, $self, $self->{_root});
    }
    elsif ($_on == $GucefCoreVariant::GUCEF_DATATYPES_FLOAT32) {
        $self->{payload} = GucefCoreVariant::GucefFloat32->new($self->{_io}, $self, $self->{_root});
    }
    elsif ($_on == $GucefCoreVariant::GUCEF_DATATYPES_ASCII_STRING) {
        $self->{payload} = GucefCoreDynamic->new($self->{_io});
    }
    elsif ($_on == $GucefCoreVariant::GUCEF_DATATYPES_UINT16) {
        $self->{payload} = GucefCoreVariant::GucefUint16->new($self->{_io}, $self, $self->{_root});
    }
    elsif ($_on == $GucefCoreVariant::GUCEF_DATATYPES_UINT64) {
        $self->{payload} = GucefCoreVariant::GucefUint64->new($self->{_io}, $self, $self->{_root});
    }
    elsif ($_on == $GucefCoreVariant::GUCEF_DATATYPES_INT32) {
        $self->{payload} = GucefCoreVariant::GucefInt32->new($self->{_io}, $self, $self->{_root});
    }
    elsif ($_on == $GucefCoreVariant::GUCEF_DATATYPES_UINT8) {
        $self->{payload} = GucefCoreVariant::GucefUint8->new($self->{_io}, $self, $self->{_root});
    }
    elsif ($_on == $GucefCoreVariant::GUCEF_DATATYPES_UINT32) {
        $self->{payload} = GucefCoreVariant::GucefUint32->new($self->{_io}, $self, $self->{_root});
    }
    elsif ($_on == $GucefCoreVariant::GUCEF_DATATYPES_FLOAT64) {
        $self->{payload} = GucefCoreVariant::GucefFloat64->new($self->{_io}, $self, $self->{_root});
    }
    elsif ($_on == $GucefCoreVariant::GUCEF_DATATYPES_BE_TIMESTAMP_IN_SECS_SINCE_UNIX_EPOCH) {
        $self->{payload} = GucefCoreVariant::GucefBeTimestampInSecsSinceUnixEpoch->new($self->{_io}, $self, $self->{_root});
    }
    elsif ($_on == $GucefCoreVariant::GUCEF_DATATYPES_BE_TIMESTAMP_IN_MS_SINCE_UNIX_EPOCH) {
        $self->{payload} = GucefCoreVariant::GucefBeTimestampInMsSinceUnixEpoch->new($self->{_io}, $self, $self->{_root});
    }
    elsif ($_on == $GucefCoreVariant::GUCEF_DATATYPES_UTF32_STRING) {
        $self->{payload} = GucefCoreDynamic->new($self->{_io});
    }
    elsif ($_on == $GucefCoreVariant::GUCEF_DATATYPES_BOOLEAN_UTF8_STRING) {
        $self->{payload} = GucefCoreDynamic->new($self->{_io});
    }
    elsif ($_on == $GucefCoreVariant::GUCEF_DATATYPES_BINARY_BLOB) {
        $self->{payload} = GucefCoreDynamic->new($self->{_io});
    }
    elsif ($_on == $GucefCoreVariant::GUCEF_DATATYPES_LE_TIMESTAMP_IN_MS_SINCE_UNIX_EPOCH) {
        $self->{payload} = GucefCoreVariant::GucefLeTimestampInMsSinceUnixEpoch->new($self->{_io}, $self, $self->{_root});
    }
    elsif ($_on == $GucefCoreVariant::GUCEF_DATATYPES_BINARY_BSOB) {
        $self->{payload} = GucefCoreVariant::GucefBsob->new($self->{_io}, $self, $self->{_root});
    }
    elsif ($_on == $GucefCoreVariant::GUCEF_DATATYPES_BOOLEAN_ASCII_STRING) {
        $self->{payload} = GucefCoreDynamic->new($self->{_io});
    }
    elsif ($_on == $GucefCoreVariant::GUCEF_DATATYPES_UTF8_STRING) {
        $self->{payload} = GucefCoreDynamic->new($self->{_io});
    }
    elsif ($_on == $GucefCoreVariant::GUCEF_DATATYPES_INT8) {
        $self->{payload} = GucefCoreVariant::GucefInt8->new($self->{_io}, $self, $self->{_root});
    }
    elsif ($_on == $GucefCoreVariant::GUCEF_DATATYPES_BOOLEAN_INT32) {
        $self->{payload} = GucefCoreVariant::GucefBooleanInt32->new($self->{_io}, $self, $self->{_root});
    }
    elsif ($_on == $GucefCoreVariant::GUCEF_DATATYPES_LE_TIMESTAMP_IN_SECS_SINCE_UNIX_EPOCH) {
        $self->{payload} = GucefCoreVariant::GucefLeTimestampInSecsSinceUnixEpoch->new($self->{_io}, $self, $self->{_root});
    }
    elsif ($_on == $GucefCoreVariant::GUCEF_DATATYPES_DATETIME_ISO8601_UTF8_STRING) {
        $self->{payload} = GucefCoreDynamic->new($self->{_io});
    }
    elsif ($_on == $GucefCoreVariant::GUCEF_DATATYPES_INT64) {
        $self->{payload} = GucefCoreVariant::GucefInt64->new($self->{_io}, $self, $self->{_root});
    }
    elsif ($_on == $GucefCoreVariant::GUCEF_DATATYPES_UTF16_BE_STRING) {
        $self->{payload} = GucefCoreDynamic->new($self->{_io});
    }
}

sub type_id {
    my ($self) = @_;
    return $self->{type_id};
}

sub payload {
    my ($self) = @_;
    return $self->{payload};
}

########################################################################
package GucefCoreVariant::GucefInt64;

our @ISA = 'IO::KaitaiStruct::Struct';

sub from_file {
    my ($class, $filename) = @_;
    my $fd;

    open($fd, '<', $filename) or return undef;
    binmode($fd);
    return new($class, IO::KaitaiStruct::Stream->new($fd));
}

sub new {
    my ($class, $_io, $_parent, $_root) = @_;
    my $self = IO::KaitaiStruct::Struct->new($_io);

    bless $self, $class;
    $self->{_parent} = $_parent;
    $self->{_root} = $_root || $self;;

    $self->_read();

    return $self;
}

sub _read {
    my ($self) = @_;

    $self->{data} = $self->{_io}->read_s8le();
}

sub data {
    my ($self) = @_;
    return $self->{data};
}

########################################################################
package GucefCoreVariant::GucefBsob;

our @ISA = 'IO::KaitaiStruct::Struct';

sub from_file {
    my ($class, $filename) = @_;
    my $fd;

    open($fd, '<', $filename) or return undef;
    binmode($fd);
    return new($class, IO::KaitaiStruct::Stream->new($fd));
}

sub new {
    my ($class, $_io, $_parent, $_root) = @_;
    my $self = IO::KaitaiStruct::Struct->new($_io);

    bless $self, $class;
    $self->{_parent} = $_parent;
    $self->{_root} = $_root || $self;;

    $self->_read();

    return $self;
}

sub _read {
    my ($self) = @_;

    $self->{data} = $self->{_io}->read_u1();
}

sub data {
    my ($self) = @_;
    return $self->{data};
}

########################################################################
package GucefCoreVariant::GucefBeTimestampInMsSinceUnixEpoch;

our @ISA = 'IO::KaitaiStruct::Struct';

sub from_file {
    my ($class, $filename) = @_;
    my $fd;

    open($fd, '<', $filename) or return undef;
    binmode($fd);
    return new($class, IO::KaitaiStruct::Stream->new($fd));
}

sub new {
    my ($class, $_io, $_parent, $_root) = @_;
    my $self = IO::KaitaiStruct::Struct->new($_io);

    bless $self, $class;
    $self->{_parent} = $_parent;
    $self->{_root} = $_root || $self;;

    $self->_read();

    return $self;
}

sub _read {
    my ($self) = @_;

    $self->{data} = $self->{_io}->read_u8le();
}

sub data {
    my ($self) = @_;
    return $self->{data};
}

########################################################################
package GucefCoreVariant::GucefLeTimestampInSecsSinceUnixEpoch;

our @ISA = 'IO::KaitaiStruct::Struct';

sub from_file {
    my ($class, $filename) = @_;
    my $fd;

    open($fd, '<', $filename) or return undef;
    binmode($fd);
    return new($class, IO::KaitaiStruct::Stream->new($fd));
}

sub new {
    my ($class, $_io, $_parent, $_root) = @_;
    my $self = IO::KaitaiStruct::Struct->new($_io);

    bless $self, $class;
    $self->{_parent} = $_parent;
    $self->{_root} = $_root || $self;;

    $self->_read();

    return $self;
}

sub _read {
    my ($self) = @_;

    $self->{data} = $self->{_io}->read_u8le();
}

sub data {
    my ($self) = @_;
    return $self->{data};
}

########################################################################
package GucefCoreVariant::GucefUint16;

our @ISA = 'IO::KaitaiStruct::Struct';

sub from_file {
    my ($class, $filename) = @_;
    my $fd;

    open($fd, '<', $filename) or return undef;
    binmode($fd);
    return new($class, IO::KaitaiStruct::Stream->new($fd));
}

sub new {
    my ($class, $_io, $_parent, $_root) = @_;
    my $self = IO::KaitaiStruct::Struct->new($_io);

    bless $self, $class;
    $self->{_parent} = $_parent;
    $self->{_root} = $_root || $self;;

    $self->_read();

    return $self;
}

sub _read {
    my ($self) = @_;

    $self->{data} = $self->{_io}->read_u2le();
}

sub data {
    my ($self) = @_;
    return $self->{data};
}

########################################################################
package GucefCoreVariant::GucefUint64;

our @ISA = 'IO::KaitaiStruct::Struct';

sub from_file {
    my ($class, $filename) = @_;
    my $fd;

    open($fd, '<', $filename) or return undef;
    binmode($fd);
    return new($class, IO::KaitaiStruct::Stream->new($fd));
}

sub new {
    my ($class, $_io, $_parent, $_root) = @_;
    my $self = IO::KaitaiStruct::Struct->new($_io);

    bless $self, $class;
    $self->{_parent} = $_parent;
    $self->{_root} = $_root || $self;;

    $self->_read();

    return $self;
}

sub _read {
    my ($self) = @_;

    $self->{data} = $self->{_io}->read_u8le();
}

sub data {
    my ($self) = @_;
    return $self->{data};
}

########################################################################
package GucefCoreVariant::GucefUint8;

our @ISA = 'IO::KaitaiStruct::Struct';

sub from_file {
    my ($class, $filename) = @_;
    my $fd;

    open($fd, '<', $filename) or return undef;
    binmode($fd);
    return new($class, IO::KaitaiStruct::Stream->new($fd));
}

sub new {
    my ($class, $_io, $_parent, $_root) = @_;
    my $self = IO::KaitaiStruct::Struct->new($_io);

    bless $self, $class;
    $self->{_parent} = $_parent;
    $self->{_root} = $_root || $self;;

    $self->_read();

    return $self;
}

sub _read {
    my ($self) = @_;

    $self->{data} = $self->{_io}->read_u1();
}

sub data {
    my ($self) = @_;
    return $self->{data};
}

########################################################################
package GucefCoreVariant::GucefInt8;

our @ISA = 'IO::KaitaiStruct::Struct';

sub from_file {
    my ($class, $filename) = @_;
    my $fd;

    open($fd, '<', $filename) or return undef;
    binmode($fd);
    return new($class, IO::KaitaiStruct::Stream->new($fd));
}

sub new {
    my ($class, $_io, $_parent, $_root) = @_;
    my $self = IO::KaitaiStruct::Struct->new($_io);

    bless $self, $class;
    $self->{_parent} = $_parent;
    $self->{_root} = $_root || $self;;

    $self->_read();

    return $self;
}

sub _read {
    my ($self) = @_;

    $self->{data} = $self->{_io}->read_s1();
}

sub data {
    my ($self) = @_;
    return $self->{data};
}

########################################################################
package GucefCoreVariant::GucefInt16;

our @ISA = 'IO::KaitaiStruct::Struct';

sub from_file {
    my ($class, $filename) = @_;
    my $fd;

    open($fd, '<', $filename) or return undef;
    binmode($fd);
    return new($class, IO::KaitaiStruct::Stream->new($fd));
}

sub new {
    my ($class, $_io, $_parent, $_root) = @_;
    my $self = IO::KaitaiStruct::Struct->new($_io);

    bless $self, $class;
    $self->{_parent} = $_parent;
    $self->{_root} = $_root || $self;;

    $self->_read();

    return $self;
}

sub _read {
    my ($self) = @_;

    $self->{data} = $self->{_io}->read_s2le();
}

sub data {
    my ($self) = @_;
    return $self->{data};
}

########################################################################
package GucefCoreVariant::GucefFloat64;

our @ISA = 'IO::KaitaiStruct::Struct';

sub from_file {
    my ($class, $filename) = @_;
    my $fd;

    open($fd, '<', $filename) or return undef;
    binmode($fd);
    return new($class, IO::KaitaiStruct::Stream->new($fd));
}

sub new {
    my ($class, $_io, $_parent, $_root) = @_;
    my $self = IO::KaitaiStruct::Struct->new($_io);

    bless $self, $class;
    $self->{_parent} = $_parent;
    $self->{_root} = $_root || $self;;

    $self->_read();

    return $self;
}

sub _read {
    my ($self) = @_;

    $self->{data} = $self->{_io}->read_f8le();
}

sub data {
    my ($self) = @_;
    return $self->{data};
}

########################################################################
package GucefCoreVariant::GucefLeTimestampInMsSinceUnixEpoch;

our @ISA = 'IO::KaitaiStruct::Struct';

sub from_file {
    my ($class, $filename) = @_;
    my $fd;

    open($fd, '<', $filename) or return undef;
    binmode($fd);
    return new($class, IO::KaitaiStruct::Stream->new($fd));
}

sub new {
    my ($class, $_io, $_parent, $_root) = @_;
    my $self = IO::KaitaiStruct::Struct->new($_io);

    bless $self, $class;
    $self->{_parent} = $_parent;
    $self->{_root} = $_root || $self;;

    $self->_read();

    return $self;
}

sub _read {
    my ($self) = @_;

    $self->{data} = $self->{_io}->read_u8le();
}

sub data {
    my ($self) = @_;
    return $self->{data};
}

########################################################################
package GucefCoreVariant::GucefInt32;

our @ISA = 'IO::KaitaiStruct::Struct';

sub from_file {
    my ($class, $filename) = @_;
    my $fd;

    open($fd, '<', $filename) or return undef;
    binmode($fd);
    return new($class, IO::KaitaiStruct::Stream->new($fd));
}

sub new {
    my ($class, $_io, $_parent, $_root) = @_;
    my $self = IO::KaitaiStruct::Struct->new($_io);

    bless $self, $class;
    $self->{_parent} = $_parent;
    $self->{_root} = $_root || $self;;

    $self->_read();

    return $self;
}

sub _read {
    my ($self) = @_;

    $self->{data} = $self->{_io}->read_s4le();
}

sub data {
    my ($self) = @_;
    return $self->{data};
}

########################################################################
package GucefCoreVariant::GucefBeTimestampInSecsSinceUnixEpoch;

our @ISA = 'IO::KaitaiStruct::Struct';

sub from_file {
    my ($class, $filename) = @_;
    my $fd;

    open($fd, '<', $filename) or return undef;
    binmode($fd);
    return new($class, IO::KaitaiStruct::Stream->new($fd));
}

sub new {
    my ($class, $_io, $_parent, $_root) = @_;
    my $self = IO::KaitaiStruct::Struct->new($_io);

    bless $self, $class;
    $self->{_parent} = $_parent;
    $self->{_root} = $_root || $self;;

    $self->_read();

    return $self;
}

sub _read {
    my ($self) = @_;

    $self->{data} = $self->{_io}->read_u8le();
}

sub data {
    my ($self) = @_;
    return $self->{data};
}

########################################################################
package GucefCoreVariant::GucefFloat32;

our @ISA = 'IO::KaitaiStruct::Struct';

sub from_file {
    my ($class, $filename) = @_;
    my $fd;

    open($fd, '<', $filename) or return undef;
    binmode($fd);
    return new($class, IO::KaitaiStruct::Stream->new($fd));
}

sub new {
    my ($class, $_io, $_parent, $_root) = @_;
    my $self = IO::KaitaiStruct::Struct->new($_io);

    bless $self, $class;
    $self->{_parent} = $_parent;
    $self->{_root} = $_root || $self;;

    $self->_read();

    return $self;
}

sub _read {
    my ($self) = @_;

    $self->{data} = $self->{_io}->read_f4le();
}

sub data {
    my ($self) = @_;
    return $self->{data};
}

########################################################################
package GucefCoreVariant::GucefUint32;

our @ISA = 'IO::KaitaiStruct::Struct';

sub from_file {
    my ($class, $filename) = @_;
    my $fd;

    open($fd, '<', $filename) or return undef;
    binmode($fd);
    return new($class, IO::KaitaiStruct::Stream->new($fd));
}

sub new {
    my ($class, $_io, $_parent, $_root) = @_;
    my $self = IO::KaitaiStruct::Struct->new($_io);

    bless $self, $class;
    $self->{_parent} = $_parent;
    $self->{_root} = $_root || $self;;

    $self->_read();

    return $self;
}

sub _read {
    my ($self) = @_;

    $self->{data} = $self->{_io}->read_u4le();
}

sub data {
    my ($self) = @_;
    return $self->{data};
}

########################################################################
package GucefCoreVariant::GucefBooleanInt32;

our @ISA = 'IO::KaitaiStruct::Struct';

sub from_file {
    my ($class, $filename) = @_;
    my $fd;

    open($fd, '<', $filename) or return undef;
    binmode($fd);
    return new($class, IO::KaitaiStruct::Stream->new($fd));
}

sub new {
    my ($class, $_io, $_parent, $_root) = @_;
    my $self = IO::KaitaiStruct::Struct->new($_io);

    bless $self, $class;
    $self->{_parent} = $_parent;
    $self->{_root} = $_root || $self;;

    $self->_read();

    return $self;
}

sub _read {
    my ($self) = @_;

    $self->{data} = $self->{_io}->read_s4le();
}

sub data {
    my ($self) = @_;
    return $self->{data};
}

1;
