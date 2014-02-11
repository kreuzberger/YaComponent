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


sub writeComponentIfc
{
  my $CompName = shift;
  my $fhHeader;

  open ( $fhHeader, "> $YaComponentParser::gIfcCodeOutPath" . '/' . "I$CompName" . "Comp.h") || YaComponent::printFatal("error creating outfile");

  if($YaComponent::gVerbose)
  {
    eval "use Data::Dumper";
    if($@)
    {
      YaComponent::printWarn $@;
      YaComponent::printFatal("Missing required package Data::Dumper");
    }
  }

  print $fhHeader '#ifndef ' . uc($CompName) ."_H\n";
  print $fhHeader '#define ' . uc($CompName) ."_H\n\n";
  print $fhHeader "#include <stdio.h>\n";

  print $fhHeader "class Ya" . $CompName ."Ifc\n";
  print $fhHeader "{\n";
  print $fhHeader "  public:\n";
  print $fhHeader "    Ya" . $CompName . "Ifc() {}\n";
  print $fhHeader "    virtual ~ Ya" . $CompName . "Ifc() {}\n";
  print $fhHeader "  protected:\n";
  print $fhHeader "};\n";

  print $fhHeader "\n#endif\n";
  close( $fhHeader);

}


sub writeComponentImpl
{
  my $CompName = shift;
  my $fhHeader;
  my $fhSource;

  open ( $fhHeader, "> $YaComponentParser::gIfcCodeOutPath" . '/' . "$CompName" . "Impl.h") || YaComponent::printFatal("error creating outfile");
  open ( $fhSource, "> $YaComponentParser::gIfcCodeOutPath" . '/' . "$CompName" . "Impl.cpp") || YaComponent::printFatal("error creating outfile");

  if($YaComponent::gVerbose)
  {
    eval "use Data::Dumper";
    if($@)
    {
      YaComponent::printWarn $@;
      YaComponent::printFatal("Missing required package Data::Dumper");
    }
  }

  print $fhHeader '#ifndef ' . uc($CompName) ."IMPL_H\n";
  print $fhHeader '#define ' . uc($CompName) ."IMPL_H\n\n";
  print $fhHeader "#include <QtCore/QObject>\n";
  print $fhHeader "#include <stdio.h>\n";

  print $fhHeader "class Ya" . $CompName ."Impl: public QObject\n";
  print $fhHeader "{\n";
  print $fhHeader "  Q_OBJECT\n";
  print $fhHeader "  public:\n";
  print $fhHeader "    Ya" . $CompName . "Impl() {}\n";
  print $fhHeader "    virtual ~ Ya" . $CompName . "Impl() {}\n";
  print $fhHeader "  protected:\n";
  print $fhHeader "};\n";

  print $fhHeader "\n#endif\n";


  print $fhSource "#include \"" . $CompName . "Impl.h\"\n";
  print $fhSource "#include \"" . $CompName . "Impl.moc\"\n";

  close( $fhHeader);

  close( $fhSource)

}



sub writeCodeFiles
{
  my $CompName = shift;

  my $fh;

  writeComponentIfc($CompName);
  open ( $fh, "> $YaComponentParser::gIfcCodeOutPath" . '/' . "$CompName" . "CompProxy.h") || YaComponent::printFatal("error creating outfile");
  close( $fh);
  open ( $fh, "> $YaComponentParser::gIfcCodeOutPath" . '/' . "$CompName" . "CompProxy.cpp") || YaComponent::printFatal("error creating outfile");
  close( $fh);
  open ( $fh, "> $YaComponentParser::gIfcCodeOutPath" . '/' . "$CompName" . "CompStub.h") || YaComponent::printFatal("error creating outfile");
  close( $fh);
  open ( $fh, "> $YaComponentParser::gIfcCodeOutPath" . '/' . "$CompName" . "CompStub.cpp") || YaComponent::printFatal("error creating outfile");
  close( $fh);
  writeComponentImpl($CompName);
}




1;
