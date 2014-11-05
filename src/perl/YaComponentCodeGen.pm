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

  print $fhSource "#include \"" . $CompName . "Impl.h\"\n\n";
  print $fhSource "namespace YaComponent {\n\n";
#  print $fhSource "#include \"" . $CompName . "Impl.moc\"\n";

  print $fhHeader '#ifndef ' . uc($CompName) ."IMPL_H\n";
  print $fhHeader '#define ' . uc($CompName) ."IMPL_H\n\n";

  my %classNameProxyHash;
  my %classNameStubHash;

  foreach my $usedIfc (@{$comp->{used}})
  {
    $classNameProxyHash{$usedIfc->{classname}} = $usedIfc->{id};
  }

  foreach my $providedIfc (@{$comp->{provided}})
  {
    $classNameStubHash{$providedIfc->{classname}} = $providedIfc->{id};
  }

  foreach my $className (sort ( keys ( %classNameProxyHash )))
  {
    print "used xml $className\n";
    print $fhHeader "#include\"" . $className . "Proxy.h\"\n";
  }

  foreach my $className (sort ( keys ( %classNameStubHash )))
  {
    print "used xml $className\n";
    print $fhHeader "#include\"" . $className . "Stub.h\"\n";
  }


  print $fhHeader "#include <QtCore/QObject>\n";
  print $fhHeader "#include <stdio.h>\n";

  print $fhHeader "namespace YaComponent {\n";

  print $fhHeader "class " . $CompName ."Impl: public QObject\n";
  print $fhHeader "{\n";
  print $fhHeader "  Q_OBJECT\n";
  print $fhHeader "  public:\n";
  print $fhHeader "    " . $CompName . "Impl(";

  print $fhSource $CompName . "Impl::$CompName" . "Impl(";
  my $strCtor="void* context";

  foreach my $className (sort ( keys ( %classNameProxyHash )))
  {
    $strCtor .= ", I" . $className . "ProxyHandler& r$className";
  }

  #chop($strCtor);

  print $fhHeader "$strCtor );\n";
  print $fhSource "$strCtor )\n";

  print $fhHeader "    virtual ~" . $CompName . "Impl() {}\n";
  print $fhHeader "  protected:\n";

  my $strCtorImpl="";

  if( exists($comp->{used}) )
  {
    $strCtorImpl .= ":"
  }

  foreach my $usedIfc (@{$comp->{used}})
  {
    print $fhHeader "  " . $usedIfc->{classname} . "Proxy m" . $usedIfc->{id} . ";\n";
    $strCtorImpl .=  " m" . $usedIfc->{id} . "( context, r$usedIfc->{classname} )\n";
    $strCtorImpl .= ",";
  }

  foreach my $providedIfc (@{$comp->{provided}})
  {
    print $fhHeader "  " . $providedIfc->{classname} . "Stub m" . $providedIfc->{id} . ";\n";
    $strCtorImpl .=  " m" . $providedIfc->{id} . "( context )\n";
    $strCtorImpl .= ",";
  }

  chop($strCtorImpl);

  print $fhSource "$strCtorImpl";
  print $fhSource "{\n\n}\n";



  print $fhHeader "\n  private:\n";
#  print $fhHeader "    ". $CompName . "Impl ();\n";
  print $fhHeader "    ". $CompName. "Impl( const " . $CompName . "Impl& );\n";
  print $fhHeader "    ". $CompName. "Impl& operator= ( const " . $CompName . "Impl& );\n";


  print $fhHeader "};\n";

  print $fhHeader "}\n";

  print $fhHeader "\n#endif\n";

  print $fhSource "}\n";


  close( $fhHeader);

  close( $fhSource)

}

