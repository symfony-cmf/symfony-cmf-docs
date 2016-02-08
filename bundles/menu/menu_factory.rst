.. index::
    single: menu factory; MenuBundle

Menu Factory
============

The menu documents are only used for persisting the menu data, they are not
actually used when rendering a menu. Menu items are the objects that are needed
to render a menu. A menu factory creates such *menu items* from the *menu
nodes* provided by the menu provider.

.. _bundles_menu_menu_factory_url_generation:

URL Generation
--------------

.. versionadded:: 2.0
    Adding content support to the ``knp_menu.factory`` service was introduced
    in CmfMenuBundle 2.0. Prior to 2.0, you had to use the
    ``ContentAwareFactory`` class and ``cmf_menu.factory`` service.

Most menu items will need a URL. By default, KnpMenu allows generating this URL
by specifying a URI or a Symfony route name.

The CmfMenuBundle provides another way to generate URLs: By using the
:ref:`dynamic router <bundles-routing-dynamic-generator>` to generate routes
from content objects.

The ``content`` menu node option, if specified, must contain something that the
content URL generator can work with. When using the :ref:`dynamic router
<bundles-routing-dynamic-generator>`, this needs to be a class implementing the
``RouteReferrersInterface``.

.. tip::

    When you don't use the dynamic router, you can create a custom url
    generator by implementing ``UrlGeneratorInterface`` and configure it using
    the ``content_url_generator`` option in ``config.yml``.

    .. versionadded:: 1.2
        The ``content_url_generator`` option was introduced in CmfMenuBundle 1.2.

How to handle Items without a URL
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When menu nodes refer to content that has been deleted or there is another
error during route generation, a menu node is skipped by default. You can set
the ``allow_empty_items`` setting of the CmfMenuBundle to ``true`` to render
these nodes as plain text instead.

.. _bundles_menu_menu_factory_link_type:

Specifying the Link Type
~~~~~~~~~~~~~~~~~~~~~~~~

The CMF provides a ``linkType`` option, which enables you to specify which URL
generation technique (URI, route or content) should be used.

The values for this options can be one of the following:

`null``
    If the value is ``null`` (or the option is not set) then the link type is
    determined automatically by looking at each of the ``uri``, ``route`` and
    ``content`` options; using the first one which is non-null;

uri
    Use the URI provided by the ``uri`` option;

route
    Generate a URL using the route named by the ``route`` option and the
    parameters provided by the ``routeParameters`` option;

content
    Generate a URL by passing the value of the ``content`` option to the
    content URL generator, using any parameters provided by the
    ``routeParameters`` option.

Publish Workflow
~~~~~~~~~~~~~~~~

The CmfMenuBundle provides a ``MenuContentVoter``, which checks if the
referenced content is published using the
:doc:`publish workflow checker <../core/publish_workflow>`. If the content is
not yet published, the menu item not will not be rendered.

Customizing Menus using Events
------------------------------

The CmfMenuBundle dispatches a ``cmf_menu.create_menu_item_from_node`` event
during the process of creating a menu item from a menu node. You can use this
event to control the ``MenuItem`` that is created and to mark the current node
or its children as skipped.

Listeners for this method receive a ``CreateMenuItemFromNodeEvent`` instance,
which provides access to node using the ``getNode()`` method and allows
skipping nodes using the ``setSkipNode()`` and ``setSkipChildren()`` methods.

.. note::

    If you mark the ``Menu`` document (the root node of each menu) as skipped,
    an empty item is still created to avoid errors when rendering a menu.

You can use the ``setItem()`` method to set the menu item to use instead of one
generated using the menu node. The child nodes are still processed like normal
and added to this new item.

.. tip::

    You can inject the ``knp_menu.factory`` service in the listener to generate
    new menu items from nodes.

Example Menu Listener
~~~~~~~~~~~~~~~~~~~~~

This listener handles menu nodes that point to a different menu by implementing
the ``MenuReferrerInterface``::

    // src/AppBundle/Menu/MenuReferrerInterface.php
    namespace AppBundle\Menu;

    interface MenuReferrerInterface
    {
        public function getMenuName();
        public function getMenuOptions();
    }

    // src/AppBundle/EventListener/CreateMenuItemFromMenuListener.php
    namespace AppBundle\EventListener;

    use Symfony\Cmf\Bundle\MenuBundle\Event\CreateMenuItemFromNodeEvent;
    use Knp\Menu\Provider\MenuProviderInterface;
    use AppBundle\Menu\MenuReferrerInterface;

    class CreateMenuItemFromMenuListener
    {
        private $provider;

        public function __construct(MenuProviderInterface $provider)
        {
            $this->provider = $provider;
        }

        public function onCreateMenuItemFromNode(CreateMenuItemFromNodeEvent $event)
        {
            $node = $event->getNode();

            if ($node instanceof MenuReferrerInterface) {
                $menuName = $node->getMenuName();
                $menuOptions = $node->getMenuOptions();

                if (!$this->provider->has($menuName)) {
                    return;
                }

                $menu = $this->provider->get($menuName, $menuOptions);
                $event->setItem($menu);
            }
        }

    }

The service needs to be tagged as event listener:

.. configuration-block::

    .. code-block:: yaml

        # app/config/services.yml
        services:
            app.menu_referrer_listener:
                class: AppBundle\EventListener\CreateMenuItemFromMenuListener
                arguments: ['@knp_menu.menu_provider']
                tags:
                    -
                        name: kernel.event_listener
                        event: cmf_menu.create_menu_item_from_node
                        method: onCreateMenuItemFromNode

    .. code-block:: xml

        <!-- app/config/services.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <services>
                <service id="acme_demo.listener.menu_referrer_listener"
                    class="AppBundle\EventListener\CreateMenuItemFromMenuListener"
                >
                    <argument type="service" id="knp_menu.menu_provider" />

                    <tag name="kernel.event_listener"
                        event="cmf_menu.create_menu_item_from_node"
                        method="onCreateMenuItemFromNode"
                    />
                </service>
            </services>
        </container>

    .. code-block:: php

        // app/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        $definition = new Definition('AppBundle\EventListener\CreateMenuItemFromMenuListener', array(
            new Reference('knp_menu.menu_provider'),
        ));
        $definition->addTag('kernel.event_listener', array(
            'event' => 'cmf_menu.create_menu_item_from_node',
            'method' => 'onCreateMenuItemFromNode',
        ));

        $container->setDefinition('acme_demo.listener.menu_referrer_listener', $definition);
