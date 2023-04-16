# This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

use strict;
use warnings;
use IO::KaitaiStruct 0.009_000;
use GucefPubsubMsgcontainerFooter;
use GucefCoreVariant;
use GucefPubsubMsgcontainerHeader;

########################################################################
package GucefPubsubMsgcontainer;

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

    $self->{header} = GucefPubsubMsgcontainerHeader->new($self->{_io});
    $self->{pubsubmsg} = ();
    do {
        $_ = GucefPubsubMsgcontainer::GucefPubsubMsg->new($self->{_io}, $self, $self->{_root});
        push @{$self->{pubsubmsg}}, $_;
    } until ($self->_io()->pos() == ((($self->_io()->size() - ($self->msg_offset_idx_size() * 4)) - 4) - 10));
    $self->{footer} = GucefPubsubMsgcontainerFooter->new($self->{_io});
}

sub msg_offset_idx_size {
    my ($self) = @_;
    return $self->{msg_offset_idx_size} if ($self->{msg_offset_idx_size});
    my $_pos = $self->{_io}->pos();
    $self->{_io}->seek(($self->_io()->size() - 14));
    $self->{msg_offset_idx_size} = $self->{_io}->read_u4le();
    $self->{_io}->seek($_pos);
    return $self->{msg_offset_idx_size};
}

sub header {
    my ($self) = @_;
    return $self->{header};
}

sub pubsubmsg {
    my ($self) = @_;
    return $self->{pubsubmsg};
}

sub footer {
    my ($self) = @_;
    return $self->{footer};
}

########################################################################
package GucefPubsubMsgcontainer::GucefPubsubMsg;

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

    if (($self->_root()->header()->options() & (1 << 1)) != 0) {
        my $_on = ($self->_root()->header()->options() & (1 << 2));
        if ($_on == 0) {
            $self->{msg_date_time} = $self->{_io}->read_u4le();
        }
        else {
            $self->{msg_date_time} = $self->{_io}->read_u8le();
        }
    }
    if (($self->_root()->header()->options() & (1 << 3)) != 0) {
        $self->{msgid} = GucefCoreVariant->new($self->{_io});
    }
    if (($self->_root()->header()->options() & (1 << 4)) != 0) {
        $self->{msg_index} = GucefCoreVariant->new($self->{_io});
    }
    if (($self->_root()->header()->options() & (1 << 5)) != 0) {
        $self->{primary_payload_included} = GucefCoreVariant->new($self->{_io});
    }
    if (($self->_root()->header()->options() & (1 << 6)) != 0) {
        $self->{nr_of_kv_pairs} = $self->{_io}->read_u4le();
    }
    if (($self->_root()->header()->options() & (1 << 6)) != 0) {
        $self->{key_value_pairs} = ();
        my $n_key_value_pairs = $self->nr_of_kv_pairs();
        for (my $i = 0; $i < $n_key_value_pairs; $i++) {
            push @{$self->{key_value_pairs}}, GucefPubsubMsgcontainer::KvPair->new($self->{_io}, $self, $self->{_root});
        }
    }
    if (($self->_root()->header()->options() & (1 << 7)) != 0) {
        $self->{nr_of_metadata_kv_pairs} = $self->{_io}->read_u4le();
    }
    if (($self->_root()->header()->options() & (1 << 7)) != 0) {
        $self->{metadata_key_value_pairs} = ();
        my $n_metadata_key_value_pairs = $self->nr_of_metadata_kv_pairs();
        for (my $i = 0; $i < $n_metadata_key_value_pairs; $i++) {
            push @{$self->{metadata_key_value_pairs}}, GucefPubsubMsgcontainer::KvPair->new($self->{_io}, $self, $self->{_root});
        }
    }
}

sub msg_date_time {
    my ($self) = @_;
    return $self->{msg_date_time};
}

sub msgid {
    my ($self) = @_;
    return $self->{msgid};
}

sub msg_index {
    my ($self) = @_;
    return $self->{msg_index};
}

sub primary_payload_included {
    my ($self) = @_;
    return $self->{primary_payload_included};
}

sub nr_of_kv_pairs {
    my ($self) = @_;
    return $self->{nr_of_kv_pairs};
}

sub key_value_pairs {
    my ($self) = @_;
    return $self->{key_value_pairs};
}

sub nr_of_metadata_kv_pairs {
    my ($self) = @_;
    return $self->{nr_of_metadata_kv_pairs};
}

sub metadata_key_value_pairs {
    my ($self) = @_;
    return $self->{metadata_key_value_pairs};
}

########################################################################
package GucefPubsubMsgcontainer::KvPair;

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

    $self->{key} = GucefCoreVariant->new($self->{_io});
    $self->{value} = GucefCoreVariant->new($self->{_io});
}

sub key {
    my ($self) = @_;
    return $self->{key};
}

sub value {
    my ($self) = @_;
    return $self->{value};
}

1;
