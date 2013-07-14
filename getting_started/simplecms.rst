.. index::
    single: SimpleCMS; Getting Started
    single: CmfSimpleCMSBundle

SimpleCMS
=========

Concept
-------

In the previous documentation pages all the basic components of Symfony CMF
have been analysed: the :doc:`routing` that allows you to associate URLs with
your :doc:`content`, which users can browse using a :doc:`menu`.

These three components complement each other but are independent: they work
without each other, allowing you to choose which ones you want to use, extend
or ignore. In some cases, however, you might just want a simple implementation
that gathers all those capabilities into a ready-to-go package. For that
purpose, the SimpleCMSBundle was created.

SimpleCMSBundle
---------------

The SimpleCMSBundle is implemented on top of most of the other Symfony CMF
Bundles, combining them into a functional CMS. It is a simple solution, but you
will find it very useful when you start implementing your own CMS using
Symfony CMF. Whether you decide to extend or replace it, it's up to you, but
in both cases, it's a good place to start developing your first CMS.

Page Document
~~~~~~~~~~~~~

Instead of separate documents for the content, routing and the menu system,
the SimpleCMSBundle provides the ``Page`` document which provides all those
roles in one class:

* It has properties for title and text body;
* It extends the ``Route`` class from the CMF ``RoutingBundle`` to work
  with the CMF router component, returning ``$this`` in
  ``getRouteContent()``;
* It implements the ``RouteAwareInterface`` with ``getRoutes`` simply
  returning ``array($this)`` to allow the CMF router to generate the URL
  to a page;
* It implements ``NodeInterface``, which means it can be used by
  CMF ``MenuBundle`` to generate a menu structure;
* It implements the ``PublishWorkflowInterface`` to be used with the
  :ref:`publish workflow checker <bundle-core-publish_workflow>`.

Here's how that works in practice:

* The routing component receives a request that it matches to a ``Route``
  instance loaded from persistent storage. That ``Route`` is a ``Page``
  instance;
* The route enhancer asks the page for its content and will receive ``$this``,
  putting the page into the request attributes;
* Other route enhancers determine the controller to use with this class
  and optionally the template to use (either a specific template stored with
  the page or one configured in the application configuration for the
  SimpleCmsBundle);
* The controller renders the page using the template, usually generating
  HTML content.
* The template might also render the menu, which will load all Pages and
  build a menu with them.

This three-in-one approach is the key concept behind the bundle.

MultilangPage
~~~~~~~~~~~~~

As you would expect, a multilanguage version of ``Page`` is also included.
``MultilangPage`` defines a ``locale`` variable and which fields will be
translated (``title``, ``label`` and ``body``). It also includes
``getStaticPrefix()`` to handle the path prefix of the ``Page``. This is part
of the route handling mechanism, and will be discussed below.

The ``MultilangPage`` class uses the ``attribute`` strategy for translation:
several translations can coexist in the same database entry, and several
translated versions of each field can be stored as different attributes in that
same entry.

As the routing is not separated from the content, it is not possible to create
different routes for different languages. This is one of the main
disadvantages of the SimpleCmsBundle.

Configuring the Content Class
.............................

SimpleCMSBundle will use
``Symfony\Cmf\Bundle\SimpleCmsBundle\Document\Page`` as the content class if
multilanguage is not enabled (this is the default). If no other class is chosen,
and multilanguage support is enabled, it will automatically switch to
``Symfony\Cmf\Bundle\SimpleCmsBundle\Document\MultilangPage``. You can
explicitly specify your content class and/or enable multilanguage support
using the configuration parameters:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_simple_cms:
            # defaults to Symfony\Cmf\Bundle\SimpleCmsBundle\Document\Page or MultilangPage (see above)
            document_class: ~
            multilang:
                # defaults to [] - declare your locales here to enable multilanguage
                locales: ~

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <container xmlns="http://cmf.symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">

            <!-- defaults to Symfony\Cmf\Bundle\SimpleCmsBundle\Document\Page or MultilangPage (see above) -->
            <config xmlns="http://cmf.symfony.com/schema/dic/simplecms"
                document-class="null"
            >
                <multilang>
                    <!-- defaults to empty list - declare your locales here to enable multilanguage -->
                    <locales></locales>
                </multilang>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_simple_cms', array(
            // defaults to Symfony\Cmf\Bundle\SimpleCmsBundle\Document\Page or MultilangPage (see above)
            'document_class' => null,
            'multilang'      => array(
                // defaults to empty array - declare your locales here to enable multilanguage
                'locales' => null,
            ),
        ));

SimpleCMSBundle in Detail
-------------------------

