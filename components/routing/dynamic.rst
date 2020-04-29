.. index::
    single: DynamicRouter; Routing

Dynamic Router
==============

The Symfony default Router was developed to handle static Route definitions,
as they are usually declared in configuration files, prior to execution.
The complete routing configuration is injected in the constructor. It then
creates a :class:`Symfony\\Component\\Routing\\Matcher\\UrlMatcher` with this
configuration, rather than having the matcher injected as a service. This makes
the default router a poor choice to handle dynamically defined routes. To
handle large numbers of user created routes, this component contains the
``DynamicRouter`` that is configured with a
:class:`Symfony\\Component\\Routing\\Matcher\\RequestMatcherInterface` or
:class:`Symfony\\Component\\Routing\\Matcher\\UrlMatcherInterface` service.
The actual matching logic depends on the underlying matcher implementation you
choose. You can easily use you own matching strategy by passing it to the
``DynamicRouter`` constructor. As part of this component, the
:doc:`nested_matcher` is already provided.

The DynamicRouter further allows to modify the resulting parameters of the
routing match, using a set of ``RouteEnhancerInterface`` that can be easily
configured.

The ``DynamicRouter`` is also able to generate URLs from ``Route`` objects.
The ``ProviderBasedGenerator`` can generate URLs loaded from a
``RouteProviderInterface`` instance. The ``ContentAwareGenerator`` can
determine the ``Route`` to generate the URL from any content object that
implements the ``RouteReferrersInterface``, meaning you can generate a URL
directly from a content object.

.. _components-routing-events:

Events
------

Optionally, you can provide an `Event Dispatcher`_ to the dynamic router.
If you do, it will trigger one of the pre-match events during the match
process, depending on which method is used and another event before generating
a URL:

* **cmf_routing.pre_dynamic_match** (Dispatched at the beginning of the
  ``match`` method)
* **cmf_routing.pre_dynamic_match_request** (Dispatched at the beginning of the
  ``matchRequest`` method. In the context of the Symfony full stack framework,
  only this event will be triggered.)
* **cmf_routing.pre_dynamic_generate** (Dispatched at the beginning of the
  ``generate`` method)

Pre-match events are of class ``Symfony\Cmf\Component\Routing\Event\RouterMatchEvent``,
the generate event is of class ``Symfony\Cmf\Component\Routing\Event\RouterGenerateEvent``.
The generate event also allows you to manipulate the route name, parameters and
reference type in the event, by updating the values in the event.

The ``Symfony\Cmf\Component\Routing\Event\Events`` class contains the event
constants. To learn how to register the events, see
"`How to create an Event Listener`_" in the core documentation.

Matcher
-------

The dynamic router needs to be injected a
:class:`Symfony\\Component\\Routing\\Matcher\\RequestMatcherInterface` or a
:class:`Symfony\\Component\\Routing\\Matcher\\UrlMatcherInterface`. This
component provides a suitable implementation with the :doc:`nested_matcher`.

.. _component-routing-enhancers:

Route Enhancers
---------------

Optionally, and following the matching process, a set of
``RouteEnhancerInterface`` instances can be applied by the ``DynamicRouter``.
Route enhancers are a way to manipulate the parameters from the matched route
before the framework continues. They can be used, for example, to dynamically
assign a controller or to keep logic out of the controller by determining
parameters or "upcasting" request parameters to the objects they correspond
to.

The component already provides some general purpose enhancers. They all follow
the principle to never change an existing field but only add fields if they
do not exist yet:

``RouteContentEnhancer``
    If the route is an instance of ``RouteObjectInterface``, this enhancer sets
    the target field to the return value of ``getContent()``.
``FieldMapEnhancer``
    Configured with a key-value map. If a specified field of the match contains
    a key, the target field is set to the value.
``FieldByClassEnhancer``
    Configured with a map of class names to values.  If the specified field
    contains an object that is an instance of a class in the map, sets the
    target field to the corresponding value. Note that the first match is
    taken, should the objects be instance of more than one of the classes. This
    enhancer is for example used to determine the controller and template based
    on the class of a Content document.  This enhancer is similar to
    ``FieldMapEnhancer``, but doing an `instanceof`_ check rather than string
    comparison for the map keys.
``FieldPresenceEnhancer``
    If a field is present in the route match, sets an other field to a
    specified value if that field is not set yet.
``ContentRepositoryEnhancer``
    If the source field is present in the route match, sets target field to the
    content returned by the ``ContentRepositoryInterface`` with value of the
    source field, if target field is not yet set.

You can also create your own route enhancer by creating a class which
implements ``Symfony\Cmf\Component\Routing\Enhancer\RouteEnhancerInterface``.

Route enhancers are registered using the ``addRouteEnhancer`` method, which has
an optional second argument to provide the priority.

