.. index::
    single: Core; Bundles

CoreBundle
==========

    This bundle provides common functionality, helpers and utilities for the
    other CMF bundles.

The major features are:

* Publish workflow interfaces and publish workflow checker to handle whether
  documents should be visible on the site or not;
* Twig helper exposing several useful functions to interact with PHPCR-ODM
  documents from within Twig templates;
* Propagate default configuration to the other CMF bundles.

Installation
------------

You can install this bundle `with composer`_ using the
`symfony-cmf/core-bundle`_ package.

Read On
-------

* :doc:`publish_workflow`
* :doc:`templating`
* :doc:`forms`
* :doc:`persistence`
* :doc:`configuration`
* :doc:`Sonata Admin integration <../sonata_phpcr_admin_integration/core>`

.. _`symfony-cmf/core-bundle`: https://packagist.org/packages/symfony-cmf/core-bundle
.. _`with composer`: https://getcomposer.org
