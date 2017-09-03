.. index::
    single: Resource; Bundles
    single: ResourceBundle

ResourceBundle
==============

    The ResourceBundle provides object resource location for CMF documents,
    originally based on Puli_.

.. caution::

    As the Puli project is stalled in beta-phase, this bundle currently ships a
    light version of Puli.

Installation
------------

You can install this bundle `with composer`_ using the
`symfony-cmf/resource-bundle`_ package.

Usage
-----

This bundle provides a generic layer on top of the persistence layer used in
the CMF. Documents can be located using *resource repositories*. These
repositories can use a variety of backends (Doctrine PHPCR-ODM and PHPCR
repositories are provided). Read more about repositories in ":doc:`repositories`".

The retrieved resources come with description information (e.g. the path and
some other information). This description is provided by *description
enhancers*. The :doc:`TreeBrowserBundle <../tree_browser/introduction>` for
instance ships with an enhancer to add paths to document icons.  Read more
about adding your own description enhancers in :doc:`description_enhancers`.

.. _Puli: http://puli.io/
