The MenuBundle
==============

The `MenuBundle`_ provides menus from a doctrine object manager with the help
of `KnpMenuBundle`_.

.. index:: MenuBundle

Dependencies
------------

This bundle is extending the `KnpMenuBundle`_.

Unless you change defaults and provide your own implementations, this bundle
also depends on

* ``SymfonyRoutingBundle`` for the router service
  ``cmf_routing.dynamic_router``.  Note that you need to explicitly
  enable the dynamic router as per default it is not loaded.  See the
  :doc:`documentation of the cmf routing bundle <routing>` for how to do this.
* :doc:`PHPCR-ODM <phpcr-odm>` to load route documents from the content repository

Configuration
-------------

If you want to use default configurations, you do not need to change anything.
The values are:

.. configuration-block::

    .. code-block:: yaml

        cmf_menu:
            menu_basepath:        /cms/menu
            document_manager_name: default
            admin_class:          ~
            document_class:       ~
            content_url_generator:  router
            route_name:           ~ # cmf routes are created by content instead of name
            content_basepath:     ~ # defaults to cmf_core.content_basepath
            allow_empty_items:    ~ # defaults to false
            voters:
                uri_prefix:       false # enable the UriPrefixVoter for current menu item
                content_identity: not set # enable the RequestContentIdentityVoter
                    content_key:  not set # override DynamicRouter::CONTENT_KEY
            use_sonata_admin:     auto # use true/false to force using / not using sonata admin
            multilang:            # the whole multilang section is optional
                use_sonata_admin:     auto # use true/false to force using / not using sonata admin
                admin_class:          ~
                document_class:       ~
                locales:              [] # if you use multilang, you have to define at least one locale

If you want to render the menu from twig, make sure you have not disabled twig
in the ``knp_menu`` configuration section.

If ``sonata-project/doctrine-phpcr-admin-bundle`` is added to the
composer.json require section, the menu documents are exposed in the
SonataDoctrinePhpcrAdminBundle.  For instructions on how to configure this
Bundle see :doc:`doctrine_phpcr_admin`.

By default, ``use_sonata_admin`` is automatically set based on whether
SonataDoctrinePhpcrAdminBundle is available but you can explicitly disable it
to not have it even if sonata is enabled, or explicitly enable to get an error
if Sonata becomes unavailable.

By default, menu items, that do not have uri or route specified, and their 
route cannot be guessed, are skipped by the bundle. So are there descendants. 
If you want to show these menu items as static text instead, just set  the 
``allow_empty_items`` parameter to true. 

Menu Entries
------------

A ``MenuItem`` document defines menu entries. You can build menu items based
on symfony routes, absolute or relative urls or referenceable PHPCR-ODM
content documents.

The menu tree is built from documents under [menu_basepath]/[menuname]. You
can use different document classes for menu items as long as they implement
``Knp\Menu\NodeInterface`` to integrate with KnpMenuBundle. The default
``MenuNode`` document discards children that do not implement this interface.

Examples::

    <?php

    // get document manager
    $dm = ...;
    $rootNode = $dm->find(null, ...); // retrieve parent menu item

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

By default content documents are created using a **weak** reference (this
means you will be able to delete the referenced content). You can specify a
strong reference by using ``setWeak(false)``::

    <?php

    $node = new MenuNode;
    // ...
    $node->setWeak(false);

.. note::

    When content is referenced weakly and subsequently deleted the rendered
    menu will not provide a link to the content.

Current Menu Item
-----------------

A menu item can be the current item. If it is the current item, this
information is passed to the twig template which by default adds the css class
``current`` and all menu items that are ancestors of that item get the class
``current_ancestor``. This will typically used in CSS to highlight menu
entries.

The decision about being current item happens by comparing the URI associated
with the menu item with the request URI. Additionally, the CMF menu bundle
supports voters that can look at the ``MenuItem`` and optionally the request.

There are two voter services configured but not enabled by default, another
voter that you can use to configure services and you can write your own voter
classes.

.. note::

    The CMF MenuBundle is based on Knp Menu 1.x. The 2.0 rewrite of Knp Menu
    will add current item voters in the core Knp library.  The CMF bundle
    voters are interface compatible and follow the same mechanism as Knp Menu
    to ease upgrading.

RequestContentIdentityVoter
~~~~~~~~~~~~~~~~~~~~~~~~~~~

This voter looks at the ``content`` field of the menu item and compares it
with the main content attribute of the request. The name for the main content
attribute in the request is configurable with the ``content_key`` option - if
not set it defaults to the constant ``DynamicRouter::CONTENT_KEY``.

You can enable this voter by setting
``cmf_menu.voters.content_identity`` to ``~`` in your config.yml to
use a custom ``content_key`` for the main content attribute name, set it
explicitly:

.. configuration-block::

    .. code-block:: yaml

        cmf_menu:
            voters:
                content_identity:
                    content_key: myKey

    .. code-block:: xml

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:cmf-menu="http://cmf.symfony.com/schema/dic/menu">
            <cmf-menu:config xmlns="http://cmf.symfony.com/schema/dic/menu">
                <voter>
                    <content-identity>
                        <content-key>myKey</content-key>
                    </content-identity>
                </voter>
            </cmf-menu:config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_menu', array(
            'voters' => array(
                'content_identity' => array(
                    'content_key' => 'myKey',
                ),
            ),
        ));

