.. index::
    single: Routing; Components
    single: Routing

Routing
=======

The Symfony CMF Routing component extends the Symfony2 core routing
component to allow more flexibility. The most important difference is that
the CMF Routing component can load routing information from a database. Like
the Symfony2 routing component, it does not need the full Symfony2 Framework
and can be used in standalone projects as well.

At the core of the Symfony CMF Routing component is the `ChainRouter`_. The
ChainRouter tries to match a request with each of its registered routers,
ignoring the :class:`Symfony\\Component\\Routing\\Exception\\ResourceNotFoundException`
until all routers got a chance to match. The first match wins - if no router
matched, the :class:`Symfony\\Component\\Routing\\Exception\\ResourceNotFoundException`
is thrown. The default Symfony2 router can be added to this chain, so the
standard routing mechanism can still be used in addition to any custom routing.

Additionally, this component is meant to provide useful implementations of the
routing interfaces. Currently, it provides the
:ref:`DynamicRouter <component-routing-dynamic>`. This flexible router uses an
injected :class:`Symfony\\Component\\Routing\\Matcher\\RequestMatcherInterface`
instance instead of the hardcoded way the Symfony2 core router works.
``DynamicRouter`` also applies ``RouteEnhancerInterface`` strategies in order
to post-process the route match information. The provided `NestedMatcher`_ is
an implementation of the ``RequestMatcherInterface`` that dynamically retrieves
Symfony2 :class:`Symfony\\Component\\Routing\\Route` objects from a
``RouteProviderInterface``. The ``DynamicRouter`` is also able to generate
URLs from ``Route`` objects. The ``ProviderBasedGenerator`` can generate URLs
loaded from a ``RouteProviderInterface`` instance. The
``ContentAwareGenerator`` can determine the ``Route`` to generate the URL from
any content object that implements the ``RouteReferrersInterface``, meaning you
can generate a URL directly from a content object.

The goal of Routing
-------------------

Routing is the task of a framework to determine, based on the web request, what
code it has to call and which parameters to apply. The Symfony2 core
:class:`Symfony\\Component\\Routing\\Matcher\\RequestMatcherInterface` defines
that a router must convert a :class:`Symfony\\Component\\Routing\\Request` into
an array of routing information. In the full stack Symfony2 framework, the code
to call is defined in the ``_controller`` field of the match parameters. The
framework is going to call the controller specified, matching eventual
parameters of that method by name with the other parameters found in the match
array or in the ``Request`` object attributes field.

.. note::

    For a good introduction to routing in the Symfony2 framework, please read
    the `Routing chapter of the Symfony2 book`_.

    To use this component outside of the Symfony2 framework context, have a
    look at the core Symfony2 `Routing Component`_ to get a fundamental
    understanding of the component. The CMF Routing Component just extends the
    basic behaviour.

Installation
------------

You can install the component in 2 different ways:

