package YaComponentIfc;

use strict;
use warnings;
#use Cwd;
#use Scalar::Util;
#use File::Path;
#use File::Find;
#use File::Basename;
#use File::Copy;
#use FindBin;
#use XML::Parser;
#use XML::Simple;
#use threads;
#use Time::HiRes qw(gettimeofday tv_interval);
#use ProBuildMake;

#use vars qw($VERSION @ISA @EXPORT);
#require Exporter;
#@ISA = qw(Exporter);
#@EXPORT = qw(createProdDirs $isWin $SrcProdDirDebug);
#$VERSION = 1.0;


BEGIN {
    use Exporter   ();
    our ($VERSION, @ISA, @EXPORT, @EXPORT_OK, %EXPORT_TAGS);

    # set the version for version checking
    $VERSION     = 1.0;

    @ISA         = qw(Exporter);
    @EXPORT      = qw( &printFatal
                       &printDbg
                       &printWarn
                      );
    %EXPORT_TAGS = ( ); 

    # your exported package globals go here,
    # as well as any optionally exported functions
#    @EXPORT_OK   = qw($RootPath);
}


our $MAJOR_VERSION = 1;
our $MINOR_VERSION = 0;
our @EXPORT_OK;
our $gVerbose=0;

our $isWin = 0;
if( $^O eq "MSWin32" )
{
  $isWin = 1;
} 
our $slash = "/";
$slash = "\\" if $isWin;

sub printFatal
{
  my $str = shift;
  die "fatal: $str\n";
}

sub printWarn
{
  my $str = shift;
  print STDERR "warning: $str\n";
}

sub printDbg
{
  my $str = shift;
  print STDERR "dbg: $str\n" if $gVerbose;
}

sub xmlEscape
{
  my $str = shift;
  $str =~ s/&(?!amp;)/&amp;/ig; 
  $str =~ s/</&lt;/ig;
  $str =~ s/>/&gt;/ig;

  return $str;
}

1;
