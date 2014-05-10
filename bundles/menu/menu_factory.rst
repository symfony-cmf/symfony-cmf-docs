.. index::
    single: menu factory; MenuBundle

Menu Factory
============

The menu documents are only used for persisting the menu data, they are not
actually used when rendering a menu. ``MenuItem`` classes from the KnpMenu
component are the objects that are needed, a menu factory takes data provided by
a class implementing ``NodeInterface`` and creates a ``MenuItem`` tree.

.. _bundles_menu_menu_factory_url_generation:

URL Generation
--------------

A menu item should have a URL associated with it. The CMF provides the
``ContentAwareFactory``, which extends the KnpMenu ``RouterAwareFactory`` and
``MenuFactory``.

* The ``MenuFactory`` only supports using the ``uri`` option to specify the
  menu items link.
* The ``RouterAwareFactory`` adds support for for generating a URL from the
  ``route`` and ``routeParameters`` options.
* The CMF adds the ``ContentAwareFactory`` which supports generating the URL
  from the ``content`` and ``routeParameters`` options when using the
  :ref:`dynamic router <bundles-routing-dynamic-generator>`.

The ``content`` option, if specified, must contain a class which implements
the ``RouteReferrersInterface``, see the :ref:`dynamic router
<bundles-routing-dynamic-generator>` documentation for more information.

URL generation is absolute or relative, depending on the boolean value of the
``routeAbsolute`` option.

.. _bundles_menu_menu_factory_link_type:

Link Type
---------

The ``linkType`` option is a CMF addition which enables you to specify which
of the three URL generation techniques to use.

The values for this options can be one of the following:

* ``null``: If the value is ``null`` (or the options is not set) then the link
  type is determined automatically by looking at each of the ``uri``, ``route`` and
  ``content`` options and using the first one which is non-null.
* **uri**: Use the URI provided by the ``uri`` option.
* **route**: Generate a URL using the route named by the ``route`` option
  and the parameters provided by the ``routeParameters`` option.
* **content**: Generate a URL by passing the ``RouteReferrersInterface``
  instance provided by the ``content`` option to the router using any
  parameters provided by the ``routeParameters`` option.

Publish Workflow
----------------

The CMF menu factory also determines if menu nodes are published and therefore
visible by use of the :doc:`publish workflow checker
<../core/publish_workflow>`.

.. versionadded:: 1.1
    The ``MenuContentVoter`` was added in CmfMenuBundle 1.1.

The ``MenuContentVoter`` decides that a menu node is not published if the
content it is pointing to is not published.

Customizing Menus using Events
------------------------------

The CMF menu factory dispatches a ``cmf_menu.create_menu_item_from_node`` event
during the process of creating a ``MenuItem`` from a class implementing
``NodeInterface``. You can use this event to control the ``MenuItem`` that is
created. The ``CreateMenuItemFromNodeEvent`` provides access to the
``NodeInterface`` and ``ContentAwareFactory``, which can be used to create a
custom ``MenuItem``, or to tell the factory to ignore the current node or its
children. For example, this event is used by the
:doc:`publish workflow checker <../core/publish_workflow>` to skip
``MenuItem`` generation for unpublished nodes.

The ``CreateMenuItemFromNodeEvent`` which is dispatched includes the following
methods which can be used to customize the creation of the ``MenuItem`` for a
``NodeInterface``.

* ``CreateMenuItemFromNodeEvent::setSkipNode(true|false)``: Setting skipNode
  to true will prevent creation of item from the node and skip any child nodes.
  **Note:** If setSkipNode(true) is called for ``Menu`` the
  ``ContentAwareFactory`` will still create an empty item for the menu. This is
  to prevent the KnpMenuBundle code from throwing an exception due to ``null``
  being passed to a function to render a menu;
* ``CreateMenuItemFromNodeEvent::setItem(ItemInterface $item|null)``: A
  listener can call setItem to provide a custom item to use for the given node.
  If an item is set, the ``ContentAwareFactory`` will use it instead of
  creating one for the node. The children of the node will still be processed
  by the ``ContentAwareFactory`` and listeners will have an opportunity then to
  override their items using this method;
* ``CreateMenuItemFromNodeEvent::setSkipChildren(true|false)``: Listeners can
  set this to true and the ``ContentAwareFactory`` will skip processing of the
  children of the current node.

Example Menu Listener
~~~~~~~~~~~~~~~~~~~~~

This listener handles menu nodes that point to a different menu by implementing
the ``MenuReferrerInterface``::

    namespace Acme\DemoBundle;

    interface MenuReferrerInterface
    {
        public function getMenuName();
        public function getMenuOptions();
    }

    namespace Acme\DemoBundle\EventListener;

    use Symfony\Cmf\Bundle\MenuBundle\Event\CreateMenuItemFromNodeEvent;
    use Acme\DemoBundle\MenuReferrerInterface;
    use Knp\Menu\Provider\MenuProviderInterface;

    class CreateMenuItemFromNodeListener
    {
        protected $provider;

        public function __construct(MenuProviderInterface $provider)
        {
            $this->provider = $provider;
        }

        public function onCreateMenuItemFromNode(CreateMenuItemFromNodeEvent $event)
        {
            $node = $event->getNode();

            if ($node implements MenuReferrerInterface) {
                $menuName = $node->getMenuName();
                $menuOptions = $node->getMenuOptions();

                if (!$this->provider->has($menuName)) {
                    return;
                }

                $menu = $this->provider->get($menuName, $menuOptions);
                $event->setItem($menu);

                // as this does not call $event->setSkipChildren(true),
                // children of $node will be rendered as children items of $menu.
            }
        }

    }

The service needs to be tagged as event listener:

.. configuration-block::

    .. code-block:: yaml

        services:
            acme_demo.listener.menu_referrer_listener:
                class: Acme\DemoBundle\EventListener\CreateMenuItemFromNodeListener
                arguments:
                    - "@knp_menu.menu_provider"
                tags:
                    -
                        name: kernel.event_listener
                        event: cmf_menu.create_menu_item_from_node
                        method: onCreateMenuItemFromNode

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <service id="acme_demo.listener.menu_referrer_listener" class="Acme\DemoBundle\EventListener\CreateMenuItemFromNodeListener">
                <argument type="service" id="knp_menu.menu_provider" />
                <tag name="kernel.event_listener"
                    event="cmf_menu.create_menu_item_from_node"
                    method="onCreateMenuItemFromNode"
                />
            </service>
        </container>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        $definition = new Definition('Acme\DemoBundle\EventListener\CreateMenuItemFromNodeListener', array(
            new Reference('knp_menu.menu_provider'),
        ));
        $definition->addTag('kernel.event_listener', array(
            'event' => 'cmf_menu.create_menu_item_from_node',
            'method' => 'onCreateMenuItemFromNode',
        ));

        $container->setDefinition('acme_demo.listener.menu_referrer_listener', $definition);
