.. index::
    single: Customization; RoutingAutoBundle

Customization
-------------

.. _routingauto_customization_pathproviders:

Adding Path Providers
~~~~~~~~~~~~~~~~~~~~~

The goal of a ``PathProvider`` class is to add one or several path elements to
the route stack. For example, the following provider will add the path
``foo/bar`` to the route stack::

    <?php

    use Symfony\Cmf\Bundle\RoutingAutoBundle\AutoRoute\PathProviderInterface;
    use Symfony\Cmf\Bundle\RoutingAutoBundle\AutoRoute\RouteStack;

    class FoobarProvider implements PathProviderInterface
    {
        public function providePath(RouteStack $routeStack)
        {
            $routeStack->addPathElements(array('foo', 'bar'));
        }
    }

To use the path provider you must register it in the container and add the
``cmf_routing_auto.provider`` tag and set the **alias** accordingly:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        services:
            my_cms_bundle.path_provider.foobar:
                class: "FoobarProvider"
                scope: prototype
                tags:
                    - { name: cmf_routing_auto.provider, alias: "foobar"}

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <service
            id="my_cms_bundle.path_provider.foobar"
            class="FoobarProvider"
            scope="prototype"
        >
            <tag name="cmf_routing_auto.provider" alias="foobar"/>
        </service>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Component\DependencyInjection\Definition;

        $definition = new Definition('FooBarProvider');
        $definition->addTag('cmf_routing_auto.provider', array('alias' => 'foobar'));
        $definition->setScope('prototype');

        $container->setDefinition('my_cms_bundle.path_provider.foobar', $definition);

The ``FoobarProvider`` is now available as **foobar** in the routing auto
configuration.

.. caution::

    Both path providers and path actions need to be defined with a scope of
    "prototype". This ensures that each time the auto routing system requests
    the class a new one is given and you do not have any state problems.

Adding Path Actions
~~~~~~~~~~~~~~~~~~~

In the auto routing system, a "path action" is an action to take if the path
provided by the "path provider" exists or not.

You can add a path action by extending the ``PathActionInterface`` and
registering your new class correctly in the DI configuration.

This is a very simple implementation from the bundle - it is used to throw an
exception when a path already exists::

    namespace Acme\MainBundle\RoutingAuto\PathNotExists;

    use Symfony\Cmf\Bundle\RoutingAutoBundle\AutoRoute\PathActionInterface;
    use Symfony\Cmf\Bundle\RoutingAutoBundle\AutoRoute\Exception\CouldNotFindRouteException;
    use Symfony\Cmf\Bundle\RoutingAutoBundle\AutoRoute\RouteStack;

    class ThrowException implements PathActionInterface
    {
        public function init(array $options)
        {
        }

        public function execute(RouteStack $routeStack)
        {
            throw new CouldNotFindRouteException('/'.$routeStack->getFullPath());
        }
    }

The ``init()`` method configures the provider (throwing errors when required
options do not exists) and the ``execute()`` method executes the action.

It is registered in the DI configuration as follows:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.xml
        services:
            cmf_routing_auto.not_exists_action.throw_exception
                class: "My\Cms\AutoRoute\PathNotExists\ThrowException"
                scope: prototype
                tags:
                    - { name: cmf_routing_auto.provider, alias: "throw_exception"}

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <service
            id="my_cms.not_exists_action.throw_exception"
            class="My\Cms\AutoRoute\PathNotExists\ThrowException"
            scope="prototype"
            >
            <tag name="cmf_routing_auto.not_exists_action" alias="throw_exception"/>
        </service>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Component\DependencyInjection\Definition;

        $definition = new Definition('My\Cms\AutoRoute\PathNotExists\ThrowException');
        $definition->addTag('cmf_routing_auto.provider', array('alias' => 'throw_exception'));
        $definition->setScope('prototype');

        $container->setDefinition('my_cms.some_bundle.path_provider.throw_exception', $definition);

Note the following:

* **Scope**: Must *always* be set to *prototype*;
* **Tag**: The tag registers the service with the auto routing system, it can
  be one of the following:

    * ``cmf_routing_auto.exists.action`` - if the action is to be used when a
      path exists;
    * ``cmf_routing_auto.not_exists.action`` - if the action is to be used when
      a path does not exist;

* **Alias**: The alias of the tag is the name by which you will reference this
  action in the auto routing configuration.
