#!perl -w

use strict;
use warnings;

#use Getopt::Long qw(:config debug);
use Getopt::Long;
use Pod::Usage;

use YaComponent;
use YaComponentParser;

my $optOk = GetOptions (
#            'rootpath=s' => \$ProBuild::RootPath,
 #           'debug' => sub{$ProBuild::currBuildCfg = 'debug'},
 #           'genmake!' => \$ProBuild::gCurrCfgRef->{'genmake'},
            'ifc=s' => \$YaComponentParser::gIfcFileName,
            'outcode=s' => \$YaComponentParser::gIfcCodeOutPath,
            'verbose' => \$YaComponent::gVerbose,
            'help' => sub{pod2usage(-verbose => 0);CORE::exit;},
            'man' => sub{pod2usage(-verbose => 1);CORE::exit;}
            );

if($optOk)
{
  # check if there are parameters to probuild that are not handled by getOpt
  # allowed is the name of a buildcfg file passed to probuild per drag and drop
  if ($ARGV[0])
  {
    YaComponent::printDbg("Unprocessed options available") ;
    foreach (@ARGV)
    {
      YaComponent::printDbg("$_");
    }
  }

  if(!defined $YaComponentParser::gIfcFileName)
  {
    YaComponent::printFatal("Missing definition of IFC file name, please use option --ifc");
  }
  else
  {
    YaComponentParser::init($YaComponentParser::gIfcFileName);
    YaComponentParser::readIfc($YaComponentParser::gIfcFileName);
  }

}

__END__

=head1 NAME

yacomponent - Component Ifc Parser and Code Generator

=head1 SYNOPSIS

yafsm [options]

 Options:
   --help             brief help message
   --man              detailed options description
   --verbose          verbose debug messages
   --ifc              IFC file name
   --outcode          path for code generation

=head1 OPTIONS

=over 8

=item B<--help>

Print a brief help message and exits.

=item B<--man>

Print detailed options descriptions and exit.

=item B<--verbose>

Print detailled debug messages. Only important through debugging of probuild

=item B<--ifc>

IFC File name.
XML IFC File description.

=head1 DESCRIPTION

B<This program> parses yacomponent ifc file description and generates code from it

=cut

