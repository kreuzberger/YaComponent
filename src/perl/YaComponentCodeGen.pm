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

 # if($YaComponent::gVerbose)
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
  print $fhHeader "#include \"YaComponentBase.h\"\n";

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

  print $fhHeader "class " . $CompName ."Impl: public YaComponentBase";
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

  foreach my $className (sort ( keys ( %classNameStubHash )))
  {
    $strCtor .= ", I" . $className . "StubHandler& r$className";
  }

  #chop($strCtor);

  print $fhHeader "$strCtor );\n";
  print $fhSource "$strCtor )\n";

  print $fhHeader "    virtual ~" . $CompName . "Impl() {}\n";

  foreach my $providedIfc (@{$comp->{provided}})
  {
    print $fhHeader "    void setConnectionPara" . $providedIfc->{id} . "( const char* pub, const char* req , int hwm = 0 );\n";
  }

  foreach my $usedIfc (@{$comp->{used}})
  {
    print $fhHeader "    void setConnectionPara" . $usedIfc->{id} . "( const char* sub, const char* req );\n";
  }

  print $fhHeader "    /// called after setting connections\n";
  print $fhHeader "    virtual void init() = 0;\n";

  print $fhHeader "  protected:\n";
  print $fhHeader "    virtual void timerEvent( QTimerEvent * event );\n";

  my $strCtorImpl="";

  $strCtorImpl .= " :YaComponentBase()";

  foreach my $usedIfc (@{$comp->{used}})
  {
    print $fhHeader "  " . $usedIfc->{classname} . "Proxy m" . $usedIfc->{id} . ";\n";
    $strCtorImpl .=  " , m" . $usedIfc->{id} . "( context, r$usedIfc->{classname} )\n";
  }

  foreach my $providedIfc (@{$comp->{provided}})
  {
    print $fhHeader "  " . $providedIfc->{classname} . "Stub m" . $providedIfc->{id} . ";\n";
    $strCtorImpl .=  " , m" . $providedIfc->{id} . "( context, r$providedIfc->{classname} )\n";
  }

  chop($strCtorImpl);

  print $fhSource "$strCtorImpl";
  print $fhSource "{\n\n}\n";


  foreach my $providedIfc (@{$comp->{provided}})
  {
    print $fhSource "void " . $CompName ."Impl::setConnectionPara" . $providedIfc->{id} . "( const char* pub, const char* req, int hwm )\n";
    print $fhSource "{\n";
    print $fhSource "  m" . $providedIfc->{id} . ".setConnectionPara( pub, req, hwm );\n";
    print $fhSource "}\n";
  }

  foreach my $usedIfc (@{$comp->{used}})
  {
    print $fhSource "void " . $CompName ."Impl::setConnectionPara" . $usedIfc->{id} . "( const char* sub, const char* req )\n";
    print $fhSource "{\n";
    print $fhSource "  m" . $usedIfc->{id} . ".setConnectionPara( sub, req );\n";
    print $fhSource "}\n";
  }

  print $fhSource "void " . $CompName . "Impl::timerEvent( QTimerEvent * event )\n";
  print $fhSource "{\n";
#  print $fhSource "  int key = 0;\n";
#  print $fhSource "  int size = 0;\n";
#  print $fhSource "  const char* msgData = 0;\n";
#  print $fhSource "  int iMsgCnt = 0;\n";
#  print $fhSource "  bool bMsgAvailable = true;\n";

#  print $fhSource "  while( bMsgAvailable )\n";
#  print $fhSource "  {\n";
#  print $fhSource "    int iBytesTotal = 0;\n";
#  print $fhSource "    int iBytes = 0;\n";

#  print $fhSource "    foreach( IYaConnection* pPtr, mConnectionHash )\n";
#  print $fhSource "    {\n";
#  print $fhSource "      int iBytes = pPtr->receive( key, size, msgData);\n";
#  print $fhSource "      iBytesTotal += iBytes;\n";
#  print $fhSource "      if( 0 < iBytes ) iMsgCnt++;\n";
#  print $fhSource "    }\n";
  foreach my $usedIfc (@{$comp->{used}})
  {
    print $fhSource "  m" . $usedIfc->{id} . ".receive();\n";
  }

#  print $fhSource "    if( 0 == iBytesTotal ) bMsgAvailable = false;\n";
#  print $fhSource "  }\n";
  print $fhSource "}\n";




#  print $fhSource "      {\n";
#  print $fhSource "        switch( key )\n";
#  print $fhSource "        {\n";
#  print Dumper($comp);
#  foreach my $usedIfc (@{$comp->{used}})
#  {

