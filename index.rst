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

.. toctree::
    :hidden:

    book/index
    cookbook/index
    bundles/index
    components/index
    contributing/index
    reference/index

Book
----

This is the Symfony CMF bible. It's the reference for any user of the CMF, who
will typically want to keep this close at hand.

.. toctree::
    :maxdepth: 1

    book/installation
    book/simplecms
    book/routing
    book/static_content
    book/structuring_content

Cookbook
--------

Special solutions for specific use cases that go beyond standard usage.

Want to know more about the CMF and how each part can be configured? There's a tutorial for each one.

.. toctree::
    :maxdepth: 1

    cookbook/database/choosing_storage_layer
    cookbook/editions/cmf_sandbox
    cookbook/editions/cmf_core
    cookbook/installing_configuring_doctrine_phpcr_odm
    cookbook/installing_configuring_inline_editing
    cookbook/creating_cms_using_cmf_and_sonata
    cookbook/using_blockbundle_and_contentbundle
    cookbook/handling_multilang_documents

Bundles
-------

Looking for some in depth information about a CMF Bundle? Want a list of all the
configuration options of a bundle? Want to know if you can use a bundle independently and how
to do it? In this case the reference is the right place for you.

.. toctree::
	:maxdepth: 1

	bundles/block/index
	bundles/blog
	bundles/content/index
	bundles/core/index
	bundles/create
	bundles/phpcr_odm
	bundles/media/index
	bundles/menu/index
	bundles/routing/index
	bundles/routing_auto
	bundles/search/index
	bundles/simple_cms/index
	bundles/doctrine_phpcr_admin
	bundles/tree_browser

Components
----------

Looking for some information about the low level components of the CMF?

.. toctree::
	:maxdepth: 1

	components/routing/introduction
	components/testing

Reference
---------

Ever wondered what configuration options you have available to you in files
such as ``app/config/config.yml``? In this section, all available
configuration is broken down per bundle.

.. toctree::
    :maxdepth: 1

    reference/configuration/block
    reference/configuration/content
    reference/configuration/core
    reference/configuration/media
    reference/configuration/routing
    reference/configuration/search
    reference/configuration/simple_cms

Contributing
------------

.. toctree::
	:maxdepth: 1

	contributing/code
	contributing/bundles
	contributing/releases
	contributing/license

.. _`decoupled CMS`: http://decoupledcms.org
.. _`Symfony2`: http://symfony.com
.. _`about`: http://cmf.symfony.com/about
.. _`Documentation planning`: https://github.com/symfony-cmf/symfony-cmf/wiki/Documentation-Planning
.. _`Symfony Content Management Framework`: http://cmf.symfony.com
.. _`documentation is hosted on github`: https://github.com/symfony-cmf/symfony-cmf-docs
