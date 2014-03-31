.. index::
    single: NestedMatcher; Routing

NestedMatcher
=============

The provided ``RequestMatcherInterface`` implementation is the
``NestedMatcher``. It is suitable to use with :doc:`DynamicRouter <dynamic>`,
and it uses a multiple step matching process to determine the resulting routing
parameters from a given :class:`Symfony\\Component\\HttpFoundation\\Request`.

The ``NestedMatcher`` uses a 3-step matching process to determine which Route
to use when handling the current Request:

#. Ask the ``RouteProviderInterface`` for the collection of ``Route`` instances
   potentially matching the ``Request``;
#. Apply all ``RouteFilterInterface`` to filter down this collection;
#. Let the ``FinalMatcherInterface`` instance decide on the best match among
   the remaining ``Route`` instances and transform it into the parameter array.

1. The RouteProvider
--------------------

Although the ``RouteProviderInterface`` can be used in other ways, it's main
goal is to be easily implemented on top of Doctrine PHPCR ODM or any other
database, effectively allowing you to store and manage routes dynamically from
the database.

Based on the ``Request``, the ``NestedMatcher`` will retrieve an ordered
collection of ``Route`` objects from the Route Provider. The idea of this
provider is to provide all routes that could potentially match, but **not** to
do any elaborate matching operations yet - this will be done in the later steps.

.. tip::

    The :doc:`RoutingBundle <../../bundles/routing/introduction>` provides
    implementations for everything needed to get this component running with
    Doctrine PHPCR-ODM as well as with the Doctrine ORM.

.. tip::

    This component provides the ``Candidates`` implementation for the very
    first step of splitting the URL on the ``/`` to allow matching with
    variable patterns.

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
--------------------

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
--------------------

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