UriPrefixVoter
~~~~~~~~~~~~~~

The uri prefix voter looks at the ``content`` field of the menu item and
checks if it contains an instance of the symfony Route class. If so, it
compares the route option ``currentUriPrefix`` with the request URI. This
allows you to make a whole sub-path of your site trigger the same menu item as
current, but you need to configure the prefix option on your route documents.

To enable the prefix voter, set the configuration key
``cmf_menu.voters.uri_prefix: ~``.

RequestParentContentIdentityVoter
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This voter has the same logic of looking for a request attribute to get the
current content, but calls ``getParent`` on it. This parent is compared to the
``content`` of the menu item. That way, content that does not have its own
menu entry but a parent that does have a menu item can trigger the right menu
entry to be highlighted.

To use this voter you need to configure a custom service with the name of the
content in the request and your model class to avoid calling getParent on
objects that do not have that method.  You need to tag the service as
``cmf_menu.voter`` and also as ``cmf_request_aware`` because it
depends on the request. The service looks the same as for complete custom
voters (see below), except you do not need to write your own PHP code:

.. configuration-block::

    .. code-block:: yaml

        services:
            my_bundle.menu_voter.parent:
                class: Symfony\Cmf\Bundle\MenuBundle\Voter\RequestParentContentIdentityVoter
                arguments:
                    - mainContent
                    - %my_bundle.my_model_class%
                tags:
                    - { name: "cmf_menu.voter" }
                    - { name: "cmf_request_aware" }

    .. code-block:: xml

        <service id="my_bundle.menu_voter.parent"
                 class="Symfony\Cmf\Bundle\MenuBundle\Voter\RequestParentContentIdentityVoter">
            <argument>mainContent</argument>
            <argument>%my_bundle.my_model_class%</argument>
            <tag name="cmf_menu.voter"/>
            <tag name="cmf_request_aware"/>
        </service>

    .. code-block:: php

        $definition = new Definition(
            'Symfony\Cmf\Bundle\MenuBundle\Voter\RequestParentContentIdentityVoter',
            array('mainContent', '%my_bundle.my_model_class%')
        ));
        $definition->addTag('cmf_menu.voter');
        $definition->addTag('cmf_request_aware');
        $container->setDefinition('my_bundle.menu_voter.parent', $definition);


Custom Voter
~~~~~~~~~~~~

Voters must implement the ``Symfony\Cmf\MenuBundle\Voter\VoterInterface``.  To
make the menu bundle notice the voter, tag it with ``cmf_menu.voter``.
If the voter needs the request, add the tag ``cmf_request_aware`` to have a
listener calling ``setRequest`` on the voter before it votes for the first
time.

For an example service definition see the section above for
``RequestParentIdentityVoter``.

A voter will look something like this::

    <?php
    namespace Acme\MenuBundle\Voter;

    use Symfony\Cmf\Bundle\MenuBundle\Voter\VoterInterface;
    use Knp\Menu\ItemInterface;

    class MyVoter implements VoterInterface
    {
        private $request;

        public function setRequest(Request $request)
        {
            $this->request = $request;
        }

        /**
         * {@inheritDoc}
         */
        public function matchItem(ItemInterface $item)
        {
            if (...) {
                // $item is the current menu item
                return true;
            }
            if (...) {
                // $item for sure is NOT the current menu item
                // even if other voters might match
                return false;
            }

            // we dont know if this is the current item
            return null;
        }
    }

Rendering Menus
---------------

Adjust your twig template to load the menu.

.. code-block:: jinja

    {{ knp_menu_render('simple') }}

The menu name is the name of the node under ``menu_basepath``. For example if
your repository stores the menu nodes under ``/cms/menu`` , rendering "main"
would mean to render the menu that is at ``/cms/menu/main``

How to use Non-Default Other Components
---------------------------------------

If you use the cmf menu with PHPCR-ODM, you just need to store Route documents
under ``menu_basepath``. If you use a different object manager, you need to
make sure that the menu root document is found with::

    $dm->find($menu_document_class, $menu_basepath . $menu_name)

The route document must implement ``Knp\Menu\NodeInterface`` - see
``MenuNode`` document for an example. You probably need to specify
menu_document_class too, as only PHPCR-ODM can determine the document from the
database content.

If you use the cmf menu with the DynamicRouter, you need no route name as the
menu document just needs to provide a field content_key in the options.  If
you want to use a different service to generate URLs, you need to make sure
your menu entries provide information in your selected content_key that the
url generator can use to generate the url. Depending on your generator, you
might need to specify a route_name too.

Note that if you just want to generate normal symfony routes with a menu that
is in the database, you can pass the core router service as
content_url_generator, make sure the content_key never matches and make your
menu documents provide the route name and eventual routeParameters.

.. _`MenuBundle`: https://github.com/symfony-cmf/MenuBundle#readme
.. _`KnpMenuBundle`: https://github.com/knplabs/KnpMenuBundle
