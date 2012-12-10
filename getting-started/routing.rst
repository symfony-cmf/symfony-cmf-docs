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
to determine who should handle each request. The default Symfony2 router
can be added to this chain, so the standard routing mechanism can still be
used.

Additionally, this component is meant to provide useful ``RouterInterface``
implementations. Currently, it provides ``DynamicRouter`` that's able to
dynamically retrieve a Symfony2 Route object from a repository. The repository
can be implemented using a database, for example with Doctrine PHPCR-ODM
or Doctrine ORM.

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

For the DynamicRouter you will need something to implement the
RouteRepositoryInterface with. We suggest using Doctrine as this allows to map
any class into a database.

ChainRouter
-----------

At the core of Symfony CMF's Routing component sits the ``ChainRouter``.
It's used as a replacement for Symfony2's default routing system, and is
responsible for determining which Controller will handle each request.

The ``ChainRouter`` works by accepting a set of prioritized routing strategies,
`RouterInterface <http://api.symfony.com/2.1/Symfony/Component/Routing/RouterInterface.html>`_
implementations, commonly referred to as "Routers".

When handling an incoming request, it will iterate over the configured Routers,
by their configured priority, until one of them is able to `match <http://api.symfony.com/2.1/Symfony/Component/Routing/RouterInterface.html#method_match>`_
the request and assign the request to a given Controller.

Routers
-------

The ``ChainRouter`` is incapable of, by itself, making any actual routing.
It's sole responsibility is managing the given set of Routers, which are the
true responsibles for the matching a request to a Controller.

You can easily create your own Routers by implementing `RouterInterface <http://api.symfony.com/2.1/Symfony/Component/Routing/RouterInterface.html>`_
but, by default, Symfony CMF already includes some basic routing mechanism to
help you get started.

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

The DynamicRouter is capable of loading `Route <http://api.symfony.com/2.1/Symfony/Component/Routing/Route.html>`_
objects dynamically from a given data source provided to the Router as a
``RouteRepositoryInterface`` implementation. Although it can be used in other
ways, the ``RouteRepositoryInterface``'s main goal is to be easily implementented
on top of Doctrine PHPCR ODM, effectively allowing you to store and manage
routes dynamically from database.

Request handling
^^^^^^^^^^^^^^^^

The incoming request are handled by the ``DynamicRouter``'s  ``match``
using the following workflow:

* The ``DynamicRouter`` uses the ``RouteRepositoryInterface`` to fetch a set of ``Route`` instances
* The ``UrlMatcher`` is then used on the returned set to find a single matching ``Route``
* A set of optional ``RouteEnhancers`` is applied to the ``Route``

The Fetching
""""""""""""

Based on the requested url, the ``DynamicRouter`` will retrieve a ordered
set of ``Route`` objects from the ``RouteRepositoryInterface``.

The underlying implementation of the ``RouteRepositoryInterface`` interface
is not part of this bundle's scope. Refer to the interface declaration for
more information. For a functional example, see `RoutingExtraBundle <https://github.com/symfony-cmf/RoutingExtraBundle>`_.

The Matching
""""""""""""

The DynamicRouter uses Symfony Routing Component's `UrlMatcher <http://api.symfony.com/2.1/Symfony/Component/Routing/Matcher/UrlMatcher.html>`_ 
when iterating over the retrieved ``Route`` objects, in order to determine
which of them to use.

The Enhancers
"""""""""""""

Optionally, a set of ``RouteEnhancerInterface`` instances can be declared and
associated with the DynamicRouter. The aim of these is to allow you to manipulate
the ``Route`` and its associated parameters. They can be used, for example,
to dynamically assign a controller or template to a ``Route``. Some simple
Enhancers are already packed with the bundle, documentation can be found
inside each class file. 

Linking a Route with a Document
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Depending on you application's logic, a requested url may have an associated
Document from the database. Those Routes should implement the ``RouteObjectInterface``,
and can optionally return a Document object, that will be bundled in the
returned array, in the ``_content`` key. Notice that a Route can implement
the above mentioned interface but still not to return any object, in which
case no associated Document will be returned.

Furthermore, routes that implement this interface can also provide a custom Route
name. The key returned by ``getRouteKey`` will be used as route name instead of 
the Symfony core compatible route name and can contain any characters. This allows
you, for example, to set a path as the route name.

All routes still need to extend the base class ``Symfony\Component\Routing\Route``

Redirections
^^^^^^^^^^^^

You can build redirections by implementing the ``RedirectRouteInterface``.
It can redirect either to an absolute URI, to a named Route that can be
generated by any Router in the chain or to another Route object in the repository.

Notice that the actual redirection logic is not handled by the bundle. You
should implement your own logic to handle the actual redirection. For an
example on implementing that redirection under the full Symfony2 stack, refer
to :doc:`../bundles/routing-extra`.

Routes and locales
^^^^^^^^^^^^^^^^^^

You can use the ``_locale`` default value in a Route to create one Route
per locale that, all referencing the same multilingual content. The ``DynamicRouter``
respects the ``_locale`` when generating routes from content. When resolving
the route, the _locale gets into the request and is picked up by the Symfony2
locale system.

.. note::

    Under PHPCR-ODM, Routes should never be translatable documents, as one
    Route document represents one single url, and serving several translations
    under the same url is not recommended.


Url generation
^^^^^^^^^^^^^^

``DynamicRouter`` uses Symfony2's default `UrlGenerator <http://api.symfony.com/2.1/Symfony/Component/Routing/Generator/UrlGenerator.html>`_
to handle url generation. You can generate urls for your content in three ways:

* Either pass an implementation of ``RouteObjectInterface`` as ``route`` parameter 
* Or pass a content object as ``content`` parameter 
* Or supply an implementation of ``ContentRepositoryInterface`` and the id of the content as parameter ``content_id``

Customization
-------------

The Routing bundles allows for several cutomization options, depending on
your specific needs:

* Your Route parameters can be easily manipulated using the exiting Enhancers
* Yuo can also add your own Enhancers to the Router logic.
* The ``DynamicRouter`` or its components can be extended to allow modifications
* You can implement your own Routers and add them to the ``ChainRouter``

.. note::

    If you feel like your specific Enhancer or Router can be usefull to others,
    get in touch with us and we'll try to include it in the bundle itself

Symfony2 integration
--------------------

Like mentioned before, this bundle was designed to only require certain parts
of Symfony2. However, if you wish to use it as part of your Symfony CMF project,
an integration bundle is also available. We strongly recommend that you take
a look at :doc:`../bundles/routing-extra`.

Authors
-------

* Filippo De Santis (p16)
* Henrik Bjornskov (henrikbjorn)
* Claudio Beatrice (omissis)
* Lukas Kahwe Smith (lsmith77)
* David Buchmann (dbu)
* `And others <https://github.com/symfony-cmf/Routing/contributors>`_

The original code for the chain router was contributed by Magnus Nordlander.
