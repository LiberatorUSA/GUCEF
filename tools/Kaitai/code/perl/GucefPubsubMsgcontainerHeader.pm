# This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

use strict;
use warnings;
use IO::KaitaiStruct 0.009_000;

########################################################################
package GucefPubsubMsgcontainerHeader;

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

    $self->{magic_text} = $self->{_io}->read_bytes(10);
    $self->{version} = $self->{_io}->read_u1();
    $self->{options} = $self->{_io}->read_u4le();
}

sub magic_text {
    my ($self) = @_;
    return $self->{magic_text};
}

sub version {
    my ($self) = @_;
    return $self->{version};
}

sub options {
    my ($self) = @_;
    return $self->{options};
}

1;
