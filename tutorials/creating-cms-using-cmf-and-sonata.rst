Creating a CMS using CMF and Sonata
===================================
The goal of this tutorial is to create a simple content management system using the CMF as well as
`SonataAdminBundle <https://github.com/sonata-project/SonataAdminBundle>`_.

.. index:: Sonata, SonataAdminBundle, SonataDoctrinePHPCRAdminBundle, SonatajQueryBundle, FOSJsRoutingBundle, TreeBundle, TreeBrowserBundle

Documentation TODO
------------------

- expand intro and add steps to take
- describe steps to make a CMS

Preconditions
-------------

- :doc:`/tutorials/installing-configuring-cmf`

Installation
------------

Download the bundles
~~~~~~~~~~~~~~~~~~~~
Add the following to your ``composer.json`` file::

.. code-block:: javascript

    "require": {
        ...
        "sonata-project/doctrine-phpcr-admin-bundle": "1.0.*",
    }

And then run::

    php composer.phar update

Initialize bundles
~~~~~~~~~~~~~~~~~~
Next, initialize the bundles in ``app/AppKernel.php`` by adding them to the ``registerBundle`` method::

.. code-block:: php

    public function registerBundles()
    {
        $bundles = array(
            // ...

            // support for the admin
            new Symfony\Cmf\Bundle\TreeBundle\SymfonyCmfTreeBundle(),
            new Symfony\Cmf\Bundle\TreeBrowserBundle\SymfonyCmfTreeBrowserBundle(),
            new Sonata\jQueryBundle\SonatajQueryBundle(),
            new Sonata\AdminBundle\SonataAdminBundle(),
            new Sonata\DoctrinePHPCRAdminBundle\SonataDoctrinePHPCRAdminBundle(),
            new FOS\JsRoutingBundle\FOSJsRoutingBundle(),
        );
        // ...
    }

Configuration
-------------

Add the sonata bundles to your application configuration ::

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        sonata_block:
            default_contexts: [cms]
            blocks:
                sonata.admin.block.admin_list:
                    contexts:   [admin]
                sonata_admin_doctrine_phpcr.tree_block:
                    settings:
                        id: '/cms'
                    contexts:   [admin]

        sonata_admin:
            templates:
                # default global templates
                ajax:    SonataAdminBundle::ajax_layout.html.twig
            dashboard:
                blocks:
                    # display a dashboard block
                    - { position: right, type: sonata.admin.block.admin_list }
                    - { position: left, type: sonata_admin_doctrine_phpcr.tree_block }

        sonata_doctrine_phpcr_admin:
            document_tree:
                Doctrine\PHPCR\Odm\Document\Generic:
                    valid_children:
                        - all
                Symfony\Cmf\Bundle\SimpleCmsBundle\Document\Page: ~
                Symfony\Cmf\Bundle\RoutingExtraBundle\Document\Route:
                    valid_children:
                        - Symfony\Cmf\Bundle\RoutingExtraBundle\Document\Route
                        - Symfony\Cmf\Bundle\RoutingExtraBundle\Document\RedirectRoute
                Symfony\Cmf\Bundle\RoutingExtraBundle\Document\RedirectRoute:
                    valid_children: []
                Symfony\Cmf\Bundle\MultilangContentBundle\Document\MultilangLanguageSelectRoute:
                    valid_children:
                        - Symfony\Cmf\Bundle\RoutingExtraBundle\Document\Route
                        - Symfony\Cmf\Bundle\RoutingExtraBundle\Document\RedirectRoute
                Symfony\Cmf\Bundle\MenuItem\Document\MenuItem:
                    valid_children:
                        - Symfony\Cmf\Bundle\MenuItem\Document\MenuItem
                        - Symfony\Cmf\Bundle\MultilangContentBundle\Document\MultilangMenuItem
                Symfony\Cmf\Bundle\MultilangContentBundle\Document\MultilangMenuItem:
                    valid_children:
                        - Symfony\Cmf\Bundle\MenuItem\Document\MenuItem
                        - Symfony\Cmf\Bundle\MultilangContentBundle\Document\MultilangMenuItem

        fos_js_routing:
            routes_to_expose:
                - admin_sandbox_main_editablestaticcontent_create
                - admin_sandbox_main_editablestaticcontent_delete
                - admin_sandbox_main_editablestaticcontent_edit
                - admin_bundle_menu_menuitem_create
                - admin_bundle_menu_menuitem_delete
                - admin_bundle_menu_menuitem_edit
                - admin_bundle_multilangcontent_multilangmenuitem_create
                - admin_bundle_multilangcontent_multilangmenuitem_delete
                - admin_bundle_multilangcontent_multilangmenuitem_edit
                - admin_bundle_multilangcontent_multilangstaticcontent_create
                - admin_bundle_multilangcontent_multilangstaticcontent_delete
                - admin_bundle_multilangcontent_multilangstaticcontent_edit
                - admin_bundle_multilangcontent_multilanglanguageselectroute_create
                - admin_bundle_multilangcontent_multilanglanguageselectroute_delete
                - admin_bundle_multilangcontent_multilanglanguageselectroute_edit
                - admin_bundle_routingextra_route_create
                - admin_bundle_routingextra_route_delete
                - admin_bundle_routingextra_route_edit
                - admin_bundle_simplecms_page_create
                - admin_bundle_simplecms_page_delete
                - admin_bundle_simplecms_page_edit
                - symfony_cmf_tree_browser.phpcr_children
                - symfony_cmf_tree_browser.phpcr_move
                - sonata.admin.doctrine_phpcr.phpcrodm_children
                - sonata.admin.doctrine_phpcr.phpcrodm_move

Add route in to your routing configuration ::

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        admin:
            resource: '@SonataAdminBundle/Resources/config/routing/sonata_admin.xml'
            prefix: /admin
