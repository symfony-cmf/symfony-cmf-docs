MenuBundle
====================

The `MenuBundle <https://github.com/symfony-cmf/MenuBundle#readme>`_
provides menus from a doctrine object manager with the help of KnpMenuBundle.

.. index:: MenuBundle


Menu entries
------------

Document\MenuItem defines menu entries. You can build menu items based on
symfony routes, absolute or relative urls or referenceable phpcr-odm content
documents.

The menu tree is built from documents under [menu_basepath]/[menuname]. You can
use different document classes for menu items, as long as they implement
``Knp\Menu\NodeInterface`` to integrate with KnpMenuBundle. The default MenuItem
Document discards children that do not implement this interface.

The currently highlighted entry is determined by checking if the content
associated with a menu document is the same as the content the DynamicRouter
has put into the request.

Setup
-----

See `/tutorials/installing-configuring-cmf`.

Configuration
-------------

If you want to use default configurations, you do not need to change anything.
The values are:

.. configuration-block::

    .. code-block:: yaml

        symfony_cmf_menu:
            menu_basepath:        /cms/menu
            document_manager_name:  default
            admin_class:          ~
            document_class:       ~
            content_url_generator:  router
            content_key:          ~ # (resolves to DynamicRouter::CONTENT_KEY)
            route_name:           ~ # cmf routes are created by content instead of name
            content_basepath:     ~ # defaults to symfony_cmf_core.content_basepath
            use_sonata_admin:     auto # use true/false to force using / not using sonata admin
            multilang:
                use_sonata_admin:     auto # use true/false to force using / not using sonata admin
                admin_class:          ~
                document_class:       ~
                locales:              []

If you want to render the menu from twig, make sure you have not disabled twig
in the ``knp_menu`` configuration section.

If ``sonata-project/doctrine-phpcr-admin-bundle`` is added to the composer.json
require section, the menu documents are exposed in the SonataDoctrinePhpcrAdminBundle.
Don't forget to instantiate ``SonataDoctrinePhpcrAdminBundle`` in your kernel in
this case.

By default, ``use_sonata_admin`` is automatically set based on whether
SonataDoctrinePhpcrAdminBundle is available but you can explicitly disable it
to not have it even if sonata is enabled, or explicitly enable to get an error
if Sonata becomes unavailable.


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

If you use the cmf menu with phpcr-odm, you just need to store Route documents
untdr ``menu_basepath``. If you use a different object manager, you need to
make sure that the menu root document is found with

.. code-block:: php

    $dm->find($menu_document_class, $menu_basepath . $menu_name)

The route document must implement ``Knp\Menu\NodeInterface`` - see
Document/MenuItem.php for an example. You probably need to specify
menu_document_class too, as only phpcr-odm can determine the document from the
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


Dependencies
------------

This bundle is extending the `KnpMenuBundle <https://github.com/knplabs/KnpMenuBundle>`_.

Unless you change defaults and provide your own implementations, this bundle also depends on

* ``SymfonyRoutingExtraBundle`` for the router service ``symfony_cmf_routing_extra.dynamic_router``.
  Note that you need to explicitly enable the dynamic router as per default it is not loaded.
  See the :doc:`documentation of the routing extra bundle</bundles/routing-extra>` for how to do this.
* :doc:`/bundles/phpcr-odm` to load route documents from the content repository
