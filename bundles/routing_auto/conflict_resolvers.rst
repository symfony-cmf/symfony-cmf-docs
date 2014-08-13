.. index::
    single: Conflict Resolvers; RoutingAutoBundle

Conflict Resolvers
------------------

Conflict resolvers are invoked when the system detects that a newly generated
route would conflict with a route already existing in the route repository.

This section details the conflict resolvers which are provided by default.

auto_increment
~~~~~~~~~~~~~~

The ``auto_increment`` conflict resolver will add a numerical suffix to the path, for
example if ``my/path`` already exists, it would first become ``my/path-1`` and if that path *also*
exists it will try ``my/path-2``, ``my/path-3`` and so on into infinity until
it finds a path which *doesn't* exist.

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
~~~~~~~~~~~~~~~

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
