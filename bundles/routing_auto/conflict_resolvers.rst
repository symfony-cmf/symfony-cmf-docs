.. index::
    single: Conflict Resolvers; RoutingAutoBundle

Conflict Resolvers
==================

Conflict resolvers are invoked when the system detects that a newly generated
route would conflict with a route already existing in the route repository.

This section details the conflict resolvers which are provided by default.

auto_increment
--------------

The ``auto_increment`` conflict resolver will add a numerical suffix to the
path, for example if ``my/path`` already exists, it would first become
``my/path-1`` and if that path *also* exists it will try ``my/path-2``,
``my/path-3`` and so on into infinity until it finds a path which *doesn't*
exist.

.. configuration-block::

    .. code-block:: yaml

        stdClass:
            uri_schema: /cmf/blog
            conflict_resolver: auto_increment

    .. code-block:: xml

        <auto-mapping xmlns="http://cmf.symfony.com/schema/routing_auto">
            <mapping class="stdClass" uri-schema="/cmf/blog">
                <conflict-resolver name="auto_increment" />
            </mapping>
        </auto-mapping>

throw_exception
---------------

The ``throw_exception`` efficiently "resolves" conflicts by throwing exceptions.
This is the default action.

.. configuration-block::

    .. code-block:: yaml

        stdClass:
            uri_schema: /cmf/blog
            conflict_resolver: throw_exception

    .. code-block:: xml

        <auto-mapping xmlns="http://cmf.symfony.com/schema/routing_auto">
            <mapping class="stdClass" uri-schema="/cmf/blog">
                <conflict-resolver name="throw_exception" />
            </mapping>
        </auto-mapping>

Creating a Custom Conflict Resolver
-----------------------------------

To create a custom conflict resolver, you have to implement
``ConflictResolverInterface``. This interface requires a method called
``resolveConflict`` which has access to the ``UriContext``. It returns the new
route.

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

Conflict resolvers should be registered as services and tagged with
``cmf_routing_auto.conflict_resolver``:

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
