Symfony CMF Documentation
=========================

The Symfony2 Content Management Framework (CMF) project is organized by the Symfony
community and has several sponsoring companies and prominent open source leaders
implementing the philosophy of the `decoupled CMS`_.

.. caution::

    This documentation is still in development. The bundles and reference sections
    are updated for the 1.0 release, but the book is still a bit sparse. Want to
    help? Thank you, all help greatly appreciated! The source of the
    `documentation is hosted on github`_.

.. toctree::
    :hidden:

    quick_tour/index
    book/index
    bundles/index
    components/index
    contributing/index
    reference/index
    cookbook/index

Quick Tour
----------

The best way to get started with the Symfony CMF is by reading the Quick Tour.
This will guide you through the code, philosophy and architecture of the
Symfony CMF project.

.. toctree::
    :maxdepth: 1

    quick_tour/the_big_picture
    quick_tour/the_model
    quick_tour/the_router
    quick_tour/the_third_party_bundles

Book
----

This is the Symfony CMF bible. It's the reference for any user of the CMF, who
will typically want to keep this close at hand.

.. toctree::
    :maxdepth: 1

    book/installation
    book/simplecms
    book/routing
    book/database_layer
    book/static_content
    book/structuring_content

Bundles
-------

Looking for some in depth information about a CMF Bundle? Want to know if you
can use a bundle independently and how to do it? In this case the bundles
section is the right place for you!

:doc:`Browse the bundles <bundles/index>`

Components
----------

The Symfony CMF believes in a concept where bundles integrate library
(components) into the framework. The components can be used in any PHP
project, even when they are not using Symfony.

:doc:`Browse the components <components/index>`

Reference
---------

Ever wondered what configuration options you have available to you in files
such as ``app/config/config.yml``? In this section, all available
configuration is broken down per bundle.

:doc:`Browse the reference <reference/index>`

Cookbook
--------

The cookbook consist of articles about advanced concepts of the Symfony CMF.

:doc:`Browse the cookbook <cookbook/index>`

Contributing
------------

Do you want to contribute to the Symfony CMF? Start reading these articles!

:doc:`Browse the contributing guide <contributing/index>`

.. _`decoupled CMS`: http://decoupledcms.org
.. _`Symfony2`: http://symfony.com
.. _`Documentation planning`: https://github.com/symfony-cmf/symfony-cmf/wiki/Documentation-Planning
.. _`Symfony Content Management Framework`: http://cmf.symfony.com
.. _`documentation is hosted on github`: https://github.com/symfony-cmf/symfony-cmf-docs
