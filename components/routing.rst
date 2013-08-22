.. index::
    single: Routing; Components
    single: Routing

Routing
=======

The Symfony CMF Routing component extends the Symfony2 core routing
component. Even though it has Symfony in its name, it does not need the full
Symfony2 Framework and can be used in standalone projects.

At the core of the Symfony CMF Routing component is the `ChainRouter`_. The
ChainRouter tries to match a request with each of its registered routers,
ignoring the :class:`Symfony\\Component\\Routing\\Exception\\ResourceNotFoundException`
until all routers got a chance to match. The first match wins - if no router
matched, the :class:`Symfony\\Component\\Routing\\Exception\\ResourceNotFoundException`
is thrown. The default Symfony2 router can be added to this chain, so the
standard routing mechanism can still be used.

Additionally, this component is meant to provide useful implementations of the
routing interfaces. Currently, it provides the
:ref:`DynamicRouter <component-routing-dynamic>`. This flexible router uses
an injected ``RequestMatcherInterface`` instance instead of the hardcoded way
the Symfony2 core router works. ``DynamicRouter`` also can apply
``RouteEnhancerInterface`` strategies in order to post-process the route match
information. The provided `NestedMatcher`_ can dynamically retrieve Symfony2
:class:`Symfony\\Component\\Routing\\Route` objects from a
``RouteProviderInterface``. The ``DynamicRouter`` is also able to generate
Routes. The ``ProviderBasedGenerator`` can generate routes loaded from a
``RouteProviderInterface`` instance, and the ``ContentAwareGenerator`` on top
of it to determine the route object from any content object that implements the
``RouteAwareInterface``.

.. note::

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

    The :doc:`../bundles/routing` provides implementations for everything
    needed to get this component running with Doctrine PHPCR-ODM and soon
    also the Doctrine ORM.

ChainRouter
-----------

At the core of the Symfony CMF Routing component sits the ``ChainRouter``.
It's used as a replacement for Symfony2's default routing system, and is
responsible for determining the parameters for each request. Typically you
need to determine which controller will handle this request - in the full
stack Symfony2 Framework, this is identified by the ``_controller`` field
of the parameters.

The ``ChainRouter`` works by accepting a set of prioritized routing
strategies, :class:`Symfony\\Component\\Routing\\RouterInterface`
implementations, commonly referred to as "Routers".

When handling an incoming request, the ``ChainRouter`` iterates over the
configured routers, sorted by their configured priority, until one of them is
able to :method:`match <Symfony\\Component\\Routing\\RouterInterface::match>`
the request and provide the request parameters.

Routers are added using the ``add`` method of the ``ChainRouter``. To add the
default Symfony2 router::

    use Symfony\Cmf\Component\Routing\ChainRouter;
    use Symfony\Component\Routing\Router;

    $chainRouter = new ChainRouter();
    $chainRouter->add(new Router(...));

Now, when the ``ChainRouter`` matches a request, it'll chain over the Symfony2
``Router`` to see if it matches. If it fails, it'll throw a
:class:`Symfony\\Component\\Routing\\Exception\\ResourceNotFoundException`.

If you add a new router, for instance the ``DynamicRouter``, it will be
chained after the Symfony2 Router (because that was added first). To change
this, you can use the second argument of the ``add`` method to set a priority.
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
    further in this article.

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
This makes it a poor choice to handle dynamically defined routes, and to
handle those situations, this bundle comes with the ``DynamicRouter``. It
is capable of handling Routes from more dynamic data sources, like database storage,
and modify the resulting parameters using a set of enhancers that can be
easily configured, greatly extending Symfony2's default functionality.

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

Events are dispatched during the Dynamic Router match process. Currently there
are 2 events:

* **cmf_routing.pre_dynamic_match** (Dispatched at the beginning of the match
  method)
* **cmf_routing.pre_dynamic_match_request** (Dispatched at the beginning of the
  matchRequest method)

Optionally, you can provide an `Event Dispatcher`_ to the dynamic router.

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
scope of this component. The :doc:`RoutingBundle <../bundles/routing>`
provides a Route Provider for Doctrine PHPCR ODM.

