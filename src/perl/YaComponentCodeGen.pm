package YaComponentCodeGen;

use strict;
use warnings;
#use Cwd;
#use Scalar::Util;
use File::Path;
#use File::Find;
use File::Basename;
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
    @EXPORT      = qw();#&createProdDirs
    %EXPORT_TAGS = ( );     # eg: TAG => [ qw!name1 name2! ],

    # your exported package globals go here,
    # as well as any optionally exported functions
#    @EXPORT_OK   = qw($RootPath);
}


our $MAJOR_VERSION = 1;
our $MINOR_VERSION = 0;
our @EXPORT_OK;



sub writeCodeFiles
{
  my $IfcName = shift;

  my $fh;

  open ( $fh, "> $YaComponentParser::gIfcCodeOutPath" . '/' . "I$IfcName" . "Comp.h") || YaComponent::printFatal("error creating outfile");
  close( $fh);
  open ( $fh, "> $YaComponentParser::gIfcCodeOutPath" . '/' . "$IfcName" . "CompProxy.h") || YaComponent::printFatal("error creating outfile");
  close( $fh);
  open ( $fh, "> $YaComponentParser::gIfcCodeOutPath" . '/' . "$IfcName" . "CompProxy.cpp") || YaComponent::printFatal("error creating outfile");
  close( $fh);
  open ( $fh, "> $YaComponentParser::gIfcCodeOutPath" . '/' . "$IfcName" . "CompStub.h") || YaComponent::printFatal("error creating outfile");
  close( $fh);
  open ( $fh, "> $YaComponentParser::gIfcCodeOutPath" . '/' . "$IfcName" . "CompStub.cpp") || YaComponent::printFatal("error creating outfile");
  close( $fh);
  open ( $fh, "> $YaComponentParser::gIfcCodeOutPath" . '/' . "$IfcName" . "CompImpl.h") || YaComponent::printFatal("error creating outfile");
  close( $fh);
  open ( $fh, "> $YaComponentParser::gIfcCodeOutPath" . '/' . "$IfcName" . "CompImpl.cpp") || YaComponent::printFatal("error creating outfile");
  close( $fh);
}




1;
