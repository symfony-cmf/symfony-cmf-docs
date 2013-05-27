Creating a CMS using CMF and Sonata
===================================

The goal of this tutorial is to create a simple content management system
using the CMF as well as `SonataAdminBundle`_ and
:doc:`../bundles/doctrine_phpcr_admin`.


.. index:: Sonata, SonataAdminBundle, SonataDoctrinePHPCRAdminBundle, SonatajQueryBundle, FOSJsRoutingBundle, TreeBundle, TreeBrowserBundle

Preconditions
-------------

* :doc:`installing-cmf-core`
* `Symfony SecurityBundle`_ (required by the SonataAdminBundle default templates)

Installation
------------

Download the Bundles
~~~~~~~~~~~~~~~~~~~~

Add the following to your ``composer.json`` file:

.. code-block:: javascript

    "require": {
        ...
        "sonata-project/doctrine-phpcr-admin-bundle": "1.0.*",
    }

And then run:

.. code-block:: bash

    $ php composer.phar update

Initialize Bundles
~~~~~~~~~~~~~~~~~~

Next, initialize the bundles in ``app/AppKernel.php`` by adding them to the
``registerBundle`` method::

    public function registerBundles()
    {
        $bundles = array(
            // ...

            // support for the admin
            new Symfony\Cmf\Bundle\TreeBrowserBundle\SymfonyCmfTreeBrowserBundle(),
            new Sonata\jQueryBundle\SonatajQueryBundle(),
            new Sonata\BlockBundle\SonataBlockBundle(),
            new Sonata\AdminBundle\SonataAdminBundle(),
            new Sonata\DoctrinePHPCRAdminBundle\SonataDoctrinePHPCRAdminBundle(),
            new FOS\JsRoutingBundle\FOSJsRoutingBundle(),
        );

        // ...
    }

Configuration
-------------

Add the sonata bundles to your application configuration:

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
                Doctrine\ODM\PHPCR\Document\Generic:
                    valid_children:
                        - all
                Symfony\Cmf\Bundle\SimpleCmsBundle\Document\Page: ~
                Symfony\Cmf\Bundle\RoutingBundle\Document\Route:
                    valid_children:
                        - Symfony\Cmf\Bundle\RoutingBundle\Document\Route
                        - Symfony\Cmf\Bundle\RoutingBundle\Document\RedirectRoute
                Symfony\Cmf\Bundle\RoutingBundle\Document\RedirectRoute:
                    valid_children: []
                Symfony\Cmf\Bundle\MenuBundle\Document\MenuNode:
                    valid_children:
                        - Symfony\Cmf\Bundle\MenuBundle\Document\MenuNode
                        - Symfony\Cmf\Bundle\MenuBundle\Document\MultilangMenuNode
                Symfony\Cmf\Bundle\MenuBundle\Document\MultilangMenuNode:
                    valid_children:
                        - Symfony\Cmf\Bundle\MenuBundle\Document\MenuNode
                        - Symfony\Cmf\Bundle\MenuBundle\Document\MultilangMenuNode

Add route in to your routing configuration:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        admin:
            resource: '@SonataAdminBundle/Resources/config/routing/sonata_admin.xml'
            prefix: /admin

        sonata_admin:
            resource: .
            type: sonata_admin
            prefix: /admin

        doctrine_phpcr_admin_bundle_odm_browser:
            resource: "@SonataDoctrinePHPCRAdminBundle/Resources/config/routing/phpcrodmbrowser.xml"

        fos_js_routing:
            resource: "@FOSJsRoutingBundle/Resources/config/routing/routing.xml"

        symfony_cmf_tree:
            resource: .
            type: 'symfony_cmf_tree'


The FOSJsRoutingBundle is used to export sonata routes to javascript, to be
used with the tree. All relevant routes have the ``expose`` option set. If you
do custom routes that need to be used with the tree, you need to do that or
configure the js routing bundle manually.

Sonata Assets
-------------

.. code-block:: bash

    $ php app/console assets:install --symlink


Defining own Admin Classes
--------------------------

The CMF bundles come with predefined admin classes which will be activated
automatically if Sonata PHPCR-ODM Admin is loaded. If you need to write
different admins and do not want to load the defaults, you can deactivate the
loading - see the documentation of the respective bundles.

To load your own Admin service, you need to declare it as a service, tag with
``sonata.admin`` with ``manager_type="doctrine_phpcr"``. For the admin to work
properly, you need to add a call for method ``setRouteBuilder`` to set it to
the service ``sonata.admin.route.path_info_slashes``, or your Admin will not
work.

The constructor expects three arguments, code, document class and controller
name. You can pass an empty argument for the code, the document class must be
the fully qualified class name of the document this admin is for and the third
argument can be used to set a custom controller that does additional
operations over the default sonata CRUD controller.

.. configuration-block::

    .. code-block:: xml

        <service id="my_bundle.admin" class="%my_bundle.admin_class%">
            <tag name="sonata.admin" manager_type="doctrine_phpcr" group="dashboard.group_content" label_catalogue="MyBundle" label="dashboard.label_my_admin" label_translator_strategy="sonata.admin.label.strategy.underscore" />
            <argument/>
            <argument>%my_bundle.document_class%</argument>
            <argument>SonataAdminBundle:CRUD</argument>

            <call method="setRouteBuilder">
                <argument type="service" id="sonata.admin.route.path_info_slashes" />
            </call>
        </service>


Finally
-------

Now Sonata is configured to work with the PHPCR you can access the dashboard
using via ``/admin/dashboard`` in your site.


Tree Problems
-------------

If you have not yet added anything to the content repository, the tree view
will not load as it cannot find a root node. To fix this, load some data as
fixtures by following ":doc:`using-blockbundle-and-contentbundle`"

Further Reading
---------------

* :doc:`../bundles/doctrine_phpcr_admin`
* :doc:`handling-multilang-documents`

.. _`SonataAdminBundle`: https://github.com/sonata-project/SonataAdminBundle
.. _`Symfony SecurityBundle`: http://symfony.com/doc/master/book/security.html
