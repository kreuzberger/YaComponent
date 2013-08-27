package YaCompLayoutParser;

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

use YaCompLayoutCodeGen;
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

our $gLayoutFileName;
our $gLayoutGenCode=1;
our $gLayoutCodeOutPath;
our @gLayoutProcesses;
our @gLayoutThreads;
our $gLayout;
our $gLayoutName;
our @gLayoutIncludes;

our $isWin = 0;
if( $^O eq "MSWin32" )
{
  $isWin = 1;
}
our $slash = "/";
$slash = "\\" if $isWin;

sub init
{
  my $layoutname = shift;
  my $layoutbasename=basename($layoutname,'.xml');
  if(!defined $gLayoutCodeOutPath)
  {
    $gLayoutCodeOutPath= cwd() . '/' .lc($layoutbasename).'/code';
  }


  if($gLayoutGenCode && (defined $gLayoutCodeOutPath))
  {
    mkpath( $gLayoutCodeOutPath, {verbose => 1, mode => 0755}) if (!(-d $gLayoutCodeOutPath));
  }
}


sub readLayout
{
  my $filename = shift;
  undef $gLayoutFileName;
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
    my $xmlContent = eval{$xml->XMLin("$filename", ForceArray => [qw(process thread component)])};
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
      $gLayoutFileName = $filename;
      $gLayoutName = basename($gLayoutFileName,'.xml');

      $gLayout = $xmlContent;

      print Dumper($gLayout) if $YaComponent::gVerbose;
      YaComponent::printDbg("parsing Layout Definition");
      parseDefinitions($gLayout);# if $YaComponent::gVerbose;

      # write out code files
      YaCompLayoutCodeGen::writeCodeFiles($gLayoutName)  if( $gLayoutGenCode);
    }


    if(!defined $gLayoutFileName)
    {
      YaComponent::printFatal("fsm file $filename could not be read");
    }
  }
  else
  {
    YaComponent::printFatal("fsm file $filename not found");
  }
}




sub parseDefinitions
{
  my $currRef = shift;

  foreach my $include (@{$currRef->{include}})
  {
    YaComponent::printDbg("include: $include->{file}");
    push(@gLayoutIncludes, $include->{file});
  }


  foreach my $process (@{$currRef->{processes}->{process}})
  {
    YaComponent::printDbg("process: $process->{name}");
    if(defined $process->{thread})
    {
      YaComponent::printDbg("thread: $process->{thread}[0]->{name}");
    }
    push(@gLayoutProcesses, $process);
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
