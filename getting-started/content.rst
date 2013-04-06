.. index::
    single: Content, SymfonyCmfContentBundle

Content
=======

Concept
-------

At the heart of every CMS stands the content, an abstraction that the publishers
can manipulate and that will later be presented to the page's users. The content's
structure greatly depends on the project's needs, and it will have a significant
impact on future development and use of the platform.

Symfony CMF SE comes with the ``ContentBundle``: a basic implementation of a
content structure, including support for multiple languages and database storage
of Routes.

Static Content
--------------

The ``StaticContent`` declares the basic content's structure. Its structure
is very similar to the ones used on Symfony2's ORM systems, and most of its
fields are self explanatory, and are what you would expect from a basic CMS:
title, body, publishing information and a parent reference, to accommodate
a tree-like hierarchy. It also includes a Block reference (more on that later).

The two implemented interfaces reveal two of the features included in this
implementation:

- ``RouteAwareInterface`` means that the content has associated Routes.

- ``PublishWorkflowInterface`` means that the content has publishing and
   unpublishing dates, which will be handled by Symfony CMF's core to determine
   access.


Multilang Static Content
------------------------

The ``MultilangStaticContent`` extends ``StaticContent``, offering the same
functionality with multi language support. It specifies which fields are to
be translated (``title``, ``body`` and ``tags``) as well as a variable to
declare the locale.

It also specifies the translation strategy:

.. configuration-block::

    .. code-block:: php

       /**
       * @PHPCRODM\Document(translator="child", referenceable=true)
       */

For information on the available translation strategies, refer to the Doctrine
page regarding `Multilanguage support in PHPCR-ODM <http://docs.doctrine-project.org/projects/doctrine-phpcr-odm/en/latest/reference/multilang.html>`_


Content Controller
------------------

To handle both content types, a ``Controller`` is also included. Its inner
workings are pretty straightforward: it accepts a content instance and optionally
a template to render it. If none is provided, it uses a pre-configured default.
It also takes into account the document's publishing status and multi language.
Both the content instance and the optional template are provided to the Controller
by the ``DynamicRouter`` of the ``RoutingExtraBundle``. More information on this is
available on the :ref:`Routing system getting started page <routing-linking-a-route-with-a-model-instance>`
page.

Admin Support
-------------

The last component needed to handle the included content types is an administration
panel. Symfony CMF can optionally support `SonataDoctrinePHPCRAdminBundle <https://github.com/sonata-project/SonataDoctrinePhpcrAdminBundle>`_
, a back office generation tool. For more information about it, please refer
to the bundle's `documentation section <https://github.com/sonata-project/SonataDoctrinePhpcrAdminBundle/tree/master/Resources/doc>`_.

In ``ContentBundle``, the required administration panels are already declared
in the ``Admin`` folder and configured in ``Resources/config/admin.xml``,
and will automatically be loaded if you install ``SonataDoctrinePHPCRAdminBundle``
(refer to :doc:`../tutorials/creating-cms-using-cmf-and-sonata` for instructions
on that).

Configuration
-------------

The bundle also supports a set of optional configuration parameters. Refer
to :doc:`../bundles/content` for the full configuration reference.

Final Thoughts
--------------

While this small bundle includes some vital components to a fully working
CMS, it often will not provide all you need. The main idea behind it is to
provide developers with a small and easy to understand starting point you can
extend or use as inspiration to develop your own content types, Controllers and
Admin panels.