* Use the official Git repository (https://github.com/symfony-cmf/Routing);
* `Install it via Composer`_ (``symfony-cmf/routing`` on `Packagist`_).

Dependencies
~~~~~~~~~~~~

The Component depends on the Symfony2 `Routing`_ component and the
`HttpKernel`_ component.

.. tip::

    For the ``DynamicRouter`` you will need something to implement the
    ``RouteProviderInterface`` with. We suggest using Doctrine as this
    provides an easy way to map classes into a database.

    The :doc:`RoutingBundle <../bundles/routing/introduction>` provides
    implementations for everything needed to get this component running with
    Doctrine PHPCR-ODM and the Doctrine ORM.

ChainRouter
-----------

At the core of the Symfony CMF Routing component sits the ``ChainRouter``. It
is used as a replacement for Symfony2's default routing system.

The ``ChainRouter`` works by accepting a set of prioritized routing
strategies, :class:`Symfony\\Component\\Routing\\RouterInterface`
implementations, commonly referred to as "routers".

When handling an incoming request, the ``ChainRouter`` iterates over the
configured routers, sorted by their configured priority, until one of them is
able to :method:`match <Symfony\\Component\\Routing\\Matcher\\RequestMatcherInterface::matchRequest>`
the request or to :method:`match <Symfony\\Component\\Routing\\RouterInterface::match>`
the URL and provide the request parameters.

.. note::

    Historically, the router had to do the matching on the URL string alone.
    Since Symfony 2.2, it can alternatively implement the
    ``RequestMatcherInterface`` to do the matching on the full
    :class:`Symfony\\Component\\Routing\\Request` object if it wants to use
    other request information into account like the domain or accept-encoding.
    The ``ChainRouter`` supports both types of route matching.

Routers are added using the ``add`` method of the ``ChainRouter``. Use this to
add the default Symfony2 router::

    use Symfony\Component\Routing\Router;
    use Symfony\Cmf\Component\Routing\ChainRouter;

    $chainRouter = new ChainRouter();
    $chainRouter->add(new Router(...));
    $chainRouter->match('/foo/bar');

Now, when the ``ChainRouter`` matches a request, it will ask the Symfony2
``Router`` to match see if it matches. If there is no match, it will throw a
:class:`Symfony\\Component\\Routing\\Exception\\ResourceNotFoundException`.

If you add a new router, for instance the ``DynamicRouter``, it will be
called after the Symfony2 Router (because that was added first). To control the
order, you can use the second argument of the ``add`` method to set a priority.
Higher priorities are sorted first.

.. code-block:: php

    use Symfony\Cmf\Component\Routing\DynamicRouter;
    // ...

    $chainRouter->add(new Router(...), 1);

    $dynamicRouter = new DynamicRouter(...);
    // ...
    $chainRouter->add($dynamicRouter, 100);

.. note::

    You'll learn how to instantiate the :ref:`DynamicRouter <component-routing-dynamic>`
    later in this article.

Routers
-------

The ``ChainRouter`` is incapable of, by itself, making any actual routing
decisions. Its sole responsibility is managing the given set of Routers,
which are responsible for matching a request and determining its parameters.

You can easily create your own routers by implementing
:class:`Symfony\\Component\\Routing\\RouterInterface` but the Symfony CMF
Routing Component already includes a powerful route matching system that you
can extend to your needs.

Symfony2 Default Router
~~~~~~~~~~~~~~~~~~~~~~~

The Symfony2 routing mechanism is itself a ``RouterInterface`` implementation,
which means you can use it as a Router in the ``ChainRouter``. This allows you
to use the default routing declaration system. Read more about this router in
the `Routing Component`_ article of the core documentation.

.. _component-routing-dynamic:

Dynamic Router
~~~~~~~~~~~~~~

The Symfony2 default Router was developed to handle static Route definitions,
as they are traditionally declared in configuration files, prior to execution.
It is injected the whole routing configuration in its constructor.
This makes it a poor choice to handle dynamically defined routes. To handle
large numbers of user created routes, this bundle comes with the
``DynamicRouter``. This router uses an interface to load Routes dynamically
from data sources like database storage. It further allows to modify the
resulting parameters using a set of enhancers that can be easily configured,
greatly extending Symfony2's default functionality.

The ``DynamicRouter`` uses a ``RequestMatcherInterface`` or ``UrlMatcherInterface``
instance to match the received Request or URL, respectively, to a parameters array.
The actual matching logic depends on the underlying implementation you choose.
You can easily use you own matching strategy by passing it to the ``DynamicRouter``
constructor. As part of this component, a `NestedMatcher`_ is already provided
which you can use straight away, or as reference for your own implementation.

Its other feature is the ``RouteEnhancerInterface``, used to infer routing
parameters from the information provided by the match.

.. _components-routing-events:

Events
......

Optionally, you can provide an `Event Dispatcher`_ to the dynamic router.
If you do, it will trigger one of the following two events during the match
process, depending on which method is used:

* **cmf_routing.pre_dynamic_match** (Dispatched at the beginning of the
  ``match`` method)
* **cmf_routing.pre_dynamic_match_request** (Dispatched at the beginning of the
  ``matchRequest`` method. In the context of the Symfony2 full stack framework,
  only this event will be triggered.)

The ``Symfony\Cmf\Component\Routing\Event\Events`` class contains the event
constants. To learn how to register the events, see "`How to create an Event
Listener`_" in the core documentation.

NestedMatcher
.............

The provided ``RequestMatcherInterface`` implementation is the
``NestedMatcher``. It is suitable to use with ``DynamicRouter``, and it uses
a multiple step matching process to determine the resulting routing parameters
from a given :class:`Symfony\\Component\\HttpFoundation\\Request`.

The ``NestedMatcher`` uses a 3-step matching process to determine which Route
to use when handling the current Request:

#. Ask the ``RouteProviderInterface`` for the collection of ``Route`` instances
   potentially matching the ``Request``;
#. Apply all ``RouteFilterInterface`` to filter down this collection;
#. Let the ``FinalMatcherInterface`` instance decide on the best match among
   the remaining ``Route`` instances and transform it into the parameter array.

1. The RouteProvider
""""""""""""""""""""

Although the ``RouteProviderInterface`` can be used in other ways, it's main
goal is to be easily implemented on top of Doctrine PHPCR ODM or any other
database, effectively allowing you to store and manage routes dynamically from
the database.

Based on the ``Request``, the ``NestedMatcher`` will retrieve an ordered
collection of ``Route`` objects from the Route Provider. The idea of this
provider is to provide all routes that could potentially match, but **not** to
do any elaborate matching operations yet - this will be done in the later steps.

The underlying implementation of the ``RouteProviderInterface`` is not in the
scope of this component. The :doc:`RoutingBundle <../bundles/routing/introduction>`
provides Route Providers for both Doctrine ORM and Doctrine PHPCR-ODM.

To create and register your own Route Provider, create a class implementing
``Symfony\Cmf\Component\Routing\RouteProviderInterface`` which will have the
following methods::

    use Symfony\Cmf\Component\Routing\RouteProviderInterface;

    use Symfony\Component\Routing\RouteCollection;
    use Symfony\Component\Routing\Exception\RouteNotFoundException;

    class DoctrineOrmRouteProvider implements RouteProviderInterface
    {
        // ...

        public function getRouteCollectionForRequest(Request $request)
        {
            // you should do some simple filtering on the URL here
            $routes = $this->routeRepository->findAll();

            $collection = new RouteCollection();
            if (0 === count($routes)) {
                return $collection; // an empty collection means no routes found
            }

            foreach ($routes as $route) {
                $collection->add($route);
            }

            return $collection;
        }

        public function getRouteByName($name, $parameters = array())
        {
            $route = $this->routeRepository->findByName($name);
            if (!$route) {
                throw new RouteNotFoundException("No route found for path '$name'");
            }

            return $route;
        }

        public function getRoutesByNames($names, $parameters = array())
        {
            return $this->routeRepository->createQueryBuilder('r')
                ->where("r.name IN (:names)")
                ->setParameter(':names', '"'.implode('","', $names.'"'))
                ->getQuery()
                ->getResult();
        }
    }

The Route Provider is set using the first argument of the constructor for the
``NestedMatcher``::

    use Symfony\Cmf\Component\Routing\NestedMatcher\NestedMatcher;
    // ...

    $routeProvider = new DoctrineOrmRouteProvider(...);
    $nestedMatcher = new NestedMatcher($routeProvider, ...);

.. _components-routing-filters:

2. The Route Filters
""""""""""""""""""""

The ``NestedMatcher`` can apply user provided ``RouteFilterInterface``
implementations to reduce the provided ``Route`` objects, e.g. for doing
content negotiation. It is the responsibility of each filter to throw the
``ResourceNotFoundException`` if no more routes are left in the collection.

Filters are created by implementing
``Symfony\Cmf\Component\Routing\NestedMatcher\RouteFilterInterface``. They can
be registered with the ``addRouteFilter`` method, which has an optional second
argument to set the priority.

.. note::

    The filter step is optional and meant for special cases. The
    CmfRoutingBundle does not use any filters by default.

3. The Final Matcher
""""""""""""""""""""

The ``FinalMatcherInterface`` implementation has to find exactly one Route or
throw an exception if no adequate match could be found. The default
implementation uses the
:class:`Symfony\\Component\\Routing\\Matcher\\UrlMatcher` of the Symfony
Routing Component and is called
``Symfony\Cmf\Component\Routing\NestedMatcher\UrlMatcher``.

You can create your own final matcher by implementing
``Symfony\Cmf\Component\Routing\NestedMatcher\FinalMatcherInterface``.

The final matcher is set using the second argument of the constructor of the
``NestedMatcher``::

    use Symfony\Cmf\Component\Routing\NestedMatcher\UrlMatcher
    // ...

    $finalMatcher  = new UrlMatcher(...);
    $nestedMatcher = new NestedMatcher($routeProvider, $finalMatcher);

.. _component-routing-enhancers:

Route Enhancers
...............

Optionally, and following the matching process, a set of
``RouteEnhancerInterface`` instances can be applied by the ``DynamicRouter``.
Route enhancers are a way to manipulate the parameters from the matched route
before the framework continues. They can be used, for example, to dynamically
assign a controller or to keep logic out of the controller by determining
parameters or "upcasting" request parameters to to the objects they correspond
to. The component already provides some general purpose enhancers:

* `FieldByClassEnhancer`_
* `FieldMapEnhancer`_
* `FieldPresenceEnhancer`_
* `RouteContentEnhancer`_

You can also create your own route enhancer by creating a class which
implements ``Symfony\Cmf\Component\Routing\Enhancer\RouteEnhancerInterface``.
Route enhancers are registered using the ``addRouteEnhancer`` method, which has
an optional second argument to provide the priority.

Linking a Route with a Content
..............................

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
from the Symfony2 component.

Redirections
............

You can build redirections by implementing the ``RedirectRouteInterface``.
It can redirect to an absolute URI, a route name that can be generated by any
Router in the chain or to another ``Route`` object.

Notice that the actual redirection logic is not handled by the bundle. You
should implement your own logic to handle the redirection. For an example of
implementing that redirection under the full Symfony2 stack, refer to
:doc:`the RoutingBundle <../bundles/routing/introduction>`.

.. _component-routing-generator:

Generating URLs
~~~~~~~~~~~~~~~

Apart from matching an incoming request to a set of parameters, a Router
is also responsible for generating an URL from a route and its parameters.
The ``ChainRouter`` iterates over its known routers until one of them is
able to generate a matching URL.

Beside ``RequestMatcherInterface`` and ``UrlMatcherInterface`` to match a
Request/URL to its corresponding parameters, the ``DynamicRouter`` also uses
an ``UrlGeneratorInterface`` instance, which allows it to generate an URL from
a route.

The generator method looks like this::

    public function generate($name, $parameters = array(), $referenceType = self::ABSOLUTE_PATH);

In Symfony2 core, the ``$name`` has to be a string with the configured name
of the route to generate. The CMF routing component adds generators that handle
alternative semantics of ``$name``.

The ``ProviderBasedGenerator`` extends Symfony2's default
:class:`Symfony\\Component\\Routing\\Generator\\UrlGenerator` (which, in turn,
implements :class:`Symfony\\Component\\Routing\\Generator\\UrlGeneratorInterface``)
and - if the name is not already a ``Route`` object - loads the Route from the
Route provider. It then lets the core logic generate the URL from that ``Route``.

