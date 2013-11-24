.. index::
    single: Core; Bundles

CoreBundle
==========

    This bundle provides common functionality, helpers and utilities for the
    other CMF bundles.

One of the provided features is an interface and implementation of a publish
workflow checker with an accompanying interface that models can implement if
they want to support this checker.

Furthermore, it provides a twig helper exposing several useful functions for
twig templates to interact with PHPCR-ODM documents.

Finally, most of its configuration settings are automatically applied as
defaults for most of the other CMF Bundles.

Installation
------------

You can install the bundle in 2 different ways:

* Use the official Git repository (https://github.com/symfony-cmf/CoreBundle);
* Install it via Composer (``symfony-cmf/core-bundle`` on `Packagist`_).

Sections
--------

* :doc:`publish_workflow`
* :doc:`dependency_injection_tags`
* :doc:`templating`
* :doc:`persistence`

.. _`Packagist`: https://packagist.org/packages/symfony-cmf/core-bundle
