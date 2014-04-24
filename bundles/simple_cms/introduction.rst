.. index::
    single: SimpleCms; Bundles
    single: SimpleCmsBundle

SimpleCmsBundle
===============

    The SimpleCmsBundle provides a simplistic CMS on top of the CMF components
    and bundles.

While the core CMF components focus on flexibility, the simple CMS trades away
some of that flexibility in favor of simplicity.

The SimpleCmsBundle provides a solution to easily map content, routes and menu
items based on a single tree structure in the content repository.

You can find an introduction to the bundle in the
:doc:`Getting started <../../book/simplecms>` section.

The `CMF website`_ is an application using the SimpleCmsBundle.

.. tip::

    For a simple example installation of the bundle check out the
    `Symfony CMF Standard Edition`_

Installation
------------

You can install the bundle in 2 different ways:

* Use the official Git repository (https://github.com/symfony-cmf/SimpleCmsBundle);
* Install it via Composer (``symfony-cmf/simple-cms-bundle`` on `Packagist`_).

Sections
--------

* :doc:`multilang`
* :doc:`rendering`
* :doc:`extending_page_class`

.. _`Symfony CMF Standard Edition`: https://github.com/symfony-cmf/symfony-cmf-standard
.. _`CMF website`: https://github.com/symfony-cmf/symfony-cmf-website/
.. _`Packagist`: https://packagist.org/packages/symfony-cmf/simple-cms-bundle
