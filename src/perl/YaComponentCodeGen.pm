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
  my $comp = shift;
  my $CompName = $comp->{name};
  my $fhHeader;

  open ( $fhHeader, "> $YaComponentParser::gCodeOutPath" . '/' . "I$CompName" . "Comp.h") || YaComponent::printFatal("error creating outfile");

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

  print $fhHeader "namespace YaComponent {\n";

  print $fhHeader "class " . $CompName ."Ifc\n";
  print $fhHeader "{\n";
  print $fhHeader "  public:\n";
  print $fhHeader "    " . $CompName . "Ifc() {}\n";
  print $fhHeader "    virtual ~" . $CompName . "Ifc() {}\n";
  print $fhHeader "  protected:\n";
  print $fhHeader "};\n";
  print $fhHeader "}\n";

  print $fhHeader "\n#endif\n";
  close( $fhHeader);

}


sub writeComponentImpl
{
  my $comp = shift;
  my $CompName = $comp->{name};
  my $fhHeader;
  my $fhSource;

  open ( $fhHeader, "> $YaComponentParser::gCodeOutPath" . '/' . "$CompName" . "Impl.h") || YaComponent::printFatal("error creating outfile");
  open ( $fhSource, "> $YaComponentParser::gCodeOutPath" . '/' . "$CompName" . "Impl.cpp") || YaComponent::printFatal("error creating outfile");

  if($YaComponent::gVerbose)
  {
    eval "use Data::Dumper";
    if($@)
    {
      YaComponent::printWarn $@;
      YaComponent::printFatal("Missing required package Data::Dumper");
    }
  }

  #print Dumper($comp) if $YaComponent::gVerbose;

  print $fhHeader '#ifndef ' . uc($CompName) ."IMPL_H\n";
  print $fhHeader '#define ' . uc($CompName) ."IMPL_H\n\n";
  print $fhHeader "#include <QtCore/QObject>\n";
  print $fhHeader "#include <stdio.h>\n";

  print $fhHeader "namespace YaComponent {\n";

  print $fhHeader "class " . $CompName ."Impl: public QObject\n";
  print $fhHeader "{\n";
  print $fhHeader "  Q_OBJECT\n";
  print $fhHeader "  public:\n";
  print $fhHeader "    " . $CompName . "Impl() {}\n";
  print $fhHeader "    virtual ~ " . $CompName . "Impl() {}\n";
  print $fhHeader "  protected:\n";
  print $fhHeader "};\n";

  print $fhHeader "}\n";

  print $fhHeader "\n#endif\n";


  print $fhSource "#include \"" . $CompName . "Impl.h\"\n";
  print $fhSource "#include \"" . $CompName . "Impl.moc\"\n";

  close( $fhHeader);

  close( $fhSource)

}

sub writeIfcStub
{
  my $ifc = shift;
  my $IfcName = $ifc->{name};
  my $fhHeader;
  my $fhSource;

  open ( $fhHeader, "> $YaComponentParser::gCodeOutPath" . '/' . "$IfcName" . "Stub.h") || YaComponent::printFatal("error creating outfile");
  open ( $fhSource, "> $YaComponentParser::gCodeOutPath" . '/' . "$IfcName" . "Stub.cpp") || YaComponent::printFatal("error creating outfile");

  if($YaComponent::gVerbose)
  {
    eval "use Data::Dumper";
    if($@)
    {
      YaComponent::printWarn $@;
      YaComponent::printFatal("Missing required package Data::Dumper");
    }
  }

 # print Dumper($ifc) if $YaComponent::gVerbose;

  print $fhHeader '#ifndef ' . uc($IfcName) ."STUB_H\n";
  print $fhHeader '#define ' . uc($IfcName) ."STUB_H\n\n";
  print $fhHeader "#include <QtCore/QObject>\n";
  print $fhHeader "#include <stdio.h>\n";

  print $fhHeader "namespace YaComponent {\n";

  print $fhHeader "class " . $IfcName ."Stub: public QObject\n";
  print $fhHeader "{\n";
  print $fhHeader "  Q_OBJECT\n";
  print $fhHeader "  public:\n";
  print $fhHeader "    " . $IfcName . "Stub() {}\n";
  print $fhHeader "    virtual ~ " . $IfcName . "Stub() {}\n";


  foreach my $prop (@{$ifc->{properties}})
  {
    print $fhHeader "    void setNotification(PROP_" . uc($prop->{id}) . ")\n";
  }

  print $fhHeader "  protected:\n";
  print $fhHeader "};\n";

  print $fhHeader "}\n";

  print $fhHeader "\n#endif\n";


  print $fhSource "#include \"" . $IfcName . "Stub.h\"\n";
  print $fhSource "#include \"" . $IfcName . "Stub.moc\"\n";

  close( $fhHeader);

  close( $fhSource)

}


sub writeIfcProxy
{
  my $comp = shift;
  my $CompName = $comp->{name};
  my $fhHeader;
  my $fhSource;

  open ( $fhHeader, "> $YaComponentParser::gCodeOutPath" . '/' . "$CompName" . "Proxy.h") || YaComponent::printFatal("error creating outfile");
  open ( $fhSource, "> $YaComponentParser::gCodeOutPath" . '/' . "$CompName" . "Proxy.cpp") || YaComponent::printFatal("error creating outfile");

  if($YaComponent::gVerbose)
  {
    eval "use Data::Dumper";
    if($@)
    {
      YaComponent::printWarn $@;
      YaComponent::printFatal("Missing required package Data::Dumper");
    }
  }

 # print Dumper($comp) if $YaComponent::gVerbose;

  print $fhHeader '#ifndef ' . uc($CompName) ."PROXY_H\n";
  print $fhHeader '#define ' . uc($CompName) ."PROXY_H\n\n";
  print $fhHeader "#include <QtCore/QObject>\n";
  print $fhHeader "#include <stdio.h>\n";

  print $fhHeader "namespace YaComponent {\n";

  print $fhHeader "class " . $CompName ."Proxy: public QObject\n";
  print $fhHeader "{\n";
  print $fhHeader "  Q_OBJECT\n";
  print $fhHeader "  public:\n";
  print $fhHeader "    " . $CompName . "Proxy() {}\n";
  print $fhHeader "    virtual ~ " . $CompName . "Proxy() {}\n";
  print $fhHeader "  protected:\n";
  print $fhHeader "};\n";

  print $fhHeader "}\n";

  print $fhHeader "\n#endif\n";


  print $fhSource "#include \"" . $CompName . "Proxy.h\"\n";
  print $fhSource "#include \"" . $CompName . "Proxy.moc\"\n";

  close( $fhHeader);

  close( $fhSource)

}



sub writeCompCodeFiles
{
  my $comp = shift;
  my $CompName = $comp->{name};

  my $fh;

#  writeComponentIfc($comp);
#  writeIfcProxy($comp);
#  writeIfcStub($comp);
  writeComponentImpl($comp);
}


sub writeIfcCodeFiles
{
  my $ifc = shift;

  if($YaComponent::gVerbose)
  {
    eval "use Data::Dumper";
    if($@)
    {
      YaComponent::printWarn $@;
      YaComponent::printFatal("Missing required package Data::Dumper");
    }
  }
  #print Dumper($ifc) if $YaComponent::gVerbose;
  my $ifcName = $ifc->{name};

  print "writing ifc code files with $ifcName\n";

  my $fh;

#  writeComponentIfc($comp);
  writeIfcProxy($ifc);
  writeIfcStub($ifc);
}



1;
