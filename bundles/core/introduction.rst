.. index::
    single: Core; Bundles

CoreBundle
==========

    This bundle provides common functionality, helpers and utilities for the
    other CMF bundles.

One of the provided features is an interface and implementation of a publish
workflow checker with an accompanying interface that models can implement if
they want to support this checker.

Furthermore, it provides a Twig helper exposing several useful functions for
Twig templates to interact with PHPCR-ODM documents.

Finally, most of its configuration settings are automatically applied as
defaults for most of the other CMF Bundles.

Installation
------------

You can install this bundle `with composer`_ using the
`symfony-cmf/core-bundle`_ package.

Sections
--------

* :doc:`publish_workflow`
* :doc:`dependency_injection_tags`
* :doc:`templating`
* :doc:`persistence`
* :doc:`configuration`

.. _`symfony-cmf/core-bundle`: https://packagist.org/packages/symfony-cmf/core-bundle
.. _`with composer`: https://getcomposer.org