#    foreach my $resp (@{$usedIfc->{responses}})
#    {
#      my $strResp;
#      $strResp .= "RESP_";
#      $strResp .= uc($resp->{package}) . "_" if ($resp->{package});
#      $strResp .= uc($resp->{id});
#      print $fhSource "          case YaComponent::$usedIfc->{name}Proxy::$strResp:\n";
#      print $fhSource "          break;\n";
#    }
#
#  }
#
#  print $fhSource "          default:\n";
#  print $fhSource "            break;\n";
#  print $fhSource "        }\n";


#  foreach my $usedIfc (@{$comp->{used}})
#  {
#    print $fhSource "  m" . $usedIfc->{id} . ".receive( key, size, msgData );\n";
#  }

#  foreach my $providedIfc (@{$comp->{provided}})
#  {
#    print $fhSource "  m" . $providedIfc->{id} . ".receive( key, size, msgData );\n";
#  }





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
  print $fhHeader "#include \"I" . $IfcName . "StubHandler.h\"\n";
  print $fhHeader "#include \"YaStubBase.h\"\n";
  print $fhHeader "#include <stdio.h>\n";

  print $fhHeader "#include \"YaPUBImpl.h\"\n";



  print $fhHeader "namespace YaComponent {\n";
  print $fhHeaderIfc "namespace YaComponent {\n";
  print $fhSource "namespace YaComponent {\n";

  print $fhSource $IfcName . "Stub::" . $IfcName . "Stub( void* context, I" . $IfcName . "StubHandler& rCallbackHandler )\n";

  print $fhHeaderIfc "class I" . $IfcName ."StubHandler\n";
  print $fhHeaderIfc "{\n";

  print $fhHeader "class " . $IfcName ."Stub: public YaStubBase\n";
  print $fhHeader "{\n";
  print $fhHeader "  Q_OBJECT\n";
  print $fhHeader "  public:\n";
  print $fhHeaderIfc "  public:\n";
  print $fhHeader "    " . $IfcName . "Stub( void* context, I" . $IfcName . "StubHandler& );\n";
  print $fhHeader "    virtual ~" . $IfcName . "Stub();\n";
 # print $fhHeader "    void setConnectionPara( const char* pub, const char* req , int hwm = 0 );\n";

  print $fhHeaderIfc "    I" . $IfcName . "StubHandler() {}\n";
  print $fhHeaderIfc "    virtual ~I" . $IfcName . "StubHandler() {}\n";

  print $fhHeader "    enum KEYS {\n";

  foreach my $prop (@{$ifc->{properties}})
  {
    my $strProp;
    $strProp .= "PROP_";
    $strProp .= uc($prop->{package}) . "_" if ($prop->{package});
    $strProp .= uc($prop->{id});
    print $fhHeader "      $strProp,\n";
  }

  foreach my $req (@{$ifc->{requests}})
  {
    my $strReq;
    $strReq .= "REQ_";
    $strReq .= uc($req->{package}) . "_" if ($req->{package});
    $strReq .= uc($req->{id});
    print $fhHeader "      $strReq,\n";
  }

  foreach my $resp (@{$ifc->{responses}})
  {
    my $strResp;
    $strResp .= "RESP_";
    $strResp .= uc($resp->{package}) . "_" if ($resp->{package});
    $strResp .= uc($resp->{id});
    print $fhHeader "      $strResp,\n";
  }
  print $fhHeader "    };\n\n";

  foreach my $prop (@{$ifc->{properties}})
  {
    my $strMethod;
    $strMethod .= $prop->{package} . "::" if ($prop->{package});
    $strMethod .= $prop->{id};
    print $fhHeader "    void send(int key, const $strMethod&);\n";
  }

  foreach my $req (@{$ifc->{requests}})
  {
    print $fhHeaderIfc "    virtual void onRequest". $req->{id};
    print $fhHeaderIfc "( ";
    my $strPara;
    foreach my $para (@{$req->{para}})
    {
      $strPara .= " const ";
      $strPara .= $para->{package} . "::" if ($para->{package});
      $strPara .= "$para->{id}&";
    }
    print $fhHeaderIfc $strPara . " ) = 0;\n";
  }

  print $fhHeader "\n";

  foreach my $resp (@{$ifc->{responses}})
  {
#  print $fhHeaderIfc "    virtual void response( const QObject*, const ";
    print $fhHeader "    virtual void response( const YaStubBase*, const ";
    print $fhHeader $resp->{package} . "::" if ($resp->{package});
    print $fhHeader $resp->{id}. "& ) {}\n";
  }

  print $fhHeader "  protected:\n";

  print $fhSource " : YaStubBase( context )\n";
  print $fhSource " , mCallbackHandler( rCallbackHandler )\n";

