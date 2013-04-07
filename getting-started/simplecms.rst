.. index::
    single: SimpleCMS, SymfonyCmfSimpleCMSBundle

SimpleCMS
=========

Concept
-------

In the previous documentation pages all the basic components of Symfony CMF
have been analysed: the :doc:`routing` that allows you to associate URLs
with your :doc:`content`, which users can browse using a :doc:`menu`.

These three components complement each other but are independent: they work
without each other, allowing you to choose which ones you want to use, extend
or ignore. In some cases, however, you might just want a simple implementation
that gathers all those functionalities in a ready-to-go package. For that
purpose, the ``SimpleCMSBundle`` was created.


SimpleCMSBundle
---------------

``SimpleCMSBundle`` is implemented on top of most of the other Symfony CMF
Bundles, combining them into a functional CMS. It's a simple solution, but
you will find it very useful when you start implementing your own CMS using
Symfony CMF. Whether you decide to extend or replace it, it's up to you,
but in both cases, it's a good place to start developing your first CMS.


Page
~~~~

``SimpleCMSBundle`` basic content type is ``Page``. Its class declaration
points out many of the features available:

- It extends ``Route``, meaning it's not only a ``Content`` instance, but
    also a ``Route``. In this case, as declared in ``getRouteContent()``, the
    ``Route`` as an associated content, itself.
- It implements ``RouteAwareInterface``, which means it has associated ``Route``
    instances. As expected, and as seen in ``getRoutes()``, it has only one ``Route``
    associated: itself.
- It implements ``NodeInterface``, which means it can be used by ``MenuBundle``
    to generate a menu structure.


The class itself is similar to the ``StaticContent`` already described in
the documentation page regarding :doc:`content`, although some key atributes,
like ``parent`` or ``path`` come from the ``Route`` class it extends.


Three-in-one
~~~~~~~~~~~~

Like explained before, this bundle gathers functionality from three distinct
bundles: :doc:`content`, :doc:`routing` and :doc:`menu`. The routing component
receives a request, that it matches to a ``Route`` instance loaded from database.
That ``Route`` points to a ``Content`` instance: itself. A controller is
also determined by the routing component, that renders the ``Content`` using
a template which, in turn, presents the user with a HTML visualization of
the stored information tree structure, rendered using ``MenuItem`` obtained
from equivalent ``NodeInterface`` instances.

``SimpleCMSBundle`` simplifies this process: ``Content``, ``Route`` and ``NodeInterface``
are gathered in one class: ``Page``. This three-in-one approach is the key
concept behind this bundle.


MultilangPage
~~~~~~~~~~~~~

Like you would expect, a multilanguage version of ``Page`` is also included.
``MultilangPage`` defines a ``locale`` variable and which fields will be
translated (``title``, ``label`` and ``body``). It also includes ``getStaticPrefix()``
to handle the path prefix of the ``Page``. This is part of the route handling
mechanism, and will be analysed bellow.

 The ``MultilangPage`` uses the ``attribute`` strategy for translation: the
 several translations coexist in the same database entry, and the several
 translated versions of each field are stored as different attributes in
 that same entry.
 
 As the routing is not separated from the content, it is not possible to
 create different routes for different languages. This is one of the biggest
 disadvantages of the ``SimpleCmsBundle``.
 
Configuring the Content Class
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

By default, ``SimpleCMSBundle`` will use ``Symfony\Cmf\Bundle\SimpleCmsBundle\Document\Page``
as the content class if multilanguage is not enabled (default). If no other
class is chosen, and multilanguage support is enabled, it will automatically
switch to ``Symfony\Cmf\Bundle\SimpleCmsBundle\Document\MultilangPage``.
You can explicitly specify your content class and/or enable multilanguage
support using the configuration parameters:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_simple_cms:
            document_class: ~  # Symfony\Cmf\Bundle\SimpleCmsBundle\Document\Page
            multilang:
                locales: ~  # defaults to [], declare your locales here to enable multilanguage

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <symfony-cmf-simple-cms:config
            document-class="null"
        >
            <symfony-cmf-simple-cms:multilang>
                <symfony-cmf-simple-cms:locales></symfony-cmf-simple-cms:locales>
            </symfony-cmf-simple-cms:multilang>
        </symfony-cmf-simple-cms:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('symfony_cmf_simple_cms', array(
            'document_class' => null,
            'multilang'      => array(
                'locales' => null,
            ),
        ));


SimpleCMSBundle in Detail
-------------------------

