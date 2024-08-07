# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))

import sphinx_rtd_theme # ReadTheDocs theme. More information available at: https://sphinx-rtd-theme.readthedocs.io/en/latest/installing.html 

# -- Project information -----------------------------------------------------

project = 'BeVolt BPS'
copyright = '2020, LHR Solar'
author = 'Clark Poon'
author = 'Manthan Upadhyaya'
author = 'Sugam Arora'

# The full version, including alpha/beta/rc tags
release = '0.0'

# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
	"sphinx_rtd_theme", # RTD theme extension
	"sphinx.ext.autosectionlabel" # extension for hyperlinks
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = []


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'sphinx_rtd_theme' # Enable RTD theme

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

#Ignore warnings for duplicate labels since we use the same label for multiple sections
#This means that if you are trying to refer to a label, it might not be possible
suppress_warnings = ['autosectionlabel.*']

#use index.rst
master_doc = 'index'

def setup(app):
    app.add_css_file('my_theme.css')