.. index::
    single: Static Content; Book
    single: CmfContentBundle

Static Content
==============

Concept
-------

At the heart of every CMS stands the content, an abstraction that the
publishers can manipulate and that will later be presented to the page's
users. The content's structure greatly depends on the project's needs, and it
will have a significant impact on future development and use of the platform.

The ContentBundle provides a basic implementation of a content document classes,
including support for multiple languages and association to Routes.

Static Content
--------------

The ``StaticContent`` class declares the basic content's structure. Its structure
is very similar to the ones used on Symfony2's ORM systems. Most of its
fields are self explanatory and are what you would expect from a basic CMS:
title, body, publishing information and a parent reference, to accommodate a
tree-like hierarchy. It also includes a Block reference (more on that later).

This document class implements three interfaces that enable additional functionality:

* ``RouteReferrersInterface`` means that the content has associated Routes.
* ``PublishTimePeriodInterface`` means that the content has publishing and
   unpublishing dates, which will be handled by Symfony CMF's core to
   determine whether or not to display the content from ``StaticContent``
* ``PublishableInterface`` means that the content has a boolean flag,
   which will be handled by Symfony CMF's core to
   determine whether or not to display the content from ``StaticContent``.

Content Controller
------------------

A controller is also included that can render either of the above content
document types. Its single action, ``indexAction``, accepts a content
instance and optionally the path of the template to be used for rendering.
If no template path is provided, it uses a pre-configured default.

The controller action also takes into account the document's publishing status
and language (for ``MultilangStaticContent``). Both the content instance and the
optional template are provided to the controller by the ``DynamicRouter`` of
the RoutingBundle. More information on this is available on the
:ref:`Routing system getting started page <start-routing-linking-a-route-with-a-model-instance>`
page.

Admin Support
-------------

The last component needed to handle the included content types is an
administration panel. Symfony CMF can optionally support
SonataDoctrinePHPCRAdminBundle_, a back office generation tool.

In ContentBundle, the required administration panels are already declared in
the ``Admin`` folder and configured in ``Resources/config/admin.xml``, and
will automatically be loaded if you install the SonataDoctrinePHPCRAdminBundle
(refer to :doc:`../tutorial/sonata-admin` for
instructions on that).

Configuration
-------------

The ContentBundle also supports a set of optional configuration parameters. Refer to
:doc:`../bundles/content/index` for the full configuration reference.

Final Thoughts
--------------

While this small bundle includes some vital components to a fully working CMS,
it often will not provide all you need. The main idea behind it is to provide
developers with a small and easy to understand starting point you can extend
or use as inspiration to develop your own content types, Controllers and Admin
panels.

.. _SonataDoctrinePHPCRAdminBundle: https://github.com/sonata-project/SonataDoctrinePhpcrAdminBundle
