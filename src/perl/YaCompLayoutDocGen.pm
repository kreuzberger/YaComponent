package YaCompLayoutDocGen;

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



sub writeLayout
{
  my $CompName = shift;
  if($YaComponent::gVerbose)
  {
    eval "use Data::Dumper";
    if($@)
    {
      YaComponent::printWarn $@;
      YaComponent::printFatal("Missing required package Data::Dumper");
    }
  }

  my $fhLayoutDoc;
  my $fhLayoutStr;
  my $fhLayoutIncludes;

  open ( $fhLayoutDoc, "> $YaCompLayoutParser::gLayoutDocOutPath" . '/' . "$CompName" . ".txt") || YaComponent::printFatal("error creating outfile");

  print $fhLayoutDoc "\@startuml\n\n";
  print $fhLayoutDoc "title: Process and Thread Layout for $CompName\n\n";


  foreach my $process (@YaCompLayoutParser::gLayoutProcesses)
  {
#    print Dumper($process) if $YaComponent::gVerbose;

    $fhLayoutStr .= "package $process->{name} {\n\n";


    foreach my $thread (@{$process->{thread}})
    {
      $fhLayoutStr .= "  frame $thread->{name} {\n";

      foreach my $comp (@{$thread->{component}})
      {
       # $fhLayoutStr .= "    [$comp->{name}] AS $comp->{id}\n";

        my($xmlfilename, $directories, $suffix) = fileparse($comp->{xml}, qw(.xml));
        $fhLayoutIncludes  .= "!include $xmlfilename/doc/$xmlfilename.txt\n";
        #$fhLayoutStr .= "!include $xmlfilename/doc/$xmlfilename.txt\n";
        $fhLayoutStr .= "    COMP_" . uc($xmlfilename) ."($comp->{name})\n";
      }

      $fhLayoutStr .= "  }\n\n"; #close thread
    }

    # components associated with the main thread
    my @components = @{$process->{component}};
    if(0 <= $#components)
    {
      $fhLayoutStr .= "  frame mainThread {\n";

      foreach my $comp (@components)
      {
        #$fhLayoutStr .= "    [$comp->{name}] AS $comp->{id}\n";

        my($xmlfilename, $directories, $suffix) = fileparse($comp->{xml}, qw(.xml));
        $fhLayoutIncludes  .= "!include $xmlfilename/doc/$xmlfilename.txt\n";
        $fhLayoutStr .= "    COMP_" . uc($xmlfilename) ."($comp->{name})\n";

        if (defined $comp->{uses} && defined $comp->{uses}->{interface} )
        {
          my @interfaces = @{ $comp->{uses}->{interface} };
          foreach my $ifc (@interfaces )
          {
            $fhLayoutStr .= "    $comp->{name} \"$ifc->{id}\" #-( $ifc->{sub} : use\n";
          }
        }
      }

      $fhLayoutStr .= "  }\n\n"; # close main thread
    }


    $fhLayoutStr .= "}\n\n"; #close package process

  }

  $fhLayoutStr .= "\@enduml\n";

  print $fhLayoutDoc $fhLayoutIncludes . "\n" if($fhLayoutIncludes);
  print $fhLayoutDoc $fhLayoutStr;

  close( $fhLayoutDoc);

}


sub writeDocFiles
{
  my $CompName = shift;

  writeLayout($CompName);
}




1;
