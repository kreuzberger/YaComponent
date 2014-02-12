package YaComponentParser;

use strict;
use warnings;
use Cwd;
#use Scalar::Util;
use File::Path;
#use File::Find;
use File::Basename;
use File::Copy;
#use FindBin;
#use XML::Parser;
#use XML::Simple;
#use threads;
#use Time::HiRes qw(gettimeofday tv_interval);
#use ProBuildMake;

use YaComponentCodeGen;
use FindBin;

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

our $gRootPath;
our $gCompFileName;
our $gCompGenCode=1;
our $gCompCodeOutPath;
our $gComp;
our $gCompName;
our $gCompPath;
our @gCompInterfaces;

our @gIfcRequests;
our @gIfcResponses;
our @gIfcProperties;
our $gIfc;
our $gIfcName;
our @gIfcIncludes;

our $isWin = 0;
if( $^O eq "MSWin32" )
{
  $isWin = 1;
}
our $slash = "/";
$slash = "\\" if $isWin;

sub init
{
  my $compname = shift;
  my $compbasename=basename($compname,'.xml');
  if(!defined $gCompCodeOutPath)
  {
    $gCompCodeOutPath= cwd() . '/' .lc($compbasename).'/code';
  }


  if($gCompGenCode && (defined $gCompCodeOutPath))
  {
    mkpath( $gCompCodeOutPath, {verbose => 1, mode => 0755}) if (!(-d $gCompCodeOutPath));
  }
}


sub readComp
{
  my $filename = shift;
  undef $gCompFileName;
  if( -f $filename)
  {
    eval "use XML::Simple";
    if($@)
    {
      YaComponent::printWarn $@;
      YaComponent::printFatal("Missing required package XML::Simple");
    }

    if($YaComponent::gVerbose)
    {
      eval "use Data::Dumper";
      if($@)
      {
        YaComponent::printWarn $@;
        YaComponent::printFatal("Missing required package Data::Dumper");
      }
    }


    # create object
    my $xml = new XML::Simple (KeyAttr=>[]);

    # read XML file
    # force array is useful for configurations that have only one entry and are not parsed into
    # array by default. So we ensure that the buildcfg always is an array!
    my $xmlContent = eval{$xml->XMLin("$filename", ForceArray => [qw(interface)])};
    if ($@)
    {
      # parse error messages look like this:
      # not well-formed at line 10, column 8, byte 382 at d:/bin/perl/site/lib/XML/Parser.pm line 183
      $@ =~ /(.*) at .*? line \d+\.?\s*$/;
      my $errmsg = $@;
      YaComponent::printWarn("Parse error: $errmsg");
      YaComponent::printFatal("exiting script due to parse error");
    }
    else
    {
      $gCompFileName = $filename;
      my $suffix;
      ($gCompName, $gCompPath, $suffix) = fileparse($gCompFileName,qw(.xml));

      $gComp = $xmlContent;

      print Dumper($gComp) if $YaComponent::gVerbose;
      YaComponent::printDbg("parsing Component Definition");
      parseCompDefinitions($gComp);# if $YaComponent::gVerbose;

      # write out code files
      YaComponentCodeGen::writeCodeFiles($gCompName)  if( $gCompGenCode);
    }


    if(!defined $gCompFileName)
    {
      YaComponent::printFatal("component file $filename could not be read");
    }
  }
  else
  {
    YaComponent::printFatal("component file $filename not found");
  }
}


sub parseCompDefinitions
{
  my $currRef = shift;

  foreach my $ifc (@{$currRef->{provides}->{interface}})
  {
    if(!defined $ifc->{id} || !defined $ifc->{xml})
    {
      YaComponent::printFatal("missing definition of interface id or xml file");
    }
    YaComponent::printDbg("if id: $ifc->{id}, xml = $ifc->{xml}");
    readIfc($ifc);
    push(@gCompInterfaces,$ifc);
  }
}

