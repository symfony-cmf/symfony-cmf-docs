Symfony CMF Documentation
=========================

Welcome to the official documentation of the `Symfony Content Management Framework`_.

The Symfony2 Content Management Framework  project is organized by the Symfony
community and has several sponsoring companies and prominent open source leaders
implementing the philosophy of the decoupled CMS. You can read learn more about the
project on the about page.

This documentation is currently in development and far from complete. See `Documentation planning`_ for an
overview of the work left to do. Want to help? Thank you, all help greatly appreciated! The source of
the `documentation is hosted here`_.

Mission Statement
-----------------

    The Symfony CMF project makes it easier for developers to add CMS functionality to
    applications built with the Symfony2 PHP framework. Key development principles for
    the provided set of bundles are scalability, usability, documentation and testing.

Why another CMS?
----------------

Actually we consider this project to be a CMF, a content management framework, rather
than a CMS, a content management system. The reason is that we are only providing tools
to build a custom CMS. There are obviously many CMS solutions available today already,
but they tend to be tailored first and foremost towards end users and also many tend
to carry a lot of legacy baggage which make them less than ideal for truly custom
development like what is possible with `Symfony2`_.

What is our target audience?
----------------------------

There are basically to main target audiences:

 * Developers that have build a custom application with Symfony2 and just need to also
   support a few content pages for things like the about/contact pages or a news section
 * Developers that need to build a highly customized authoring and content delivery
   solution that no out of the box CMS can properly provide through customization.

Tutorials
---------

Learning or want to learn the CMF? Want to know if the CMF fits your project? The book tells you all.

.. toctree::
	:maxdepth: 1

	tutorials/installing-configuring-cmf
	tutorials/choosing-a-storage-layer
	tutorials/installing-configuring-doctrine-phpcr-odm
	tutorials/installing-configuring-simple-cms
	tutorials/installing-configuring-inline-editing
	tutorials/creating-cms-using-cmf-and-sonata
	tutorials/using-blockbundle-and-contentbundle

Bundles
-------

Looking for some in depth information about a CMF Bundle? Want a list of all the
configuration options of a bundle? Want to know if you can use a bundle independently and how
to do it? The reference is the right place to search.

.. toctree::
	:maxdepth: 1

	bundles/block
	bundles/content
	bundles/core
	bundles/create
	bundles/phpcr-odm
	bundles/menu
	bundles/routing-extra
	bundles/search
	bundles/simple-cms
	bundles/doctrine_phpcr_admin
	bundles/tree
	bundles/tree-browser

Cookbook
--------

Special solutions for special needs that go beyond standard usage.

.. toctree::
	:maxdepth: 1

	cookbook/phpcr-odm-custom-documentclass-mapper
	cookbook/using-a-custom-route-repository

Components
----------

Looking for some for information about the low level components of the CMF?

.. toctree::
	:maxdepth: 1

	components/routing

Contributing
------------

.. toctree::
	:maxdepth: 1

	contributing/code
	contributing/license

.. _`Symfony2`: http://symfony.com
.. _`Documentation planning`: https://github.com/symfony-cmf/symfony-cmf/wiki/Documentation-Planning
.. _`Symfony Content Management Framework`: http://cmf.symfony.com
.. _`documentation is hosted here`: https://github.com/symfony-cmf/symfony-cmf-docs
