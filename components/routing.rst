Routing Component
=================

The `Symfony CMF Routing component <https://github.com/symfony-cmf/Routing>`_
library extends the Symfony2 core routing component. Even though it has Symfony
in its name, it does not need the full Symfony2 framework and can be used in
standalone projects. For integration with Symfony we provide
:doc:`../bundles/routing-extra`.

At the core of the Symfony CMF Routing component is the ``ChainRouter``, that
is used instead of the Symfony2's default routing system. The ChainRouter
can chain several ``RouterInterface`` implementations, one after the other,
to determine what should handle each request. The default Symfony2 router
can be added to this chain, so the standard routing mechanism can still be
used.

Additionally, this component is meant to provide useful implementations of the
routing interfaces. Currently, it provides the ``DynamicRouter``, which can use
a configured matcher and generator and apply ``RouteEnhancerInterface``
strategies to the match. The ``NestedMatcher`` can dynamically retrieve
Symfony2 Route objects from a route provider. The provider can be implemented
using a database, for example with Doctrine PHPCR-ODM or Doctrine ORM.
The ``ProviderBasedGenerator`` can generate routes loaded from a route provider
and the ``ContentAwareGenerator`` on top of that tries to determine the route
object from a content object.

.. note::

    To use this component outside of the Symfony2 framework context, have
    a look at the core Symfony2 `Routing <https://github.com/symfony/Routing>`_
    to get a fundamental understanding of the component. CMF Routing just extends
    the basic behaviour.

.. index:: Routing

Dependencies
------------

This component uses `composer <http://getcomposer.org>`_. It needs the
Symfony2 Routing component and the Symfony2 HttpKernel (for the logger
interface and cache warmup interface).

For the ``DynamicRouter`` you will need something to implement the
``RouteProviderInterface`` with. We suggest using Doctrine as this provides an
easy way to map classes into a database.

ChainRouter
-----------

At the core of Symfony CMF's Routing component sits the ``ChainRouter``.
It's used as a replacement for Symfony2's default routing system, and is
responsible for determining the parameters for each request. Typically you
need to determine which Controller will handle this request - in the full
stack Symfony2 framework, this is identified by the ``_controller`` field
of the parameters.

The ``ChainRouter`` works by accepting a set of prioritized routing strategies,
`RouterInterface <http://api.symfony.com/2.1/Symfony/Component/Routing/RouterInterface.html>`_
implementations, commonly referred to as "Routers".

When handling an incoming request, the ChainRouter iterates over the configured Routers,
by their configured priority, until one of them is able to `match <http://api.symfony.com/2.1/Symfony/Component/Routing/RouterInterface.html#method_match>`_
the request and provide the request parameters.

Routers
-------

The ``ChainRouter`` is incapable of, by itself, making any actual routing decisions.
It's sole responsibility is managing the given set of Routers, which are the
true responsibles for matching a request and determining its parameters.

You can easily create your own Routers by implementing `RouterInterface <http://api.symfony.com/2.1/Symfony/Component/Routing/RouterInterface.html>`_
but Symfony CMF already includes a powerful route matching system that you can
extend to your needs.

.. note::

    If you are using this as part of a full Symfony CMF project, please refer to
    :doc:`../bundles/routing-extra` for instructions on how to add Routers to
    the ``ChainRouter``. Otherwise, use the ``ChainRouter``'s ``add`` method to
    configure new Routers.

Symfony2 Default Router
~~~~~~~~~~~~~~~~~~~~~~~

The Symfony2 routing mechanism is itself a ``RouterInterface`` implementation,
which means you can use it as a Router in the ``ChainRouter``. This allows you
to use the default routing declaration system.

Dynamic Router
~~~~~~~~~~~~~~

The Symfony2 default router makes many assumptions that are optimized
towards the configured routing files of Symfony2. This component provides the
``DynamicRouter`` which accepts a ``RequestMatcherInterface`` or
``UrlMatcherInterface`` instance in the constructor instead of creating those
objects on the fly. The matcher is responsible of doing the actual match
from request to a parameters array.