Now that you understand what the SimpleCMSBundle does, we'll detail how it
does it. Several other components are part of this bundle that change the
default behaviour of its dependencies.

Routing
~~~~~~~

The SimpleCMSBundle mostly relies on RoutingBundle and its set of
configurable capabilities to meet its requirements. It declares an
independent ``DynamicRouter`` service, with its own specific ``RouteProvider``,
``NestedMatcher``, Enhancers set and other useful services, all of them
instances of the classes bundled with RoutingBundle.

The only exception to this is ``RouteProvider``: the SimpleCMSBundle has its
own strategy to retrieve ``Route`` instances from persistent storage. This is
related to the way ``Route`` instances are stored by RoutingBundle.
By default, the ``path`` parameter will hold the prefixed full URI, including
the locale identifier. This would mean an independent ``Route`` instance
should exist for each translation of the same ``Content``. However, as we've
seen, ``MultilangPage```stores all translations in the same entry. So, to
avoid duplication, the locale prefix is stripped from the URI prior to
persistence, and SimpleCMSBundle includes ``MultilangRouteProvider``, which is
responsible for fetching ``Route`` instances taking that into account.

When rendering the actual URL from ``Route``, the locale prefix needs to be
replaced, otherwise the resulting addresses would not specify the locale they
refer to. To do so, ``MultilangPage`` uses the already mentioned
``getStaticPrefix()`` implementation.

Example: An incoming request for ``contact`` would be prefixed with the
``/cms/simple`` basepath, and the storage would be queried for
``/cms/simple/contact/``.  However, in a multilanguage setup, the locale is
prefixed to the URI, resulting in a query either for
``/cms/simple/en/contact/`` or ``/cms/simple/de/contact/``, which would
require two independent entries to exist for the same actual content. With the
above mentioned approach, the ``locale`` is stripped from the URI prior to
``basepath`` prepending, resulting in a query for ``/cms/simple/contact/`` in
both cases.

Routes and Redirects
....................

SimpleCMSBundle includes ``MultilangRoute`` and
``MultilangRedirectRoute``. These are extensions to the ``Route`` and ``RedirectRoute``
found in RoutingBundle, but with the necessary changes to handle the
prefix strategy discussed earlier.

Content Handling
~~~~~~~~~~~~~~~~

``Route`` instances are responsible for determining which ``Controller`` will
handle the current request. :ref:`start-routing-getting-controller-template`
shows how Symfony CMF SE can determine which ``Controller`` to use when
rendering a certain content document, and the SimpleCMSBundle uses these mechanisms to
do so.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_simple_cms:
            # defaults to cmf_content.controller:indexAction
            generic_controller: ~

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <container xmlns="http://cmf.symfony.com/schema/dic/services"
            xmlns:cmf-simple-cms="http://cmf.symfony.com/schema/dic/simplecms"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">

            <!-- defaults to cmf_content.controller:indexAction -->
            <cmf-simple-cms:config
                generic-controller="null"
            />
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_simple_cms', array(
            // defaults to cmf_content.controller:indexAction
            'generic_controller' => null,
        ));

Unless you specify otherwise, the ContentController from SimpleCMSBundle
is used for all Documents. The default configuration
associates all ``document_class`` instances with this ``Controller``, and
specifies no default template. However, you can configure several
``controllers_by_class`` and ``templates_by_class`` rules, which will
associate, respectively, ``Controller`` and templates to a specific Content
type. Symfony CMF SE includes an example of both in its default configuration.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_simple_cms:
            routing:
                templates_by_class:
                    Symfony\Cmf\Bundle\SimpleCmsBundle\Document\Page:  CmfSimpleCmsBundle:Page:index.html.twig
                controllers_by_class:
                    Symfony\Cmf\Bundle\RoutingBundle\Document\RedirectRoute:  cmf_routing.redirect_controller:redirectAction

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <container xmlns="http://cmf.symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">

            <config xmlns="http://cmf.symfony.com/schema/dic/simplecms"
                <routing>
                    <templates-by-class
                        alias="Symfony\Cmf\Bundle\SimpleCmsBundle\Document\Page">
                        CmfSimpleCmsBundle:Page:index.html.twig
                    </templates-by-class

                    <controllers-by-class
                        alias="Symfony\Cmf\Bundle\RoutingBundle\Document\RedirectRoute">
                        cmf_routing.redirect_controller:redirectAction
                    </templates-by-class
                </routing>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_simple_cms', array(
            'routing' => array(
                'templates_by_class' => array(
                    'Symfony\Cmf\Bundle\SimpleCmsBundle\Document\Page'             => CmfSimpleCmsBundle:Page:index.html.twig,
                    'Symfony\Cmf\Bundle\RoutingBundle\Document\RedirectRoute' => 'cmf_routing.redirect_controller:redirectAction',
                ),
            ),
        ));

These configuration parameters will be used to instantiate
:ref:`Route Enhancers <start-routing-getting-route-object>`. More information
about them can be found in the :doc:`../components/routing` component
documentation page.

The specific example above determines that content instances of class ``Page``
will be rendered using the above template, if none other is explicitly
provided by the associated ``Route`` (which, in this case, is ``Page``
itself). It also states that all content documents that instantiate ``RedirectRoute``
will be rendered using the specified ``Controller`` instead of the the default.
Again, the actual ``Route`` can provided a controller that will take priority
over this one. Both the template and the controller are part of
SimpleCMSBundle.

Menu Generation
~~~~~~~~~~~~~~~

As mentioned before, ``Page`` implements ``NodeInterface``, which means it
can be used to generate a ``MenuItem`` that will, in turn, be rendered into HTML
menus.

To do so, the default ``MenuBundle`` mechanisms are used, only a custom
``basepath`` is provided to the ``PHPCRMenuProvider`` instance. This is
defined in the SimpleCMSBundle configuration options, and used when handling
content storage to support functionality as described in :doc:`menu`
documentation. This parameter is optional, and can be configured as follows:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_simple_cms:
            # defaults to auto; true/false can be used to force providing/not providing a menu
            use_menu: ~

            # defaults to /cms/simple
            basepath: ~

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <container xmlns="http://cmf.symfony.com/schema/dic/services"
            xmlns:cmf-simple-cms="http://cmf.symfony.com/schema/dic/simplecms"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">

            <!-- use-menu: defaults to auto; true/false can be used to force providing/not providing a menu -->
            <!-- basepath: defaults to /cms/simple -->
            <cmf-simple-cms:config
                use-menu="null"
                basepath="null"
            />
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_simple_cms', array(
            // defaults to auto; true/false can be used to force providing/not providing a menu
            'use_menu' => null,

            // defaults to /cms/simple
            'basepath' => null,
        ));

Admin Support
-------------

The SimpleCMSBundle also includes the administration panel and respective
service needed for integration with `SonataDoctrinePHPCRAdminBundle`_, a
backend editing bundle. For more information about it, please refer to the
bundle's `documentation section`_.

The included administration panels will automatically be loaded if you install
the SonataDoctrinePHPCRAdminBundle (refer to
:doc:`../tutorials/creating-cms-using-cmf-and-sonata` for instructions on how
to do so). You can change this behaviour with the following configuration
option:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_simple_cms:
            # defaults to auto; true/false can be used to force using/not using SonataAdmin
            use_sonata_admin: ~

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <container xmlns="http://cmf.symfony.com/schema/dic/services"
            xmlns:cmf-simple-cms="http://cmf.symfony.com/schema/dic/simplecms"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">

            <!-- defaults to auto; true/false can be used to force using/not using SonataAdmin -->
            <cmf-simple-cms:config
                use-sonata-admin="null"
            />
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_simple_cms', array(
            // defaults to auto; true/false can be used to force using/not using SonataAdmin
            'use_sonata_admin' => null,
        ));

Fixtures
--------

The SimpleCMSBundle includes a support class for integration with
`DoctrineFixturesBundle`_, aimed at making loading initial data easier.
A working example is provided in Symfony CMF SE that illustrates how you can
easily generate ``MultilangPage`` and ``MultilangMenuNode`` instances from YAML
files.

Configuration
-------------

This bundle is configurable using a set of parameters, but all of them are
optional. You can go to the :doc:`../bundles/simple-cms` reference page for
the full configuration options list and aditional information.

Further Notes
-------------

For more information on the SimpleCMSBundle, please refer to:

* :doc:`../bundles/simple-cms` for configuration reference and advanced
  details about the bundle.
* :doc:`../getting-started/routing` for information about the routing
  component in which the SimpleCMSBundle is based on.
* :doc:`../getting-started/content` for information about the base content
  bundle that the SimpleCMSBundle depends on.
* :doc:`../getting-started/menu` for information about the menu system used
  by the SimpleCMSBundle.

.. _`SonataDoctrinePHPCRAdminBundle`: https://github.com/sonata-project/SonataDoctrinePhpcrAdminBundle
.. _`documentation section`: https://github.com/sonata-project/SonataDoctrinePhpcrAdminBundle/tree/master/Resources/doc
.. _`DoctrineFixturesBundle`: http://symfony.com/doc/master/bundles/DoctrineFixturesBundle/index.html