To create and register your own Route Provider, create a class extending
``Symfony\Cmf\Component\Routing\RouteProviderInterface`` and implementing
the methods::

    use Symfony\Cmf\Component\Routing\RouteProviderInterface;

    use Symfony\Component\Routing\RouteCollection;
    use Symfony\Component\Routing\Exception\RouteNotFoundException;

    class DoctrineOrmRouteProvider implements RouteProviderInterface
    {
        // ...

        public function getRouteCollectionForRequest(Request $request)
        {
            // you can also do some very minor filtering here
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

Optionally, and following the matching process, a set of ``RouteEnhancerInterface``
instances can be applied by the ``DynamicRouter``. The aim of these are to
allow you to manipulate the parameters from the matched route. They can be
used, for example, to dynamically assign a controller or template to a
``Route`` or to "upcast" a request parameter to an object. The component
already provides some simple enhancers:

* `FieldByClassEnhancer`_
* `FieldMapEnhancer`_
* `FieldPresenceEnhancer`_
* `RouteContentEnhancer`_

You can also create your own Route Enhancer by creating a class which
implements ``Symfony\Cmf\Component\Routing\Enhancer\RouteEnhancer``. Route
Enhancers are registered using the ``addRouteEnhancer`` method, which has an
optional second argument to provide the priority.

Linking a Route with a Content
..............................

Depending on your application's logic, a requested url may have an associated
content object. A route for such URL may implement the ``RouteObjectInterface``
to return a content object if present. If you configure the
``RouteContentEnhancer``, it will included that content in the match array,
using the ``_content`` key. Notice that a Route can implement the above
mentioned interface but still not to return any model instance, in which case
no associated object will be returned.

Furthermore, routes that implement this interface can also provide a custom
Route name. The key returned by ``getRouteKey`` will be used as route name
instead of the Symfony core compatible route name and can contain any
characters. This allows you, for example, to set a path as the route name. Both
UrlMatchers provided with the ``NestedMatcher`` replace the ``_route`` key
with the route instance and put the provided name into ``_route_name``.

All routes still need to extend the base
:class:`Symfony\\Component\\Routing\\Route <Symfony\\Component\\Routing\\Route>`
class.

Redirections
............

You can build redirections by implementing the ``RedirectRouteInterface``.
It can redirect to an absolute URI, a named Route that can be generated by any
Router in the chain or to another Route object provided by the Route.

Notice that the actual redirection logic is not handled by the bundle. You
should implement your own logic to handle the redirection. For an example of
implementing that redirection under the full Symfony2 stack, refer to
:doc:`the RoutingBundle <../bundles/routing>`.

Generating URLs
~~~~~~~~~~~~~~~

Apart from matching an incoming request to a set of parameters, a Router
is also responsible for generating an URL from a Route and its parameters.
The ``ChainRouter`` iterates over its known routers until one of them is
able to generate a matching URL.

Beside ``RequestMatcherInterface`` and ``UrlMatcherInterface`` to match a
Request/URL to its corresponding parameters, the ``DynamicRouter`` also uses
a ``UrlGeneratorInterface`` instance, which allows it to generate an URL from
a Route.

The included ``ProviderBasedGenerator`` extends Symfony2's default
:class:`Symfony\\Component\\routing\\Generator\\UrlGenerator` (which, in turn,
implements ``UrlGeneratorInterface``) and - if the name is not already a
``Route`` object - loads the Route from the Route provider. It then lets the
core logic generate the URL from that Route.

The component also includes the ``ContentAwareGenerator``, which extends the
``ProviderBasedGenerator`` to check if the name is an object implementing
``RouteReferrersReadInterface`` and, if so, gets the Route from the content.
Using the ``ContentAwareGenerator``, you can generate urls for your content in
three ways:

* Either pass a ``Route`` object as $name
* Or pass a ``RouteAwareInterface`` object that is your content as $name
* Or provide an implementation of ``ContentRepositoryInterface`` and pass the id
  of the content object as parameter ``content_id`` and ``null`` as $name.

Symfony2 integration
--------------------

As mentioned before, this component was designed to use independently of the
Symfony2 framework.  However, if you wish to use it as part of your Symfony
CMF project, an integration bundle is also available. Read more about the
RoutingBundle in ":doc:`../bundles/routing`" in the bundles documentation.

.. _`Install it via Composer`: http://symfony.com/doc/current/components/using_components.html
.. _`Packagist`: https://packagist.org/packages/symfony-cmf/routing
.. _`Routing Component`: http://symfony.com/doc/current/components/routing/introduction.html
.. _`Composer`: http://getcomposer.org
.. _`HttpKernel`: http://symfony.com/doc/current/components/http_kernel/introduction.html
.. _`FieldByClassEnhancer`: https://github.com/symfony-cmf/Routing/blob/master/Enhancer/FieldByClassEnhancer.php
.. _`FieldMapEnhancer`: https://github.com/symfony-cmf/Routing/blob/master/Enhancer/FieldMapEnhancer.php
.. _`FieldPresenceEnhancer`: https://github.com/symfony-cmf/Routing/blob/master/Enhancer/FieldPresenceEnhancer.php
.. _`RouteContentEnhancer`: https://github.com/symfony-cmf/Routing/blob/master/Enhancer/RouteContentEnhancer.php
.. _`Event Dispatcher`: http://symfony.com/doc/current/components/event_dispatcher/index.html
.. _`How to create an Event Listener`: http://symfony.com/doc/current/cookbook/service_container/event_listener.html 