sub writeIfcStub
{
  my $ifc = shift;
  my $IfcName = $ifc->{name};
  my $fhHeader;
  my $fhHeaderIfc;
  my $fhSource;

  open ( $fhHeader, "> $YaComponentParser::gCodeOutPath" . '/' . "$IfcName" . "Stub.h") || YaComponent::printFatal("error creating outfile");
  open ( $fhHeaderIfc, "> $YaComponentParser::gCodeOutPath" . '/I' . "$IfcName" . "StubHandler.h") || YaComponent::printFatal("error creating outfile");
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
  print $fhSource "#include \"" . $IfcName . "Stub.h\"\n";

  print $fhHeader '#ifndef ' . uc($IfcName) ."STUB_H\n";
  print $fhHeader '#define ' . uc($IfcName) ."STUB_H\n\n";

  print $fhHeaderIfc '#ifndef ' . uc($IfcName) ."STUBHANDLER_H\n";
  print $fhHeaderIfc '#define ' . uc($IfcName) ."STUBHANDLER_H\n\n";


  foreach my $incFile (@{$ifc->{includes}})
  {
    print $fhHeader "#include \"$incFile\"\n";
  }
#  print $fhHeader "#include <QtCore/QObject>\n";
  print $fhHeader "#include \"YaStubBase.h\"\n";
  print $fhHeader "#include <stdio.h>\n";

  print $fhHeader "#include \"YaPUBImpl.h\"\n";



  print $fhHeader "namespace YaComponent {\n";
  print $fhHeaderIfc "namespace YaComponent {\n";
  print $fhSource "namespace YaComponent {\n";

#  print $fhSource $IfcName . "Stub::" . $IfcName . "Stub( void* context )\n";
  print $fhSource $IfcName . "Stub::" . $IfcName . "Stub( )\n";

  print $fhHeaderIfc "class I" . $IfcName ."StubHandler\n";
  print $fhHeaderIfc "{\n";

  print $fhHeader "class " . $IfcName ."Stub: public YaStubBase\n";
  print $fhHeader "{\n";
  print $fhHeader "  Q_OBJECT\n";
  print $fhHeader "  public:\n";
  print $fhHeaderIfc "  public:\n";
#  print $fhHeader "    " . $IfcName . "Stub( void* context);\n";
  print $fhHeader "    " . $IfcName . "Stub(  );\n";
  print $fhHeader "    virtual ~" . $IfcName . "Stub();\n";

  print $fhHeaderIfc "    I" . $IfcName . "StubHandler() {};\n";
  print $fhHeaderIfc "    virtual ~I" . $IfcName . "StubHandler() {};\n";


  print $fhHeader "  protected:\n";

  if(0 <= @{$ifc->{properties}})
  {
    print $fhHeader "    typedef enum PROPERTIES {\n";
    foreach my $prop (@{$ifc->{properties}})
    {
      my $strProp;
      $strProp .= "PROP_";
      $strProp .= uc($prop->{package}) . "_" if ($prop->{package});
      $strProp .= uc($prop->{id});
      print $fhHeader "      $strProp,\n";
    }
    print $fhHeader "    } tProperty;\n\n";
  }

  foreach my $prop (@{$ifc->{properties}})
  {
    my $strMethod;
    $strMethod .= $prop->{package} . "::" if ($prop->{package});
    $strMethod .= $prop->{id};
    print $fhHeader "    void send(int key, const $strMethod&);\n";
  }

  foreach my $req (@{$ifc->{requests}})
  {
    print $fhHeaderIfc "    void onRequest". $req->{id};
    print $fhHeaderIfc "( ";
    my $strPara;
    foreach my $para (@{$req->{para}})
    {
      $strPara .= " const ";
      $strPara .= $para->{package} . "::" if ($para->{package});
      $strPara .= "$para->{id}&";
    }
    chop($strPara);
    print $fhHeaderIfc $strPara . ") = 0;\n";
  }

  print $fhHeader "\n";

  foreach my $resp (@{$ifc->{responses}})
  {
#  print $fhHeaderIfc "    virtual void response( const QObject*, const ";
    print $fhHeader "    virtual void response( const ";
    print $fhHeader $resp->{package} . "::" if ($resp->{package});
    print $fhHeader $resp->{id}. "& ) {}\n";
  }


  print $fhSource " : YaStubBase()\n";

  foreach my $prop (@{$ifc->{properties}})
  {
#    print $fhHeader "    void setNotification(PROP_" . uc($prop->{id}) . ");\n";
    print $fhHeader "    YaPUBImpl m" . $prop->{id} . ";\n";
    print $fhSource " , m" . $prop->{id} . "( 0 )\n";
  }
  print $fhSource "{\n\n}\n\n";


  print $fhSource $IfcName . "Stub::~" . $IfcName . "Stub()\n";
  print $fhSource "{\n\n}\n\n";

  print $fhHeader "};\n";
  print $fhHeaderIfc "};\n";

  print $fhHeader "}\n";
  print $fhHeaderIfc "}\n";
  print $fhSource "}\n";

  print $fhHeader "\n#endif\n";

  close( $fhHeader);
  close( $fhHeaderIfc);
  close( $fhSource)

}


