Symfony CMF Documentation
=========================

The Symfony2 Content Management Framework (CMF) project is organized by the Symfony
community and has several sponsoring companies and prominent open source leaders
implementing the philosophy of the `decoupled CMS`_. You can learn more about the
project on the `about`_ page.

This documentation is currently in development and far from complete. See `Documentation planning`_
for an overview of the work left to do. Want to help? Thank you, all help greatly appreciated!
The source of the `documentation is hosted on github`_.

Mission Statement
-----------------

    The Symfony CMF project makes it easier for developers to add CMS functionality to
    applications built with the Symfony2 PHP framework. Key development principles for
    the provided set of bundles are scalability, usability, documentation and testing.

Why another CMS?
----------------

Actually we consider this project to be a CMF, a **content management framework**, rather
than a CMS, a content management system. The reason is that we are only **providing the tools
to build a custom CMS**. There are clearly many CMS solutions available already,
but they tend to be monolithic packages tailored towards end users. Many carry a certain
amount of legacy baggage which make them less than **ideal for developing highly
custom applications**, in contrast to what is possible with `Symfony2`_.

What is our target audience?
----------------------------

There are basically two main target audiences:

#. Developers who have built an existing custom application with Symfony2 and need a fast
   way to add support for content management. Be it sophisticated CMS features like semantic
   content, inline editing, multi-channel delivery etc. or just a few content pages for things
   like the about/contact pages.

#. Developers that need to build a highly customized authoring and content delivery
   solution that no out-of-the-box CMS can properly provide through customization alone.

Requirements
------------

* `Symfony2 requirements`_
* MySQL >= 5.1.5 (if using ``jackalope-doctrine-dbal`` with MySQL connection)

Getting started
---------------

Just started learning about the CMF? Want to know if the CMF fits your project? Start here.

.. toctree::
	:hidden:

	getting_started/index

.. include:: getting_started/map.rst.inc


Tutorials
---------

Want to know more about the CMF and how each part can be configured? There's a tutorial for each one.

.. toctree::
	:hidden:

	tutorials/index

.. include:: tutorials/map.rst.inc

Bundles
-------

Looking for some in depth information about a CMF Bundle? Want a list of all the
configuration options of a bundle? Want to know if you can use a bundle independently and how
to do it? In this case the reference is the right place for you.

.. toctree::
	:hidden:

	bundles/index

.. include:: bundles/map.rst.inc

Cookbook
--------

Special solutions for specific use cases that go beyond standard usage.

.. toctree::
	:hidden:

	cookbook/index

.. include:: cookbook/map.rst.inc

Components
----------

Looking for some information about the low level components of the CMF?

.. toctree::
	:hidden:

	components/index

.. include:: components/map.rst.inc

Contributing
------------

.. toctree::
	:hidden:

	contributing/index

.. include:: contributing/map.rst.inc

.. _`decoupled CMS`: http://decoupledcms.org
.. _`Symfony2`: http://symfony.com
.. _`Symfony2_requirements`: http://symfony.com/doc/current/reference/requirements.html
.. _`about`: http://cmf.symfony.com/about
.. _`Documentation planning`: https://github.com/symfony-cmf/symfony-cmf/wiki/Documentation-Planning
.. _`Symfony Content Management Framework`: http://cmf.symfony.com
.. _`documentation is hosted on github`: https://github.com/symfony-cmf/symfony-cmf-docs