sub readIfc
{
  my $ifc = shift;
  my $currfilename = $ifc->{xml};
  my $filename = $currfilename;

  if(defined $currfilename)
  {
    $filename = $gCompPath . "/" . $currfilename if (! -f $currfilename);
    $filename = $gRootPath . "/" . $currfilename if (! -f $currfilename);
  }

  if( -f $filename)
  {
    eval "use XML::Simple";
    if($@)
    {
      YaComponent::printWarn $@;
      YaComponent::printFatal("Missing required package XML::Simple");
    }

    if($YaComponent::gVerbose)
    {
      eval "use Data::Dumper";
      if($@)
      {
        YaComponent::printWarn $@;
        YaComponent::printFatal("Missing required package Data::Dumper");
      }
    }


    # create object
    my $xml = new XML::Simple (KeyAttr=>[]);

    # read XML file
    # force array is useful for configurations that have only one entry and are not parsed into
    # array by default. So we ensure that the buildcfg always is an array!
    my $xmlContent = eval{$xml->XMLin("$filename", ForceArray => [qw(prop req resp include)])};
    if ($@)
    {
      # parse error messages look like this:
      # not well-formed at line 10, column 8, byte 382 at d:/bin/perl/site/lib/XML/Parser.pm line 183
      $@ =~ /(.*) at .*? line \d+\.?\s*$/;
      my $errmsg = $@;
      YaComponent::printWarn("Parse error: $errmsg");
      YaComponent::printFatal("exiting script due to parse error");
    }
    else
    {
      my $ifcFileName = $filename;
      my $ifcBaseName = basename($ifcFileName,'.xml');

      my $localIfc = $xmlContent;

      print Dumper($localIfc) if $YaComponent::gVerbose;
      YaComponent::printDbg("parsing Interface Definition");
      parseIfcDefinitions($localIfc);# if $YaComponent::gVerbose;

    }

    if(!defined $filename)
    {
      YaComponent::printFatal("interface file $filename could not be read");
    }
  }
  else
  {
    YaComponent::printFatal("interface file $filename not found");
  }
}




sub parseIfcDefinitions
{
  my $currRef = shift;

  foreach my $property (@{$currRef->{properties}->{prop}})
  {
    #my $cnt = keys(%{$cfg});
     #YaComponent::printDbg("$cnt");

    if(!defined $property->{onChange})
    {
      $property->{onChange} = 0;
    }
    if(!defined $property->{id} )
    {
      YaComponent::printFatal("missing definition of property id");
    }
    YaComponent::printDbg("property id: $property->{id}, onChange = $property->{onChange}");
    push(@gIfcProperties,$property);
  }



  foreach my $request (@{$currRef->{requests}->{req}})
  {
    #my $cnt = keys(%{$cfg});
     #YaComponent::printDbg("$cnt");
    push(@gIfcRequests,$request);
    YaComponent::printDbg("request: $request->{id}");
  }

  foreach my $response (@{$currRef->{responses}->{resp}})
  {
    #my $cnt = keys(%{$cfg});
     #YaComponent::printDbg("$cnt");
    push(@gIfcResponses,$response);
    YaComponent::printDbg("response: $response->{id}");
  }


  foreach my $include (@{$currRef->{include}})
  {
    YaComponent::printDbg("include: $include->{file}");
    push(@gIfcIncludes, $include->{file});
  }
}


#sub hasSubStates
#{
#  my $hasSubStates = 0;
#  my $state = shift;
#  if($state->{state})
#  {
#   	YaComponent::printDbg("Found substates in state $state->{name}");
#    $hasSubStates = 1;
#  }#

  #return $hasSubStates;
#}


#sub getMethodTokens
#{
#  my $str = shift;
#  my $parseUsage = 0;
#  $parseUsage = shift;
#  my %func;

#  my @tokens = split(/;/,$str);
#  foreach my $token (@tokens)
#  {
#    $token =~ /(\b\w+\b).*\((.*)\)/;
#    my $funcname = $1;
#    my $args = $2;
#    my @args = split(/,/,$args);

#    foreach my $arg(@args)
#    {
#      my $argtype;
#      my $argname;
#      $arg =~ s/^\s+//;
 #     $arg =~ s/\s+$//;
#      $arg =~ /(.+)(\s+)+(\b\w+\b)+/;
#      if( $1 && $3 )
#      {
#        $argtype = $1;
 #       $argname = $3;
#      }
#      else
#      {
#        $arg =~ /(\b\w+\b)/;
#        if( $parseUsage )
#        {
#          $argname = $1;
#        }
#        else
#        {
#          $argtype = $1;
#        }
#      }

#      push(@{$func{params}},{name => $argname, type => $argtype});

#    }
#    $func{name}=$funcname;
#  }
#  return %func;
#}

#sub getEnterStateName
#{
#  my $state = shift;
#  my $enterStateName;
#  if($state->{state})
#  {
#    foreach my $substate (@{$state->{state}})
#    {
#      if ( $substate->{type} &&  "entry" eq $substate->{type} )
#      {
#        $enterStateName = $substate->{name};
#      }
#    }
#  }

#  return $enterStateName;
#}


1;
