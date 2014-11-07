package YaCompLayoutCodeGen;

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



sub writeProcessMain
{
  my $CompName = shift;
  my $hashAdresses = shift;

  if($YaComponent::gVerbose)
  {
    eval "use Data::Dumper";
    if($@)
    {
      YaComponent::printWarn $@;
      YaComponent::printFatal("Missing required package Data::Dumper");
    }
  }

#  my $fhHeader;
#  open ( $fhHeader, "> $YaCompLayoutParser::gLayoutCodeOutPath" . '/' . "$CompName" . ".h") || YaComponent::printFatal("error creating outfile");
#  print $fhHeader '#ifndef ' . uc($CompName) ."_H\n";
#  print $fhHeader '#define ' . uc($CompName) ."_H\n\n";
#  print $fhHeader "#include <QtCore/QThread>\n";
#  print $fhHeader "#include <stdio.h>\n";


  foreach my $process (@YaCompLayoutParser::gLayoutProcesses)
  {
    print Dumper($process) if $YaComponent::gVerbose;

    my $fhSource;
    open ( $fhSource, "> $YaCompLayoutParser::gLayoutCodeOutPath" . '/' . "$CompName" . $process->{name} . ".cpp") || YaComponent::printFatal("error creating outfile");


#    print $fhSource '#include "' . $CompName . 'CompLayout.h"' . "\n";
#    print $fhSource "#include \"YaComponent.h\"\n";
#    print $fhHeader 'extern void ' . $process->{name} ."Start();\n";

    #print $fhSource 'void ' . $process->{name} ."Start()\n";
    #print $fhSource '{' ."\n";

    print $fhSource "  void* context = YaComponent::context_new();\n";

    foreach my $thread (@{$process->{thread}})
    {
#      print $fhHeader " class YaThread" . $thread->{name} .": public QThread\n";
#      print $fhHeader "{\n";
#      print $fhHeader "  public:\n";
#      print $fhHeader "    YaThread" . $thread->{name} . "() {}\n";
#      print $fhHeader "    virtual ~YaThread" . $thread->{name} . "() {}\n";
#      print $fhHeader "  protected:\n";
#      print $fhHeader "    virtual void run() {printf(\"starting thread\\n\");exec();printf(\"exiting thread\\n\");}\n";
#      print $fhHeader "};\n";

      print $fhSource "  YaComponentThread" . " $thread->{name}" .";\n";
      print $fhSource "  printf(\"starting thread\\n\");\n";
      print $fhSource "  $thread->{name}.start();\n";
      foreach my $comp (@{$thread->{component}})
      {
        writeComponentParts($fhSource, $comp, $hashAdresses);
        print $fhSource "  $comp->{name}.moveToThread(&$thread->{name});\n";
        print $fhSource "  printf(\"calling move to thread\\n\");\n";
      }

    }

    # components associated with the main thread
    foreach my $comp (@{$process->{component}})
    {
      writeComponentParts($fhSource, $comp, $hashAdresses);
    }


   # print $fhSource '}' ."\n";

    close( $fhSource);
  }
#  print $fhHeader "\n#endif\n";
#  close( $fhHeader);

}

sub writeComponentParts
{
  my $fhSource = shift;
  my $comp = shift;
  my $adresses = shift;

  my($xmlfilename, $directories, $suffix) = fileparse($comp->{xml}, qw(.xml));

  #print $fhSource "  #include \"" . $xmlfilename . "Impl.h\"\n";
  print $fhSource "  " . $xmlfilename . " $comp->{name}(context);\n";

  foreach my $ifc (@{$comp->{provides}->{interface}})
  {
    print $fhSource "  $comp->{name}.setConnectionPara". $ifc->{id} . "( \"$ifc->{pub}\", \"$ifc->{req}\"";
    if( $ifc->{hwm} )
    {
      print $fhSource ", $ifc->{hwm} );\n";
    }
    else
    {
      print $fhSource " );\n";
    }
  }

  foreach my $ifc (@{$comp->{uses}->{interface}})
  {
    my $key =$ifc->{sub};
    if( exists $adresses->{$key} )
    {
      print $fhSource "  $comp->{name}.setConnectionPara". $ifc->{id} . "( \"";
      print $fhSource $adresses->{$key}{pub};
      print $fhSource "\", \"";
      print $fhSource $adresses->{$key}{req};
      print $fhSource "\" );\n";
    }
  }

  print $fhSource "  $comp->{name}.init();\n";

}

sub parseAdressInformations()
{
  my %hashAdresses;


  foreach my $process (@YaCompLayoutParser::gLayoutProcesses)
  {
    foreach my $thread (@{$process->{thread}})
    {
      foreach my $comp (@{$thread->{component}})
      {
        foreach my $ifc (@{$comp->{provides}->{interface}})
        {
          $hashAdresses{$comp->{name} . "." . $ifc->{id}} = {pub => $ifc->{pub}, req => $ifc->{req}};
        }
      }
    }

    # components associated with the main thread
    foreach my $comp (@{$process->{component}})
    {
      foreach my $ifc (@{$comp->{provides}->{interface}})
      {
        $hashAdresses{$comp->{name} . "." . $ifc->{id}} = {pub => "$ifc->{pub}", req => "$ifc->{req}"};
      }
    }
  }

  return %hashAdresses;
}


sub writeCodeFiles
{
  my $CompName = shift;

#  my $fh;

#  open ( $fh, "> $YaCompLayoutParser::gLayoutCodeOutPath" . '/' . "I$CompName" . "CompLayout.h") || YaComponent::printFatal("error creating outfile");
#  close( $fh);

  my %hashAdresses = parseAdressInformations();

  writeProcessMain($CompName, \%hashAdresses);
}




1;
