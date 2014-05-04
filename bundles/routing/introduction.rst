.. index::
    single: Routing; Bundles
    single: RoutingBundle

RoutingBundle
=============

    The `RoutingBundle`_ integrates dynamic routing into Symfony using the CMF
    Routing component . See the
    :doc:`component documentation <../../components/routing/introduction>`
    if you are interested in the implementation details of the services
    explained in this chapter.

The ``ChainRouter`` is meant to replace the default Symfony Router. All it
does is manage a prioritized list of routers and try to match requests and
generate URLs with all of them. One of the routers in that chain can of course
be the default router so you can still use the Symfony2 standard way of
specifying routes where it makes sense.

Additionally, this bundle delivers useful router implementations. It provides
the ``DynamicRouter`` that routes based on a custom loader logic for Symfony2
Route objects. The provider can be implemented using a database. This bundle
provides default implementations for Doctrine PHPCR-ODM and Doctrine ORM.

The DynamicRouter service is only made available when explicitly enabled in
the application configuration.

Finally this bundles provides route documents for Doctrine PHPCR-ODM and
ORM, as well as a controller for redirection routes.

Installation
------------

You can install this bundle `with composer`_ using the
`symfony-cmf/routing-bundle`_ package.

.. index:: RoutingBundle
.. index:: Routing

ChainRouter
-----------

The ChainRouter can replace the default symfony routing system with a chain-
enabled implementation. It does not route anything on its own, but only loops
through all chained routers. To handle standard configured symfony routes, the
symfony default router can be put into the chain.

You can configure the routing services to use in the chain, see
:ref:`reference-config-routing-chain_routers`.

.. _routing-chain-router-tag:

Loading Routers with Tagging
----------------------------

You can use the service tag ``router`` to automatically register your routers.
The tag has an optional ``priority`` attribute. The higher the priority, the
earlier your router will be asked to match the route. If you do not specify the
priority, your router will come last.  If there are several routers with the
same priority, the order between them is undetermined. The tagged service
will look like this

.. configuration-block::

    .. code-block:: yaml

        services:
            acme_core.my_router:
                class: "%my_namespace.my_router_class%"
                tags:
                    - { name: router, priority: 300 }

    .. code-block:: xml

        <service id="acme_core.my_router" class="%my_namespace.my_router_class%">
            <tag name="router" priority="300" />
            <!-- ... -->
        </service>

    .. code-block:: php

        $container
            ->register('acme_core.my_router', '%acme_core.my_router')
            ->addTag('router', array('priority' => 300))
        ;

See also official Symfony2 `documentation for DependencyInjection tags`_

Sections
--------

* :doc:`dynamic`
* :doc:`dynamic_customize`

Further reading
---------------

For more information on Routing in the Symfony CMF, please refer to:

* The documentation of the :doc:`dynamic`.
* The :doc:`routing introduction chapter <../../book/routing>` of the book.
* The :doc:`routing component documentation <../../components/routing/introduction>`
  for implementation details of the routers.
* Symfony2's `Routing`_ component documentation.

.. _`with composer`: http://getcomposer.org
.. _`symfony-cmf/routing-bundle`: https://packagist.org/packages/symfony-cmf/routing-bundle
.. _`RoutingBundle`: https://github.com/symfony-cmf/RoutingBundle#readme
.. _`PHPCR-ODM`: http://www.doctrine-project.org/projects/phpcr-odm.html
.. _`documentation for DependencyInjection tags`: http://symfony.com/doc/2.1/reference/dic_tags.html
.. _`Routing`: http://symfony.com/doc/current/components/routing/introduction.html
