# This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

use strict;
use warnings;
use IO::KaitaiStruct 0.009_000;

########################################################################
package GucefPubsubMsgcontainerFooter;

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

    $self->{msg_offset_idx} = ();
    my $n_msg_offset_idx = $self->mois();
    for (my $i = 0; $i < $n_msg_offset_idx; $i++) {
        push @{$self->{msg_offset_idx}}, $self->{_io}->read_u4le();
    }
    $self->{msg_offset_idx_size} = $self->{_io}->read_u4le();
    $self->{magic_text} = $self->{_io}->read_bytes(10);
}

sub mois {
    my ($self) = @_;
    return $self->{mois} if ($self->{mois});
    my $_pos = $self->{_io}->pos();
    $self->{_io}->seek(($self->_io()->size() - 14));
    $self->{mois} = $self->{_io}->read_u4le();
    $self->{_io}->seek($_pos);
    return $self->{mois};
}

sub msg_offset_idx {
    my ($self) = @_;
    return $self->{msg_offset_idx};
}

sub msg_offset_idx_size {
    my ($self) = @_;
    return $self->{msg_offset_idx_size};
}

sub magic_text {
    my ($self) = @_;
    return $self->{magic_text};
}

1;
