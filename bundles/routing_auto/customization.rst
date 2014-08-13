.. index::
    single: Customization; RoutingAutoBundle

Customization
-------------

.. _routingauto_customization_tokenproviders:

Token Providers
~~~~~~~~~~~~~~~

The goal of a ``TokenProvider`` class is to provide values for tokens in the
URI schema. Such values can be derived from the object for which the route
is being generated, or from the environment (e.g. the you could use the
current locale in the route).

The following token provider will simply provide the value "foobar"::

    // src/Acme/CmsBundle/RoutingAuto/PathProvider/FoobarTokenProvider.php
    namespace Symfony\Cmf\Component\RoutingAuto\TokenProvider;

    use Symfony\Cmf\Component\RoutingAuto\TokenProviderInterface;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;
    use Symfony\Cmf\Component\RoutingAuto\UriContext;

    class FoobarTokenProvider implements TokenProviderInterface
    {
        /**
         * {@inheritDoc}
         */
        public function provideValue(UriContext $uriContext, $options)
        {
            return 'foobar';
        }

        /**
         * {@inheritDoc}
         */
        public function configureOptions(OptionsResolverInterface $optionsResolver)
        {
        }
    }

To use the path provider you must register it in the container and add the
``cmf_routing_auto.token_provider`` tag and set the **alias** accordingly:

.. configuration-block::

    .. code-block:: yaml

        services:
            acme_cms.token_provider.foobar:
                class: Acme\CmsBundle\RoutingAuto\PathProvider\FoobarTokenProvider
                tags:
                    - { name: cmf_routing_auto.token_provider, alias: "foobar" }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <service
                id="acme_cms.token_provider.foobar"
                class="Acme\CmsBundle\RoutingAuto\PathProvider\FoobarTokenProvider"
            >
                <tag name="cmf_routing_auto.token_provider" alias="foobar"/>
            </service>
        </container>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;

        $definition = new Definition('Acme\CmsBundle\RoutingAuto\PathProvider\FoobarTokenProvider');
        $definition->addTag('cmf_routing_auto.token_provider', array('alias' => 'foobar'));

        $container->setDefinition('acme_cms.token_provider.foobar', $definition);

The ``FoobarTokenProvider`` is now available as **foobar** in the routing auto
configuration.

Conflict Resolvers
~~~~~~~~~~~~~~~~~~

Conflict resolvers decide what happens if a generated route already exists in
the route repository and is not related to the context object.

The following example will append a unique string to the URI to resolve a
conflict::

    namespace Symfony\Cmf\Component\RoutingAuto\ConflictResolver;

    use Symfony\Cmf\Component\RoutingAuto\ConflictResolverInterface;
    use Symfony\Cmf\Component\RoutingAuto\UriContext;
    use Symfony\Cmf\Component\RoutingAuto\Adapter\AdapterInterface;

    class UniqidConflictResolver implements ConflictResolverInterface
    {
        public function resolveConflict(UriContext $uriContext)
        {
            $uri = $uriContext->getUri();
            return sprintf('%s-%s', uniqid());
        }
    }

It is registered in the DI configuration as follows:

.. configuration-block::

    .. code-block:: yaml

        services:
            acme_cms.conflict_resolver.foobar:
                class: Acme\CmsBundle\RoutingAuto\ConflictResolver\UniqidConflictResolver
                tags:
                    - { name: cmf_routing_auto.conflict_resolver, alias: "uniqid"}

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <service
                id="acme_cms.conflict_resolver.foobar"
                class="Acme\CmsBundle\RoutingAuto\ConflictResolver\UniqidConflictResolver"
            >
                <tag name="cmf_routing_auto.conflict_resolver" alias="uniqid"/>
            </service>
        </container>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;

        $definition = new Definition('Acme\CmsBundle\RoutingAuto\ConflictResolver\UniqidConflictResolver');
        $definition->addTag('cmf_routing_auto.conflict_resolver', array('alias' => 'foobar'));

        $container->setDefinition('acme_cms.conflict_resolver.uniqid', $definition);

Defunct Route Handlers
~~~~~~~~~~~~~~~~~~~~~~

Defunct Route Handlers decide what happens to old routes when an object is
updated and its generated URI changes.

They are not all-together trivial - the following handler removes old routes and is
the default handler::

    namespace Symfony\Cmf\Component\RoutingAuto\DefunctRouteHandler;

    use Symfony\Cmf\Component\RoutingAuto\DefunctRouteHandlerInterface;
    use Symfony\Cmf\Component\RoutingAuto\UriContextCollection;
    use Symfony\Cmf\Component\RoutingAuto\Adapter\AdapterInterface;

    class RemoveDefunctRouteHandler implements DefunctRouteHandlerInterface
    {
        protected $adapter;

        public function __construct(AdapterInterface $adapter)
        {
            $this->adapter = $adapter;
        }

        public function handleDefunctRoutes(UriContextCollection $uriContextCollection)
        {
            $referringAutoRouteCollection = $this->adapter->getReferringAutoRoutes($uriContextCollection->getSubjectObject());

            foreach ($referringAutoRouteCollection as $referringAutoRoute) {
                if (false === $uriContextCollection->containsAutoRoute($referringAutoRoute)) {
                    $newRoute = $uriContextCollection->getAutoRouteByTag($referringAutoRoute->getAutoRouteTag());

                    $this->adapter->migrateAutoRouteChildren($referringAutoRoute, $newRoute);
                    $this->adapter->removeAutoRoute($referringAutoRoute);
                }
            }
        }
    }

It is registered in the DI configuration as follows:

.. configuration-block::

    .. code-block:: yaml

        services:
            acme_cms.defunct_route_handler.foobar:
                class: Acme\CmsBundle\RoutingAuto\DefunctRouteHandler\RemoveConflictResolver
                tags:
                    - { name: cmf_routing_auto.defunct_route_handler, alias: "remove"}

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <service
                id="acme_cms.defunct_route_handler.foobar"
                class="Acme\CmsBundle\RoutingAuto\DefunctRouteHandler\RemoveConflictResolver"
            >
                <tag name="cmf_routing_auto.defunct_route_handler" alias="remove"/>
            </service>
        </container>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;

        $definition = new Definition('Acme\CmsBundle\RoutingAuto\DefunctRouteHandler\RemoveConflictResolver');
        $definition->addTag('cmf_routing_auto.defunct_route_handler', array('alias' => 'foobar'));

        $container->setDefinition('acme_cms.defunct_route_handler.remove', $definition);