Now that you understand what ``SimpleCMSBundle`` does, we'll detail how it
does it. Several other components are part of this bundle, that change the
default behaviour of its dependencies.


The Routing
~~~~~~~~~~~

``SimpleCMSBundle`` doesn't add much functionality to the routing part of
Symfony CMF. Instead, it greatly relies on ``RoutingExtraBundle`` and its
set of configurable functionalities to meet its requirements. It declares
an independent ``DynamicRouter``, with it's own specific ``RouteProvider``,
``NestedMatcher``, Enhancers set and other useful services, all of them instances
of the classes bundled with ``RoutingBundle`` and ``RoutingExtraBudle``.
This service declaration duplication allows you to reuse the original ``RoutingExtraBundle``
configuration options to declare another Router, if you wish to do so.

The only exception to this is ``RouteProvider``: the ``SimpleCMSBundle``
has its own strategy to retrieve ``Route`` instances from database. This
is related with the way ``Route`` instances are stored in database by ``RoutingExtraBundle``.
By default, the ``path`` parameter will hold the prefixed full URI, including
the locale identifier. This would mean an independent ``Route`` instance
should exist for each translation of the same ``Content``. However, as we've
seen, ``MultilangPage```stores all translations in the same entry. So, to
avoid duplication, the locale prefix is stripped from the URI prior to persistance,
and ``SimpleCMSBundle`` includes ``MultilangRouteProvider``, which is responsible
for fetching ``Route`` instances taking that into account.

When rendering the actual URL from ``Route``, the locale prefix needs to be
put back, otherwise the resulting addresses wouldn't specify the locale they
refer to. To do so, ``MultilangPage`` uses the already mentioned ``getStaticPrefix()``
implementation.

Exemplifying: An incoming request for ``contact`` would be prefixed with
``/cms/simple`` basepath, and the storage would be queried for ``/cms/simple/contact/``.
However, in a multilanguage setup, the locale is prefixed to the URI, resulting
in a query either for ``/cms/simple/en/contact/`` or ``/cms/simple/de/contact/``,
which would require two independent entries to exist for the same actual
content. With the above mentioned approach, the ``locale`` is stripped from
the URI prior to ``basepath`` prepending, resulting in a query for ``/cms/simple/contact/``
in both cases.


Routes and Redirections
^^^^^^^^^^^^^^^^^^^^^^^

``SimpleCMSBundle`` includes ``MultilangRoute`` and ``MultilangRedirectRoute``,
extensions to the ``Route`` and ``RedirectRoute`` found in ``RoutingExtraBudle``,
but with the necessary changes to handle the prefix strategy discussed earlier.


Content Handling
~~~~~~~~~~~~~~~~

``Route`` instances are responsible for determining which ``Controller``
will handle the current request. :ref:`routing-getting-controller-template`
shows how Symfony CMF SE can determine which ``Controller`` to use when rendering
a certain content, and ``SimpleCMSBundle`` uses these mechanisms to do so.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_simple_cms:
            generic_controller: ~  # symfony_cmf_content.controller:indexAction

    .. code-block:: xml
    
        <!-- app/config/config.xml -->
        <symfony-cmf-simple-cms:config
            generic-controller="null"
        />

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('symfony_cmf_simple_cms', array(
            'generic_controller' => null,
        ));

By default, it uses the above mentioned service, which instanciates ``ContentController``
from ``ContentBundle``. The default configuration associates all ``document_class``
instances with this ``Controller``, and specifies no default template. However,
you can configure several ``controllers_by_class`` and ``templates_by_class``
rules, which will associate, respectively, ``Controller`` and templates to
a specific Content type. Symfony CMF SE includes an example of both in its
default configuration.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_simple_cms:
            routing:
                templates_by_class:
                    Symfony\Cmf\Bundle\SimpleCmsBundle\Document\Page:  SymfonyCmfSimpleCmsBundle:Page:index.html.twig
                controllers_by_class:
                    Symfony\Cmf\Bundle\RoutingExtraBundle\Document\RedirectRoute:  symfony_cmf_routing_extra.redirect_controller:redirectAction

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <symfony-cmf-simple-cms:config>
            <symfony-cmf-simple-cms:routing>
                <symfony-cmf-simple-cms:templates-by-class
                    alias="Symfony\Cmf\Bundle\SimpleCmsBundle\Document\Page">
                    SymfonyCmfSimpleCmsBundle:Page:index.html.twig
                </symfony-cmf-simple-cms:templates-by-class

                <symfony-cmf-simple-cms:controllers-by-class
                    alias="Symfony\Cmf\Bundle\RoutingExtraBundle\Document\RedirectRoute">
                    symfony_cmf_routing_extra.redirect_controller:redirectAction
                </symfony-cmf-simple-cms:templates-by-class
            </symfony-cmf-simple-cms:routing>
        </symfony-cmf-simple-cms:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('symfony_cmf_simple_cms', array(
            'routing' => array(
                'templates_by_class' => array(
                    'Symfony\Cmf\Bundle\SimpleCmsBundle\Document\Page'             => SymfonyCmfSimpleCmsBundle:Page:index.html.twig,
                    'Symfony\Cmf\Bundle\RoutingExtraBundle\Document\RedirectRoute' => 'symfony_cmf_routing_extra.redirect_controller:redirectAction',
                ),
            ),
        ));

These configuration parameters will be used to instantiate :ref:`Route Enhancers <routing-getting-route-object>`.
More information about them can be found in the :doc:`../components/routing`
component documentation page.

These specific example determines that content instances of class ``Page``
will be rendered using the above mentioned template, if no other is explicitly
provided by the associated ``Route`` (which, in this case, is ``Page`` itself).
It also states that all contents that instantiate ``RedirectRoute`` will
be rendered using the mentioned ``Controller`` instead of the default. Again, 
the actual ``Route`` can provided a controller, in will take priority over
this one. Both the template and the controller are part of ``SimpleCMSBundle``.


Menu Generation
~~~~~~~~~~~~~~~

Like mentioned before, ``Page`` implements ``NodeInterface``, which means
it can be used to generate ``MenuItem`` that will, in turn, be rendered into
HTML menus presented to the user.

To do so, the default ``MenuBundle`` mechanisms are used, only a custom ``basepath``
is provided to the ``PHPCRMenuProvider`` instance. This is defined in ``SimpleCMSBundle``
configuration options, and used when handling content storage, to support
functionality as described in :doc:`menu` documentation. This parameter is
optional, can be configured like so:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_simple_cms:
            use_menu: ~  # defaults to auto , true/false can be used to force providing / not providing a menu
            basepath: ~  # /cms/simple

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <symfony-cmf-simple-cms:config
            use-menu="null"
            basepath="null"
        />

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('symfony_cmf_simple_cms', array(
            'use_menu' => null,
            'basepath' => null,
        ));


Admin Support
-------------

``SimpleCMSBundle`` also includes the administration panel and respective
service needed for integration with `SonataDoctrinePHPCRAdminBundle <https://github.com/sonata-project/SonataDoctrinePhpcrAdminBundle>`_,
a backoffice generation tool that can be installed with Symfony CMF. For
more information about it, please refer to the bundle's `documentation section <https://github.com/sonata-project/SonataDoctrinePhpcrAdminBundle/tree/master/Resources/doc>`_.

