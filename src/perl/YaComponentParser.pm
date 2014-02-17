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

our $gGenCode=1;
our $gCodeOutPath;

our $gCompPath;

our $gIfcFileName;

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
  if(!defined $gCodeOutPath)
  {
    $gCodeOutPath= cwd() . '/' .lc($compbasename).'/code';
  }


  if($gGenCode && (defined $gCodeOutPath))
  {
    mkpath( $gCodeOutPath, {verbose => 1, mode => 0755}) if (!(-d $gCodeOutPath));
  }
}


sub readComp
{
  my $filename = shift;
  my $comp;
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
      my $suffix;
      (my $compName, $gCompPath, $suffix) = fileparse($filename,qw(.xml));

      $comp = $xmlContent;

 #     print Dumper($comp) if $YaComponent::gVerbose;
      YaComponent::printDbg("parsing Component Definition");
      parseCompDefinitions($comp);# if $YaComponent::gVerbose;
      $comp->{name}=$compName;
#      YaComponent::printDbg("Component after parsing");
#      print Dumper($gComp) if $YaComponent::gVerbose;
      # write out code files
    }

    if(!defined $filename)
    {
      YaComponent::printFatal("component file $filename could not be read");
    }
  }
  else
  {
    YaComponent::printFatal("component file $filename not found");
  }

  return $comp;
}


sub readIfc
{
  my $filename = shift;
  my $ifc;
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
      my $ifcBaseName = basename($filename,'.xml');

      $ifc = $xmlContent;

#      print Dumper($localIfc) if $YaComponent::gVerbose;
      YaComponent::printDbg("parsing Interface Definition");
      my($prop,$req,$resp,$incl) = parseIfcDefinitions($ifc);# if $YaComponent::gVerbose;
      $ifc->{properties} = \@{$prop};
      $ifc->{requests} = \@{$req};
      $ifc->{responses} = \@{$resp};
      $ifc->{includes} = \@{$incl};
      $ifc->{name} = $ifcBaseName;
      #YaComponent::printDbg("finished parsing Interface Definition");
      #print Dumper($ifc) if $YaComponent::gVerbose;
      #YaComponent::printDbg("is result of parsing");

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

  return $ifc;

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
    parseIfc($ifc);
  }
#  print "component after parsing:\n" if $YaComponent::gVerbose;
#  print Dumper($currRef) if $YaComponent::gVerbose;

}

sub parseIfc
{
  my $ifc = shift;
  my $currfilename = $ifc->{xml};
  my $filename = $currfilename;

  if(defined $currfilename)
  {
    $filename = $gCompPath . "/" . $currfilename if (! -f $currfilename);
    $filename = $gRootPath . "/" . $currfilename if (! -f $filename);
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

#      print Dumper($localIfc) if $YaComponent::gVerbose;
      YaComponent::printDbg("parsing Interface Definition");
      my($prop,$req,$resp,$incl) = parseIfcDefinitions($localIfc);# if $YaComponent::gVerbose;
      $ifc->{properties} = \@{$prop};
      $ifc->{requests} = \@{$req};
      $ifc->{responses} = \@{$resp};
      $ifc->{includes} = \@{$incl};
      #print Dumper($ifc) if $YaComponent::gVerbose;

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
  my @ifcProperties;
  my @ifcRequests;
  my @ifcResponses;
  my @ifcIncludes;

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
    push(@ifcProperties,$property);
  }



  foreach my $request (@{$currRef->{requests}->{req}})
  {
    #my $cnt = keys(%{$cfg});
     #YaComponent::printDbg("$cnt");
    push(@ifcRequests,$request);
    YaComponent::printDbg("request: $request->{id}");
  }

  foreach my $response (@{$currRef->{responses}->{resp}})
  {
    #my $cnt = keys(%{$cfg});
     #YaComponent::printDbg("$cnt");
    push(@ifcResponses,$response);
    YaComponent::printDbg("response: $response->{id}");
  }


  foreach my $include (@{$currRef->{include}})
  {
    YaComponent::printDbg("include: $include->{file}");
    push(@ifcIncludes, $include->{file});
  }

 return (\@ifcProperties, \@ifcRequests, \@ifcResponses, \@ifcIncludes);
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