The CMF component also includes the ``ContentAwareGenerator``, which extends
the ``ProviderBasedGenerator``, that checks if ``$name`` is an object
implementing ``RouteReferrersReadInterface``. If it is, it gets the ``Route``
from that object. Using the ``ContentAwareGenerator``, you can generate URLs
for your content in three ways:

* Either pass a ``Route`` object as $name
* Or pass a ``RouteReferrersInterface`` object that is your content as $name
* Or provide an implementation of ``ContentRepositoryInterface`` and pass the id
  of the content object as parameter ``content_id`` and ``null`` as $name.

If you want to implement your own generator for ``$name``s that are not
strings, you need to implement the ``ChainedRouterInterface`` and implement the
``supports($name)`` method to tell the ``ChainRouter`` if your router can
accept this ``$name`` to generate a URL.

Symfony2 integration
--------------------

As mentioned before, this component was designed to use independently of the
Symfony2 framework.  However, if you wish to use it as part of your Symfony
CMF project, an integration bundle is also available. Read more about the
RoutingBundle in :doc:`../bundles/routing/introduction` in the bundles
documentation.

.. _`Install it via Composer`: http://symfony.com/doc/current/components/using_components.html
.. _`Packagist`: https://packagist.org/packages/symfony-cmf/routing
.. _`Routing chapter of the Symfony2 book`: http://symfony.com/doc/current/book/routing.html
.. _`Routing Component`: http://symfony.com/doc/current/components/routing/introduction.html
.. _`Composer`: http://getcomposer.org
.. _`HttpKernel`: http://symfony.com/doc/current/components/http_kernel/introduction.html
.. _`FieldByClassEnhancer`: https://github.com/symfony-cmf/Routing/blob/master/Enhancer/FieldByClassEnhancer.php
.. _`FieldMapEnhancer`: https://github.com/symfony-cmf/Routing/blob/master/Enhancer/FieldMapEnhancer.php
.. _`FieldPresenceEnhancer`: https://github.com/symfony-cmf/Routing/blob/master/Enhancer/FieldPresenceEnhancer.php
.. _`RouteContentEnhancer`: https://github.com/symfony-cmf/Routing/blob/master/Enhancer/RouteContentEnhancer.php
.. _`Event Dispatcher`: http://symfony.com/doc/current/components/event_dispatcher/index.html
.. _`How to create an Event Listener`: http://symfony.com/doc/current/cookbook/service_container/event_listener.html