The included administration panels will automatically be loaded if you install
``SonataDoctrinePHPCRAdminBundle`` (refer to :doc:`../tutorials/creating-cms-using-cmf-and-sonata`
for instructions on how to do so). You can change this behaviour with the
following configuration option:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_simple_cms:
            use_sonata_admin: ~  # defaults to auto , true/false can be used to using / not using SonataAdmin

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <symfony-cmf-simple-cms:config
            use-sonata-admin="null"
        />

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('symfony_cmf_simple_cms', array(
            'use_sonata_admin' => null,
        ));


Fixtures
--------

``SimpleCMSBundle`` includes a support class for integration with `DoctrineFixturesBundle <http://symfony.com/doc/master/bundles/DoctrineFixturesBundle/index.html>`_,
aimed at making loading initial data easier. A working example is provided
in Symfony CMF SE, that illustrates how you can easily generate ``MultilangPage``
and ``MultilangMenuNode`` instances from yml files.


Configuration
-------------

This bundle is configurable using a set of parameters, but all of them are
optional. You can go to the :doc:`../bundles/simple-cms` reference page for the
full configuration options list and aditional information.

Further Notes
-------------

For more information on the SimpleCMSBundle, please refer to:

- :doc:`../bundles/simple-cms` for configuration reference and advanced details
  about the bundle.
- :doc:`../getting-started/routing` for information about the routing component
  in which ``SimpleCMSBundle`` is based on.
- :doc:`../getting-started/content` for information about the base content
  bundle that ``SimpleCMSBundle`` depends on.
- :doc:`../getting-started/menu` for information about the menu system used
  by ``SimpleCMSBundle``.
