package YaComponentDocGen;

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


sub writeCompDoc
{
  my $comp = shift;
  my $CompName = $comp->{name};


  my $fhCompDoc;
  my $strCompDefine;
  my $ifCnt = 0;
  my @provIfc;
  my @useIfc;

  open ( $fhCompDoc, "> $YaComponentParser::gDocOutPath" . '/' . "$CompName" . ".txt") || YaComponent::printFatal("error creating outfile");

  $strCompDefine .= "!definelong COMP_". uc($CompName) ."(name";

  @provIfc = @{$comp->{provides}->{interface}} if(defined $comp->{provides});
  foreach(@provIfc)
  {
   # $strCompDefine .= ",p" . $ifCnt;
    $ifCnt++;
  }

  @useIfc = @{$comp->{uses}->{interface}}   if( defined $comp->{uses} );
  foreach(@useIfc)
  {
  #  $strCompDefine .= ",u" . $ifCnt;
    $ifCnt++;
  }
  $ifCnt = 0;

  $strCompDefine .= ")\n";

  $strCompDefine .= "  [name] << $CompName >>\n";

  foreach my $ifc ( @provIfc )
  {
    my($xmlfilename, $directories, $suffix) = fileparse($ifc->{xml}, qw(.xml));
    $strCompDefine .= "  interface $ifc->{id} << $xmlfilename >> as name.$ifc->{id}\n";
    $strCompDefine .= "  name.$ifc->{id} << $xmlfilename >> - [name]\n";
  }

  foreach my $ifc ( @useIfc )
  {
#    my($xmlfilename, $directories, $suffix) = fileparse($ifc->{xml}, qw(.xml));
#    $strCompDefine .= "  [name] --> $xmlfilename : use\n";
  }

  $strCompDefine .= "!enddefinelong\n";

  print $fhCompDoc "$strCompDefine\n\n";


  close( $fhCompDoc);

}


sub writeIfcDoc
{
  my $ifc = shift;
  my $IfcName = $ifc->{name};

  my $fhIfcDoc;
  my $fhIfcStr;
  my $fhIfcIncludes;

  open ( $fhIfcDoc, "> $YaComponentParser::gDocOutPath" . '/' . "$IfcName" . ".txt") || YaComponent::printFatal("error creating outfile");

  foreach my $incFile (@{$ifc->{includes}})
  {
  }

  if(0 <= @{$ifc->{properties}})
  {
    foreach my $prop (@{$ifc->{properties}})
    {
#      $strProp .= uc($prop->{package}) . "_" if ($prop->{package});
#      $strProp .= uc($prop->{id});
    }
  }

  foreach my $req (@{$ifc->{requests}})
  {
#    print $fhHeader "    void request". $req->{id};
    my $strPara;
    foreach my $para (@{$req->{para}})
    {
#      $strPara .= " const ";
#      $strPara .= $para->{package} . "::" if ($para->{package});
#      $strPara .= "$para->{id}&,";
    }
  }

  foreach my $resp (@{$ifc->{responses}})
  {
#    print $fhHeader $resp->{package} . "::" if ($resp->{package});
#    print $fhHeader $resp->{id}. "& ) = 0;\n";
  }

  close( $fhIfcDoc)
}

1;
