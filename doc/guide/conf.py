# -*- coding: utf-8 -*-
#
# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.

import os

extensions = ['sphinxcontrib.plantuml']

root_dir = os.path.dirname(os.path.abspath(os.path.realpath(__file__) + "/../../"))

plantuml = 'java -jar ' + f'{root_dir}/bin/plantuml.jar'
# The master toctree document.
master_doc = 'UserGuide'

# General information about the project.
project = u'YaComponent UserGuide'
copyright = u'2014, Joerg Kreuzberger'

version = '24.1'
# The full version, including alpha/beta/rc tags.
release = '24.1.0'

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
exclude_patterns = ['_build']

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
html_theme = 'nature'

# Output file base name for HTML help builder.
htmlhelp_basename = 'YaComponentUserGuidedoc'