#  foreach my $prop (@{$ifc->{properties}})
#  {
#    print $fhHeader "    YaPUBImpl m" . $prop->{id} . ";\n";
#    print $fhSource " , m" . $prop->{id} . "( 0 )\n";
#  }

#  print $fhHeader "    YaPUBImpl mPublisher;\n";
 # print $fhSource " , mPublisher( context )\n";


  print $fhSource "{\n\n}\n\n";


  print $fhSource $IfcName . "Stub::~" . $IfcName . "Stub()\n";
  print $fhSource "{\n\n}\n\n";

  foreach my $prop (@{$ifc->{properties}})
  {
    my $strMethod;
    $strMethod .= $prop->{package} . "::" if ($prop->{package});
    $strMethod .= $prop->{id};
    print $fhSource "void " . $IfcName . "Stub::send(int key, const $strMethod& rMessage )\n{\n";
    print $fhSource "  mPublisher.send(key, rMessage );\n";
    print $fhSource "}\n";
  }

#  print $fhSource "void " . $IfcName . "Stub::setConnectionPara( const char* pub, const char* req, int hwm  )\n";
#  print $fhSource "{\n";
#  print $fhSource "  mPublisher.setConnectionPara( pub, req, hwm );\n";
#  print $fhSource "}\n";



  print $fhHeader "    I" . $IfcName . "StubHandler& mCallbackHandler;\n";
  print $fhHeader "\n};\n";
  print $fhHeaderIfc "\n};\n";

  print $fhHeader "\n}\n";
  print $fhHeaderIfc "\n}\n";
  print $fhSource "}\n";

  print $fhHeader "\n#endif\n";
  print $fhHeaderIfc "\n#endif\n";

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
  print $fhSource ": YaProxyBase( context )\n";
  print $fhSource ", mCallbackHandler( callbackHandler )\n";
#  foreach my $prop (@{$ifc->{properties}})
#  {
#    print $fhSource ", m" . $prop->{id} . "(context)\n";
#  }
#  print $fhSource ", mSubscriber(context)\n";

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
 # print $fhHeader "    void setConnectionPara( const char* sub, const char* req);\n";
  print $fhHeader "    virtual int receive();\n";

  print $fhHeaderIfc "\nclass YaProxyBase;\n\n";

  print $fhHeaderIfc "class I" . $IfcName ."ProxyHandler\n";
  print $fhHeaderIfc "{\n";
  print $fhHeaderIfc "  public:\n";
  print $fhHeaderIfc "    I" . $IfcName . "ProxyHandler() {}\n";
  print $fhHeaderIfc "    virtual ~I" . $IfcName . "ProxyHandler() {}\n\n";

  #print $fhHeader "  protected:\n";


  print $fhHeader "    enum KEYS {\n";
  foreach my $prop (@{$ifc->{properties}})
  {
    my $strProp;
    $strProp .= "PROP_";
    $strProp .= uc($prop->{package}) . "_" if ($prop->{package});
    $strProp .= uc($prop->{id});
    $strDtor .= "  clearNotification($strProp);\n";
    print $fhHeader "      $strProp,\n";
  }
  foreach my $req (@{$ifc->{requests}})
  {
    my $strReq;
    $strReq .= "REQ_";
    $strReq .= uc($req->{package}) . "_" if ($req->{package});
    $strReq .= uc($req->{id});
    print $fhHeader "      $strReq,\n";
  }
  foreach my $resp (@{$ifc->{responses}})
  {
    my $strResp;
    $strResp .= "RESP_";
    $strResp .= uc($resp->{package}) . "_" if ($resp->{package});
    $strResp .= uc($resp->{id});
    print $fhHeader "      $strResp,\n";
  }
  print $fhHeader "    };\n\n";



#    print $fhHeader "    void setNotification( int  );\n";
#    print $fhHeader "    void clearNotification( int );\n\n";

  foreach my $prop (@{$ifc->{properties}})
  {
    print $fhHeaderIfc "    virtual void onProperty( const YaProxyBase*, const ";
    print $fhHeaderIfc $prop->{package} . "::" if ($prop->{package});
    print $fhHeaderIfc $prop->{id} ."& ) = 0;\n";

#      print $fhHeader "    virtual void onProperty( const YaProxyBase*, const ";
#      print $fhHeader $prop->{package} . "::" if ($prop->{package});
#      print $fhHeader $prop->{id} ."& ) = 0;\n";

  }


  print $fhHeader "\n";
  print $fhHeaderIfc "\n";
  print $fhSource "$strDtor\n}\n\n";

  # write notification section

  #print $fhSource "void " . $IfcName ."Proxy::setNotification( " . $IfcName ."Proxy::tProperty prop )\n";
