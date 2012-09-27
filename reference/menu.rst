SymfonyCmfMenuBundle
====================

The `SymfonyCmfMenuBundle <https://github.com/symfony-cmf/MenuBundle#readme>`_
provides menus from a doctrine object manager with the help of KnpMenuBundle.

.. index:: MenuBundle

Menu entries
------------

Document\MenuItem defines menu entries. You can build menu items based on
symfony routes, absolute or relative urls or referenceable phpcr-odm content
documents.

The menu tree is built from documents under [menu_basepath]/[menuname]. To
prevent accidentally exposing nodes, only nodes ending on -item are considered
menu items.
You can use different document classes for menu items, as long as they implement
Knp\Menu\NodeInterface to integrate with KnpMenuBundle.

The currently highlighted entry is determined by checking if the content
associated with a menu document is the same as the content DoctrineRouter
has put into the request.

Until we have a decent tutorial, you can look into the `cmf-sandbox <https://github.com/symfony-cmf/cmf-sandbox>`_
and specifically the `menu fixtures <https://github.com/symfony-cmf/cmf-sandbox/blob/master/src/Sandbox/MainBundle/Resources/data/fixtures/030_LoadMenuData.php>`_.

The `CMF website <http://cmf.symfony.com>`_ is another application using the CMF and the MenuBundle. 

Configuration
-------------
::

    knp_menu:
        twig: true

    symfony_cmf_menu:
        menu_basepath: /phpcr/path/to/menutree
        document_manager: doctrine_phpcr.odm.default_document_manager
        menu_document_class: null
        content_url_generator: symfony_cmf_routing_extra.dynamic_router
        content_key: null (resolves to DoctrineRouter::CONTENT_KEY)
        route_name: null
        use_sonata_admin: auto|true|false 
        content_basepath: /phpcr/path/to/content (used for the menu admin)

If ``sonata-project/doctrine-phpcr-admin-bundle`` is added to the composer require,
the MenuBundle can be used inside the SonataDoctrinePhpcrAdminBundle. But then, 
the SonataDoctrinePhpcrAdminBundle has to be instantiated in your application's kernel.

By default, ``use_sonata_admin`` is automatically set based on whether
SonataDoctrinePhpcrAdminBundle is available.

Usage
-----

Adjust your twig template to load the menu.

    {{ knp_menu_render('simple') }}


The menu name is the name of the node under ``menu_basepath``. For example if your
repository stores the menu nodes under ``/cms/menu`` , rendering "main" would mean
to render the menu that is at ``/cms/menu/main``


How to use non-default other components
---------------------------------------

If you use the cmf menu with phpcr-odm, you just need to store Route documents
unter ``menu_basepath``. If you use a different object manager, you need to
make sure that the route root document is found with

    $dm->find(route_document_class, menu_basepath . menu_name)

The route document must implement ``Knp\Menu\NodeInterface`` - see
Document/MenuItem.php for an example. You probably need to specify
menu_document_class too, as only phpcr-odm can determine the document from the
database content.

If you use the cmf menu with the DoctrineRouter, you need no route name as the
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

* KnpMenuBundle

Unless you change defaults and provide your own implementations, also depends on

* SymfonyRoutingExtraBundle for the doctrine router service symfony_cmf_chain_routing.doctrine_router
    Note that you need to explicitly enable the doctrine router as per default it is not loaded.
    See the documentation of the routing extra bundle for how to do this.
* Doctrine PHPCR-ODM to load route documents from the content repository
