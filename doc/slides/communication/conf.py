# -*- coding: utf-8 -*-
#

# Add any Sphinx extension module names here, as strings. They can be extensions
# coming with Sphinx (named 'sphinx.ext.*') or your custom ones.
extensions = ['sphinx.ext.todo', 'sphinx_revealjs']

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# General information about the project.
project = u'YaComponent Communication'
copyright = u'2014, Joerg Kreuzberger'

# The short X.Y version.
version = '24.1'
# The full version, including alpha/beta/rc tags.
release = '24.1.0'

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
exclude_patterns = ['_build']

# -- Options for HTML output ---------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
html_theme = 'default'

# Output file base name for HTML help builder.
htmlhelp_basename = 'YaComponentCommunicationdoc'


revealjs_style_theme = "moon"
revealjs_script_conf ={
    'hash': True,
    'keyboard': {
            '188': '() => setPresenterMode(true)'
    }
}


# revealjs_static_path = html_static_path
# revealjs_js_files = ['slides.js']