Route Enhancer Compiler Pass
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This component provides a ``RegisterRouteEnhancersPass``. If you use the
`Symfony Dependency Injection Component`_, you can use this compiler pass to
register all enhancers having a specific tag with the dynamic router::

    use Symfony\Cmf\Component\Routing\DependencyInjection\Compiler\RegisterRouterEnhancersPass;
    use Symfony\Component\DependencyInjection\ContainerBuilder;

    // a ContainerBuilder
    $container = ...;

    $pass = new RegisterRouterEnhancersPass('cmf_routing.dynamic_router', 'dynamic_router_route_enhancer');
    $container->addCompilerPass($pass);

After adding the passes and configuring the container builder, you continue
with compiling the container as explained in the
`Symfony DI Component compilation section`_.

You can optionally configure the dynamic router service name. The compiler pass
will modify this service definition to register the enhancers when the dynamic
router is loaded from the container. If you do not specify anything, the
default service name is ``cmf_routing.dynamic_router``.

You can also configure the tag name you want to use with the second argument to
the compiler pass constructor. If you don't, the default tag is
``dynamic_router_route_enhancer``. If you are using the
:doc:`Symfony CMF RoutingBundle <../../bundles/routing/introduction>`, this tag is
already active with the default name.

Linking a Route with a Content
------------------------------

Depending on your application's logic, a requested URL may have an associated
content object. A route for such an URL may implement the
``RouteObjectInterface`` to return a content object if present. If you
configure the ``RouteContentEnhancer``, it will insert the content object into
the match array at the ``_content`` key. Notice that a ``Route`` may implement
the ``RouteObjectInterface`` but still not to return any model instance in
some cases. In that situation, the ``_content`` field will not be set.

Furthermore, routes that implement this interface can also provide a custom
Route name. The key returned by ``getRouteKey`` will be used as route name
instead of the Symfony core compatible route name and can contain any
characters. This allows you, for example, to set a path as the route name. Both
UrlMatchers provided with the ``NestedMatcher`` replace the ``_route`` key
with the route instance and put the provided name into ``_route_name``.

All routes still need to extend the base class
:class:`Symfony\\Component\\Routing\\Route <Symfony\\Component\\Routing\\Route>`
from the Symfony component.

Redirections
------------

You can create redirections by implementing the ``RedirectRouteInterface``.
It can redirect to an absolute URI, a route name that can be generated by any
Router in the chain or to another ``Route`` object.

Notice that the actual redirection logic is not handled by the bundle. You
should implement your own logic to handle the redirection. For an example of
implementing that redirection under the full Symfony stack, refer to
:doc:`the RoutingBundle <../../bundles/routing/introduction>`.

.. _component-routing-generator:

Generating URLs
---------------

Apart from matching an incoming request to a set of parameters, a Router
is also responsible for generating an URL from a route and its parameters.
The ``ChainRouter`` iterates over its known routers until one of them is
able to generate a matching URL.

Beside ``RequestMatcherInterface`` and ``UrlMatcherInterface`` to match a
Request/URL to its corresponding parameters, the ``DynamicRouter`` also uses
an ``UrlGeneratorInterface`` instance, which allows it to generate an URL from
a route.

The generator method looks like this::

    public function generate($name, $parameters = [], $referenceType = self::ABSOLUTE_PATH);

In Symfony core, the ``$name`` has to be a string with the configured name
of the route to generate. The CMF routing component adds generators that handle
alternative semantics of ``$name``.

.. versionadded:: 2.3

    Since `symfony-cmf/routing: 2.3.0`, the route document should be passed in
    the route parameters as `_route_object`, and the special route name
    `cmf_routing_object` is to be used. When using older versions of routing,
    you need to pass the route document as route name.

The ``ProviderBasedGenerator`` extends Symfony's default
:class:`Symfony\\Component\\Routing\\Generator\\UrlGenerator` (which, in turn,
implements :class:`Symfony\\Component\\Routing\\Generator\\UrlGeneratorInterface`)
and asks the route provider to find a route based on the name and parameters. It
then lets the core logic generate the URL from that ``Route``.

The CMF component also includes the ``ContentAwareGenerator``, which extends
the ``ProviderBasedGenerator``, that checks if ``_route_object`` parameter is an object
implementing ``RouteReferrersReadInterface``. If it is, it gets the ``Route``
from that object. Using the ``ContentAwareGenerator``, you can generate URLs
for your content in three ways:

* Either pass a ``Route`` object as the ``_route_object`` parameter
* Or pass a ``RouteReferrersInterface`` object that is your content as the ``_route_object`` parameter
* Or provide an implementation of ``ContentRepositoryInterface`` and pass the id
  of the content object as parameter ``content_id`` and ``cmf_routing_object`` as $name.

If you want to implement your own generator, implement the ``VersatileGeneratorInterface``
to get better debug messages for when a route can not be generated.

.. _`Event Dispatcher`: https://symfony.com/doc/current/components/event_dispatcher/index.html
.. _`How to create an Event Listener`: https://symfony.com/doc/current/cookbook/event_dispatcher/event_listener.html
.. _instanceof: http://php.net/operators.type
.. _`Symfony Dependency Injection Component`: https://symfony.com/doc/current/components/dependency_injection/index.html
.. _`Symfony DI Component compilation section`: https://symfony.com/doc/current/components/dependency_injection/compilation.html