Its other feature are the ``RouteEnhancerInterface`` strategies used to infer
routing parameters from the information provided by the match (see below).

Nested Matcher
^^^^^^^^^^^^^^

This matcher uses a multiple step matching process, and is suitable for use with
the DynamicRouter. The *initial* ``RouteProviderInterface`` is capable of
loading candidate `Route <http://api.symfony.com/2.1/Symfony/Component/Routing/Route.html>`_
objects for a request dynamically from its data source. Although it can be used
in other ways, the ``RouteProviderInterface``'s main goal is to be easily
implementented on top of Doctrine PHPCR ODM or a relational database,
effectively allowing you to store and manage routes dynamically from database.

*Intermediate* ``RouteFilterInterface`` implementations can filter the set of
potential route matches.

The *final* decision for the match is taken by the ``FinalMatcherInterface``.
The core Symfony2 UrlMatcher is a good example of a final matcher.

Request handling
""""""""""""""""

Assuming you use the ``DynamicRouter`` together with the ``NestedMatcher``, incoming
requests are handled by the ``DynamicRouter``'s  ``match``:

* The configured matcher's match method is called. In case of the ``NestedMatcher``, it does:

  * Ask the ``RouteProviderInterface`` for the collection of ``Route`` instances potentially matching the ``Request``
  * Apply all ``RouteFilterInterface`` to filter down this collection
  * Let the ``FinalMatcherInterface`` instance decide on the best match among the remaining ``Route`` instances and transform it into the parameter array.
  
* The ``DynamicRouter`` applies all ``RouteEnhancerInterface`` to the matched parameters

RouteProviderInterface
""""""""""""""""""""""

Based on the ``Request``, the ``NestedMatcher`` will retrieve an ordered
collection of ``Route`` objects from the ``RouteProviderInterface``. The idea
of this provider is to provide all routes that could potentially match, but
**not** to do any elaborate matching operations yet - this is the job of the
later steps.

The underlying implementation of the ``RouteProviderInterface`` is not in the
scope of this bundle. Please refer to the interface declaration for more
information. For a functional example, see `RoutingExtraBundle <https://github.com/symfony-cmf/RoutingExtraBundle>`_.

Matching
""""""""

The ``NestedMatcher`` can apply user provided ``RouteFilterInterface`` implementations
to reduce the provided ``Route`` objects, e.g. for doing content negotiation.
It is the responsibility of each filter to throw the ``ResourceNotFoundException`` if
no more routes are left in the collection.

The final matcher has to determine exactly one route as the best match or throw
an exception if nothing matches the request. The default implementation uses the
`UrlMatcher <http://api.symfony.com/2.1/Symfony/Component/Routing/Matcher/UrlMatcher.html>`_
of the Symfony Routing Component.

Enhancing the route
"""""""""""""""""""

Optionally, a set of ``RouteEnhancerInterface`` instances can be declared and
associated with the DynamicRouter. The aim of these is to allow you to
manipulate the parameters from the matched route. They can be used, for
example, to dynamically assign a controller or template to a ``Route`` or to
"upcast" a request parameter to an object. Some simple Enhancers are already
packed with the bundle, documentation can be found inside each class file.

Linking a Route with a Content
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Depending on your application's logic, a requested url may have an associated
content from the database. Those Routes should implement the
``RouteObjectInterface``, and can optionally return a model instance. If you
configure the ``RouteContentEnhancer``, it will included that content in the
match array, with the ``_content`` key. Notice that a Route can implement
the above mentioned interface but still not to return any model instance,
in which case no associated object will be returned.

Furthermore, routes that implement this interface can also provide a custom
Route name. The key returned by ``getRouteKey`` will be used as route name
instead of the Symfony core compatible route name and can contain any
characters. This allows you, for example, to set a path as the route name. Both
UrlMatchers provided with the NestedMatcher replace the _route key with the
route instance and put the provided name into _route_name.