#  print $fhSource "void " . $IfcName ."Proxy::setNotification( int key )\n";
#  print $fhSource "{\n";
#  print $fhSource "  mSubscriber.setNotification( key );\n";
#  print $fhSource "}\n\n";


#  print $fhSource "void " . $IfcName ."Proxy::clearNotification( " . $IfcName ."Proxy::tProperty prop )\n";
#  print $fhSource "void " . $IfcName ."Proxy::clearNotification( int key )\n";
#  print $fhSource "{\n";
#  print $fhSource "  mSubscriber.clearNotification( key );\n";
#  print $fhSource "}\n\n";

 # print $fhSource "void " . $IfcName ."Proxy::setConnectionPara( const char* sub, const char* req )\n";
 # print $fhSource "{\n";
 # print $fhSource "  mSubscriber.setConnectionPara( sub, req );\n";
 # print $fhSource "}\n\n";



  foreach my $req (@{$ifc->{requests}})
  {
    print $fhHeader "    void request". $req->{id};
    print $fhSource "void " . $IfcName ."Proxy::request". $req->{id};
    print $fhHeader "( ";
    print $fhSource "( ";
    my $strPara;
    foreach my $para (@{$req->{para}})
    {
      $strPara .= " const ";
      $strPara .= $para->{package} . "::" if ($para->{package});
      $strPara .= "$para->{id}&,";
    }
    chop($strPara);
    print $fhHeader $strPara . ");\n";
    print $fhSource $strPara . " msg )\n{\n";
    print $fhSource "  mSubscriber.request( REQ_";
    print $fhSource uc($req->{package}) . "_" if ($req->{package});
    print $fhSource uc($req->{id}) . ", msg );\n";
    print $fhSource "}\n";
  }

  print $fhSource "int " . $IfcName ."Proxy::receive()\n";
  print $fhSource "{\n";
  print $fhSource "  int key = 0;\n";
  print $fhSource "  int size = 0;\n";
  print $fhSource "  const char* msgData = 0;\n";
  print $fhSource "  int iMsgCnt = 0;\n";
  print $fhSource "  bool bMsgAvailable = true;\n";

  print $fhSource "  while( bMsgAvailable )\n";
  print $fhSource "  {\n";
  print $fhSource "    int iBytesTotal = 0;\n";

  print $fhSource "    int iBytes = mSubscriber.receive(key, size, msgData );\n";
  print $fhSource "    iBytesTotal += iBytes;\n";
  print $fhSource "    if( 0 < iBytes ) iMsgCnt++;\n";

  print $fhSource "    switch( key )\n";
  print $fhSource "    {\n";
  foreach my $resp (@{$ifc->{responses}})
  {
    my $strResp;
    $strResp .= "RESP_";
    $strResp .= uc($resp->{package}) . "_" if ($resp->{package});
    $strResp .= uc($resp->{id});
    print $fhSource "    case $strResp:\n";
    print $fhSource "      m$resp->{id}.ParseFromArray(msgData, size);\n";
    print $fhSource "      mCallbackHandler.onResponse( this, m$resp->{id} );\n";
    print $fhSource "      break;\n";
  }

  foreach my $prop (@{$ifc->{properties}})
  {
    my $strProp;
    $strProp .= "PROP_";
    $strProp .= uc($prop->{package}) . "_" if ($prop->{package});
    $strProp .= uc($prop->{id});
    print $fhSource "    case $strProp:\n";
    print $fhSource "      break;\n";
  }

  print $fhSource "    default:\n";
  print $fhSource "      break;\n";
  print $fhSource "    }\n";


  print $fhSource "  }\n";

  print $fhSource "}\n";

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
    print $fhHeader "  " . $prop->{package} . "::" if ($prop->{package});
    print $fhHeader $prop->{id};
    print $fhHeader " m" . $prop->{id} . ";\n";
  }
  foreach my $prop (@{$ifc->{responses}})
  {
    print $fhHeader "  " . $prop->{package} . "::" if ($prop->{package});
    print $fhHeader $prop->{id};
    print $fhHeader " m" . $prop->{id} . ";\n";
  }
 #   print $fhHeader "YaSUBImpl mSubscriber;\n";

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
