﻿MenuBundle
==========

The `MenuBundle <https://github.com/symfony-cmf/MenuBundle#readme>`_
provides menus from a doctrine object manager with the help of KnpMenuBundle.

.. index:: MenuBundle

Dependencies
------------

This bundle is extending the `KnpMenuBundle <https://github.com/knplabs/KnpMenuBundle>`_.

Unless you change defaults and provide your own implementations, this bundle also depends on

* ``SymfonyRoutingExtraBundle`` for the router service ``symfony_cmf_routing_extra.dynamic_router``.
  Note that you need to explicitly enable the dynamic router as per default it is not loaded.
  See the :doc:`documentation of the routing extra bundle<routing-extra>` for how to do this.
* :doc:`PHPCR-ODM<phpcr-odm>` to load route documents from the content repository

Configuration
-------------

If you want to use default configurations, you do not need to change anything.
The values are:

.. configuration-block::

    .. code-block:: yaml

        symfony_cmf_menu:
            menu_basepath:        /cms/menu
            document_manager_name: default
            admin_class:          ~
            document_class:       ~
            content_url_generator:  router
            content_key:          ~ # (resolves to DynamicRouter::CONTENT_KEY)
            route_name:           ~ # cmf routes are created by content instead of name
            content_basepath:     ~ # defaults to symfony_cmf_core.content_basepath
            use_sonata_admin:     auto # use true/false to force using / not using sonata admin
            multilang:                # the whole multilang section is optionnal
                use_sonata_admin:     auto # use true/false to force using / not using sonata admin
                admin_class:          ~
                document_class:       ~
                locales:              [] # if you use multilang, you have to define at least one locale

If you want to render the menu from twig, make sure you have not disabled twig
in the ``knp_menu`` configuration section.

If ``sonata-project/doctrine-phpcr-admin-bundle`` is added to the composer.json
require section, the menu documents are exposed in the SonataDoctrinePhpcrAdminBundle.
For instructions on how to configure this Bundle see :doc:`doctrine_phpcr_admin`.

By default, ``use_sonata_admin`` is automatically set based on whether
SonataDoctrinePhpcrAdminBundle is available but you can explicitly disable it
to not have it even if sonata is enabled, or explicitly enable to get an error
if Sonata becomes unavailable.


Menu entries
------------

A ``MenuItem`` document defines menu entries. You can build menu items based on
symfony routes, absolute or relative urls or referenceable PHPCR-ODM content
documents.

The menu tree is built from documents under [menu_basepath]/[menuname]. You can
use different document classes for menu items as long as they implement
``Knp\Menu\NodeInterface`` to integrate with KnpMenuBundle. The default ``MenuNode``
document discards children that do not implement this interface.

The currently highlighted entry is determined by checking if the content
associated with a menu document is the same as the content the DynamicRouter
has put into the request.

Examples::

    <?php

    // get document manager
    $dm = ...;
    $rootNode = $dm->find(null, '...'); // retrieve parent menu item

    // using referenceable content document
    $blogContent = $dm->find(null, '/my/cms/content/blog');

    $blogNode = new MenuNode();
    $blogNode->setName('blog');
    $blogNode->setParent($parent);
    $blogNode->setContent($blogDocument);
    $blogNode->setLabel('Blog');

    $dm->persist($blogNode);

    // using a route document
    $timelineRoute = $dm->find(null, '/my/cms/routes/timeline');

    $timelineNode = new MenuNode();
    $timelineNode->setContent($timelineRoute);
    // ...

    $dm->persist($timelineNode);

    // using a symfony route
    $sfRouteNode = new MenuNode();
    $sfRouteNode->setRoute('my_hard_coded_symfony_route');
    // ...

    $dm->persist($sfRouteNode);

    // using URL
    $urlNode = new MenuNode();
    $urlNode->setUri('http://www.example.com');
    // ...

    $dm->persist($urlNode);

    $dm->flush();

By default content documents are created using a **weak** reference (this means
you will be able to delete the referenced content). You can specify a strong
reference by using ``setWeak(false)``::

    <?php

    $node = new MenuNode;
    // ...
    $node->setWeak(false);

.. note::

    When content is referenced weakly and subsequently deleted the 
    rendered menu will not provide a link to the content.

Usage
-----

Adjust your twig template to load the menu.

.. code-block:: jinja

    {{ knp_menu_render('simple') }}

The menu name is the name of the node under ``menu_basepath``. For example if your
repository stores the menu nodes under ``/cms/menu`` , rendering "main" would mean
to render the menu that is at ``/cms/menu/main``


How to use non-default other components
---------------------------------------

If you use the cmf menu with PHPCR-ODM, you just need to store Route documents
under ``menu_basepath``. If you use a different object manager, you need to
make sure that the menu root document is found with

.. code-block:: php

    $dm->find($menu_document_class, $menu_basepath . $menu_name)

The route document must implement ``Knp\Menu\NodeInterface`` - see
``MenuNode`` document for an example. You probably need to specify
menu_document_class too, as only PHPCR-ODM can determine the document from the
database content.

If you use the cmf menu with the DynamicRouter, you need no route name as the
menu document just needs to provide a field content_key in the options.
If you want to use a different service to generate URLs, you need to make sure
your menu entries provide information in your selected content_key that the url
generator can use to generate the url. Depending on your generator, you might
need to specify a route_name too.
Note that if you just want to generate normal symfony routes with a menu that
is in the database, you can pass the core router service as content_url_generator,
make sure the content_key never matches and make your menu documents provide
the route name and eventual routeParameters.