sub writeIfcProxy
{
  my $ifc = shift;
  my $IfcName = $ifc->{name};
  my $fhHeader;
  my $fhHeaderIfc;
  my $fhSource;

  open ( $fhHeader, "> $YaComponentParser::gCodeOutPath" . '/' . "$IfcName" . "Proxy.h") || YaComponent::printFatal("error creating outfile");
  open ( $fhHeaderIfc, "> $YaComponentParser::gCodeOutPath" . '/I' . "$IfcName" . "ProxyHandler.h") || YaComponent::printFatal("error creating outfile");
  open ( $fhSource, "> $YaComponentParser::gCodeOutPath" . '/' . "$IfcName" . "Proxy.cpp") || YaComponent::printFatal("error creating outfile");

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

  print $fhSource "#include \"" . $IfcName . "Proxy.h\"\n";
  print $fhSource "namespace YaComponent {\n";
  print $fhSource $IfcName . "Proxy::" . $IfcName . "Proxy( void* context, I" . $IfcName . "ProxyHandler& callbackHandler)\n";
  print $fhSource ": YaProxyBase()\n";
  print $fhSource ", mCallbackHandler( callbackHandler )\n";
  foreach my $prop (@{$ifc->{properties}})
  {
    print $fhSource ", m" . $prop->{id} . "(context)\n";
  }
  print $fhSource "{\n}\n\n";

  my $strDtor = "\n" . $IfcName . "Proxy::~" . $IfcName . "Proxy()\n{\n";


  print $fhHeader '#ifndef ' . uc($IfcName) ."PROXY_H\n";
  print $fhHeader '#define ' . uc($IfcName) ."PROXY_H\n\n";

  print $fhHeaderIfc '#ifndef ' . uc($IfcName) ."PROXYHANDLER_H\n";
  print $fhHeaderIfc '#define ' . uc($IfcName) ."PROXYHANDLER_H\n\n";


  foreach my $incFile (@{$ifc->{includes}})
  {
    print $fhHeader "#include \"$incFile\"\n";
    print $fhHeaderIfc "#include \"$incFile\"\n";
  }

  print $fhHeader "#include \"I" . $IfcName . "ProxyHandler.h\"\n";
  print $fhHeader "#include \"YaProxyBase.h\"\n";
  print $fhHeader "#include \"YaSUBImpl.h\"\n";
  print $fhHeader "#include <QtCore/QObject>\n";
#  print $fhHeader "#include <stdio.h>\n";

  print $fhHeader "namespace YaComponent {\n";
 #print $fhHeaderIfc "namespace YaComponent {\n";

  print $fhHeader "class " . $IfcName ."Proxy: public YaProxyBase\n";
  print $fhHeader "{\n";
#  print $fhHeader "  Q_OBJECT\n";
  print $fhHeader "  public:\n";
  print $fhHeader "    " . $IfcName . "Proxy( void* context, I" . $IfcName . "ProxyHandler& );\n";
  print $fhHeader "    virtual ~" . $IfcName . "Proxy();\n\n";

  print $fhHeaderIfc "\nclass YaProxyBase;\n\n";

  print $fhHeaderIfc "class I" . $IfcName ."ProxyHandler\n";
  print $fhHeaderIfc "{\n";
  print $fhHeaderIfc "  public:\n";
  print $fhHeaderIfc "    I" . $IfcName . "ProxyHandler() {}\n";
  print $fhHeaderIfc "    virtual ~I" . $IfcName . "ProxyHandler() {}\n\n";

  #print $fhHeader "  protected:\n";


  if(0 <= @{$ifc->{properties}})
  {
    print $fhHeader "    typedef enum PROPERTIES {\n";
    foreach my $prop (@{$ifc->{properties}})
    {
      my $strProp;
      $strProp .= "PROP_";
      $strProp .= uc($prop->{package}) . "_" if ($prop->{package});
      $strProp .= uc($prop->{id});
      $strDtor .= "  clearNotification($strProp);\n";
      print $fhHeader "      $strProp,\n";
    }
    print $fhHeader "    } tProperty;\n\n";

    print $fhHeader "    void setNotification( tProperty );\n";
    print $fhHeader "    void clearNotification( tProperty);\n\n";

    foreach my $prop (@{$ifc->{properties}})
    {
      print $fhHeaderIfc "    virtual void onProperty( const YaProxyBase*, const ";
      print $fhHeaderIfc $prop->{package} . "::" if ($prop->{package});
      print $fhHeaderIfc $prop->{id} ."& ) = 0;\n";

#      print $fhHeader "    virtual void onProperty( const YaProxyBase*, const ";
#      print $fhHeader $prop->{package} . "::" if ($prop->{package});
#      print $fhHeader $prop->{id} ."& ) = 0;\n";

    }

  }

  print $fhHeader "\n";
  print $fhHeaderIfc "\n";
  print $fhSource "$strDtor\n}\n\n";

  # write notification section

  print $fhSource "void " . $IfcName ."Proxy::setNotification( " . $IfcName ."Proxy::tProperty prop )\n";
  print $fhSource "{\n}\n\n";


  print $fhSource "void " . $IfcName ."Proxy::clearNotification( " . $IfcName ."Proxy::tProperty prop )\n";
  print $fhSource "{\n}\n\n";


  foreach my $req (@{$ifc->{requests}})
  {
    print $fhHeader "    void request". $req->{id};
    print $fhHeader "( ";
    my $strPara;
    foreach my $para (@{$req->{para}})
    {
      $strPara .= " const ";
      $strPara .= $para->{package} . "::" if ($para->{package});
      $strPara .= "$para->{id}&,";
    }
    chop($strPara);
    print $fhHeader $strPara . ");\n";
  }

  print $fhHeader "\n";

  foreach my $resp (@{$ifc->{responses}})
  {
    print $fhHeaderIfc "    virtual void onResponse( const YaProxyBase*, const ";
    print $fhHeaderIfc $resp->{package} . "::" if ($resp->{package});
    print $fhHeaderIfc $resp->{id}. "& ) = 0;\n";
  }

  print $fhHeader "private:\n";
  print $fhHeader " " . $IfcName . "Proxy();\n";
  print $fhHeader "  I" . $IfcName . "ProxyHandler& mCallbackHandler;\n";

  foreach my $prop (@{$ifc->{properties}})
  {
    print $fhHeader "YaSUBImpl m" . $prop->{id} . ";\n";
  }

  print $fhHeader "};\n";
  print $fhHeaderIfc "};\n";


  print $fhHeader "}\n";
#  print $fhHeaderIfc "}\n";

  print $fhHeader "\n#endif\n";
  print $fhHeaderIfc "\n#endif\n";

  print $fhSource "\n}\n"; #closing namespace

#  print $fhSource "#include \"moc_" . $IfcName . "Proxy.cxx\"\n";

  close( $fhHeader);
  close( $fhHeaderIfc);
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
