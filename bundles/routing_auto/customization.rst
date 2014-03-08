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

    // src/Acme/CmsBundle/RoutingAuto/PathProvider/FoobarProvider.php
    namespace Acme\CmsBundle\RoutingAuto\PathProvider;

    use Symfony\Cmf\Bundle\RoutingAutoBundle\AutoRoute\PathProvider\AbstractPathProvider;
    use Symfony\Cmf\Bundle\RoutingAutoBundle\AutoRoute\RouteStack;

    class FoobarProvider extends AbstractPathProvider
    {
        public function providePath(RouteStack $routeStack, array $options)
        {
            $routeStack->addPathElements(array('foo', 'bar'));
        }
    }

To use the path provider you must register it in the container and add the
``cmf_routing_auto.provider`` tag and set the **alias** accordingly:

.. configuration-block::

    .. code-block:: yaml

        services:
            acme_cms.path_provider.foobar:
                class: Acme\CmsBundle\RoutingAuto\PathProvider\FoobarProvider
                tags:
                    - { name: cmf_routing_auto.provider, alias: "foobar"}

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <service
                id="acme_cms.path_provider.foobar"
                class="Acme\CmsBundle\RoutingAuto\PathProvider\FoobarProvider"
            >
                <tag name="cmf_routing_auto.provider" alias="foobar"/>
            </service>
        </container>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;

        $definition = new Definition('Acme\CmsBundle\RoutingAuto\PathProvider\FoobarProvider');
        $definition->addTag('cmf_routing_auto.provider', array('alias' => 'foobar'));

        $container->setDefinition('acme_cms.path_provider.foobar', $definition);

The ``FoobarProvider`` is now available as **foobar** in the routing auto
configuration.

Adding Path Actions
~~~~~~~~~~~~~~~~~~~

In the auto routing system, a "path action" is an action to take if the path
provided by the "path provider" exists or not.

You can add a path action by extending the ``PathActionInterface`` and
registering your new class correctly in the DI configuration.

This is a very simple implementation from the bundle - it is used to throw an
exception when a path already exists::

    namespace Symfony\Cmf\Bundle\RoutingAutoBundle\RoutingAuto\PathNotExists;

    use Symfony\Cmf\Bundle\RoutingAutoBundle\AutoRoute\PathAction\AbstractPathAction;
    use Symfony\Cmf\Bundle\RoutingAutoBundle\AutoRoute\Exception\CouldNotFindRouteException;
    use Symfony\Cmf\Bundle\RoutingAutoBundle\AutoRoute\RouteStack;

    class ThrowException extends AbstractPathAction
    {
        public function execute(RouteStack $routeStack, array $options)
        {
            throw new CouldNotFindRouteException('/'.$routeStack->getFullPath());
        }
    }

The ``init()`` method configures the provider (throwing errors when required
options do not exists) and the ``execute()`` method executes the action.

It is registered in the DI configuration as follows:

.. configuration-block::

    .. code-block:: yaml

        services:
            cmf_routing_auto.not_exists_action.throw_exception:
                class: Symfony\Cmf\Bundle\RoutingAutoBundle\RoutingAuto\PathNotExists\ThrowException
                tags:
                    - { name: cmf_routing_auto.not_exists_action, alias: "throw_exception"}

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <service
                id="cmf_routing_auto.not_exists_action.throw_exception"
                class="Symfony\Cmf\Bundle\RoutingAutoBundle\RoutingAuto\PathNotExists\ThrowException"
                >
                <tag name="cmf_routing_auto.not_exists_action" alias="throw_exception"/>
            </service>
        </container>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;

        $definition = new Definition('Symfony\Cmf\Bundle\RoutingAutoBundle\RoutingAuto\PathNotExists\ThrowException');
        $definition->addTag('cmf_routing_auto.provider', array('alias' => 'throw_exception'));

        $container->setDefinition('cmf_routing_auto.not_exists_action.throw_exception', $definition);

Note the following:

* **Tag**: The tag registers the service with the auto routing system, it can
  be one of the following:

    * ``cmf_routing_auto.exists.action`` - if the action is to be used when a
      path exists;
    * ``cmf_routing_auto.not_exists.action`` - if the action is to be used when
      a path does not exist;

* **Alias**: The alias of the tag is the name by which you will reference this
  action in the auto routing configuration.

Adding Configuration to your Builder Services
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The builder services (path providers and actions) can also be configured. To
do this, you need to override the ``configureOptions`` method and configure
the ``OptionsResolver`` class::

    use Symfony\Component\OptionsResolver\OptionsResolverInterface;
    // ...

    class FoobarProvider extends AbstractPathProvider
    {
        public function configureOptions(OptionsResolverInterface $resolver)
        {
            $resolver->setDefaults(array(
                'capitialize' => false,
            ));
        }

        public function providePath(RouteStack $routeStack, array $options)
        {
            $pathElements = array('foo', 'bar');

            if ($options['captialize']) {
                $pathElements = array_map('ucfirst', $pathElements);
            }

            $routeStack->addPathElements($pathElements);
        }
    }

Read more about it in the documentation of the `OptionsResolver Component`_

.. _`OptionsResolver Component`: http://symfony.com/doc/current/doc/components/options_resolver.rst