All routes still need to extend the base class ``Symfony\Component\Routing\Route``.

Redirections
^^^^^^^^^^^^

You can build redirections by implementing the ``RedirectRouteInterface``.
It can redirect either to an absolute URI, to a named Route that can be
generated by any Router in the chain or to another Route object provided by the
Route.

Notice that the actual redirection logic is not handled by the bundle. You
should implement your own logic to handle the redirection. For an example on
implementing that redirection under the full Symfony2 stack, refer to
:doc:`../bundles/routing-extra`.


Generating URLs
---------------

A router is also an ``UrlGeneratorInterface`` object, which allows it to
generate an URL from a route. The ``ChainRouter`` just asks each of its routers
to generate the URL until it finds one that can generate the URL.

The ``DynamicRouter`` is configured with a generator instance. The CMF routing
component provides some generators that can handle more than Symfony2 route
names to generate an URL. The ``ProviderBasedGenerator`` extends Symfony2's
default `UrlGenerator <http://api.symfony.com/2.1/Symfony/Component/Routing/Generator/UrlGenerator.html>`_
and - if $name is not already a ``Route`` object, loads the route from
the ``RouteProviderInterface``, then lets the core logic generate the URL from
that route. The ``ContentAwareGenerator`` extends the ProviderBasedGenerator to
check if $name is an object implementing ``RouteAwareInterface`` and if so gets
the route from the content.

Using the ContentAwareGenerator, you can generate urls for your content in
three ways:

* Either pass a ``Route`` object as $name
* Or pass a ``RouteAwareInterface`` object that is your content as $name
* Or provide an implementation of ``ContentRepositoryInterface`` and pass the id
  of the content object as parameter ``content_id`` and ``null`` as $name.


ContentAwareGenerator and locales
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

You can use the ``_locale`` default value in a Route to create one Route
per locale, all referencing the same multilingual content instance. The ``ContentAwareGenerator``
respects the ``_locale`` when generating routes from content instances. When resolving
the route, the ``_locale`` gets into the request and is picked up by the Symfony2
locale system.

.. note::

    Under PHPCR-ODM, Routes should never be translatable documents, as one
    Route document represents one single url, and serving several translations
    under the same url is not recommended.

    If you need translated URLs, make the locale part of the route name.


Customization
-------------

The Routing bundles allows for several customization options, depending on
your specific needs:

* You can implement your own RouteProvider to load routes from a different source
* Your Route parameters can be easily manipulated using the existing Enhancers
* You can also add your own Enhancers to the DynamicRouter
* You can add RouteFilterInterface instances to the NestedMatcher
* The ``DynamicRouter`` or its components can be extended to allow modifications
* You can implement your own Routers and add them to the ``ChainRouter``

.. note::

    If you feel like your specific Enhancer or Router can be useful to others,
    get in touch with us and we'll try to include it in the bundle itself

Symfony2 integration
--------------------

Like mentioned before, this bundle was designed to only require certain parts
of Symfony2. However, if you wish to use it as part of your Symfony CMF project,
an integration bundle is also available. We strongly recommend that you take
a look at :doc:`../bundles/routing-extra`.

For a starter's guide to the Routing bundle and its integration with Symfony2,
refer to :doc:`../getting-started/routing` 

We strongly recommend reading Symfony2's `Routing <http://symfony.com/doc/current/components/routing/introduction.html>`_
component documentation page, as it's the base of this bundle's implementation.

Authors
-------

* Filippo De Santis (p16)
* Henrik Bjornskov (henrikbjorn)
* Claudio Beatrice (omissis)
* Lukas Kahwe Smith (lsmith77)
* David Buchmann (dbu)
* Larry Garfield (Crell)
* `And others <https://github.com/symfony-cmf/Routing/contributors>`_

The original code for the chain router was contributed by Magnus Nordlander.
