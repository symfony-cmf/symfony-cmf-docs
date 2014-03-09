.. index::
    single: Routing; Getting Started
    single: CmfRoutingBundle

Routing
=======

This is an introduction to understand the concepts behind CMF routing. For the
reference documentation please see the documentation for the
:doc:`Routing component <../components/routing/introduction>` and the
:doc:`RoutingBundle <../bundles/routing/introduction>`.

Concept
-------

Why a new Routing Mechanism?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

CMS are highly dynamic sites, where most of the content is managed by the
administrators rather than developers. The number of available pages can
easily reach the thousands, which is usually multiplied by the number of
available translations. Best accessibility and SEO practices, as well as user
preferences dictate that the URLs should be definable by the content managers.

The default Symfony2 routing mechanism, with its configuration file approach,
is not the best solution for this problem. It does not provide a way of handling
dynamic, user-defined routes, nor does it scale well to a large number of routes.

The Solution
~~~~~~~~~~~~

In order to address these issues, a new routing system needed to be developed
that takes into account the typical needs of CMS routing:

* User-defined URLs;
* Multi-site;
* Multi-language;
* Tree-like structure for easier management;
* Content, Menu and Route separation for added flexibility.

The Symfony CMF Routing component was created with these requirements in mind.

The ``ChainRouter``
-------------------

At the core of Symfony CMF's Routing component sits the ``ChainRouter``.
It is used as a replacement for Symfony2's default routing system and,
like the Symfony2 router, is responsible for determining which Controller
will handle each request.

The ``ChainRouter`` works by accepting a set of prioritized routing
strategies, :class:`Symfony\\Component\\Routing\\RouterInterface`
implementations, commonly referred to as "Routers". The routers are
responsible for matching an incoming request to an actual Controller and, to
do so, the ``ChainRouter`` iterates over the configured Routers according to
their configured priority:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_routing:
            chain:
                routers_by_id:
                    # enable the DynamicRouter with a low priority
                    # this way the non dynamic routes take precedence
                    # to prevent needless database look ups
                    cmf_routing.dynamic_router: 20

                    # enable the symfony default router with a higher priority
                    router.default: 100

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <container xmlns="http://cmf.symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/routing">
                <chain>
                    <!-- enable the DynamicRouter with a low priority
                         this way the non dynamic routes take precedence
                         to prevent needless database look ups -->
                    <routers-by-id
                        id="cmf_routing.dynamic_router">
                        20
                    </routers-by-id>

                    <!-- enable the symfony default router with a higher priority -->
                    <routers-by-id
                        id="router.default">
                        100
                    </routers-by-id>
                </chain>
            </config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_routing', array(
            'chain' => array(
                'routers_by_id' => array(
                    // enable the DynamicRouter with a low priority
                    // this way the non dynamic routes take precedence
                    // to prevent needless database look ups
                    'cmf_routing.dynamic_router' => 20,

                    // enable the symfony default router with a higher priority
                    'router.default' => 100,
                ),
            ),
        ));

You can also load Routers using tagged services, by using the ``router`` tag
and an optional ``priority``. The higher the priority, the earlier your router
will be asked to match the route. If you do not specify the priority, your
router will come last. If there are several routers with the same priority,
the order between them is undetermined. The tagged service will look like
this:

.. configuration-block::

    .. code-block:: yaml

        services:
            my_namespace.my_router:
                class: "%my_namespace.my_router_class%"
                tags:
                    - { name: router, priority: 300 }

    .. code-block:: xml

        <service id="my_namespace.my_router" class="%my_namespace.my_router_class%">
            <tag name="router" priority="300" />
        </service>

    .. code-block:: php

        $container
            ->register('my_namespace.my_router', '%my_namespace.my_router_class%')
            ->addTag('router', array('priority' => 300))
        ;

The Symfony CMF Routing system adds a new ``DynamicRouter``, which complements
the default ``Router`` found in Symfony2.

The Default Symfony2 Router
---------------------------

Although it replaces the default routing mechanism, Symfony CMF Routing allows
you to keep using the existing system. In fact, the standard Symfony2 routing
is enabled by default, so you can keep using the routes you declared in your
configuration files, or as declared by other bundles.

.. _start-routing-dynamic-router:

The DynamicRouter
-----------------

This Router can dynamically load ``Route`` instances from a dynamic source via
a so called *provider*. In fact it only loads candidate routes. The actual
matching process is exactly the same as with the standard Symfony2 routing
mechanism. However the ``DynamicRouter`` additionally is able to determine
which Controller and Template to use based on the ``Route`` that is matched.

By default the ``DynamicRouter`` is disabled. To activate it, just add the
following to your configuration file:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_routing:
            dynamic:
                enabled: true

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <container xmlns="http://cmf.symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">

            <config xmlns="http://cmf.symfony.com/schema/dic/routing">
                <dynamic enabled="true" />
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_routing', array(
            'dynamic' => array(
                'enabled' => true,
            ),
        ));

This is the minimum configuration required to load the ``DynamicRouter`` as a
service, thus making it capable of performing routing. Actually, when you
browse the default pages that come with the Symfony CMF SE, it is the
``DynamicRouter`` that matches your requests with the Controllers and
Templates.

.. _start-routing-getting-route-object:

Getting the Route Object
~~~~~~~~~~~~~~~~~~~~~~~~

The provider to use can be configured to best suit each implementation's
needs. As part of this bundle, an implementation for `Doctrine ORM`_ and
`PHPCR-ODM`_ is provided. Also, you can easily create your own by simply
implementing the ``RouteProviderInterface``. Providers are responsible
for fetching an ordered subset of candidate routes that could match the
request. For example the default `PHPCR-ODM`_ provider loads the ``Route``
at the path in the request and all parent paths to allow for some of the
path segments being parameters.

For more detailed information on this implementation and how you can customize
or extend it, refer to :doc:`../bundles/routing/introduction`.

The ``DynamicRouter`` is able to match the incoming request to a Route object
from the underlying provider. The details on how this matching process is
carried out can be found in the
:doc:`component documentation <../components/routing/dynamic>`.

.. note::

    To have the route provider find routes, you also need to provide the data
    in your storage. With PHPCR-ODM, this is either done through the admin
    interface (see at the bottom) or with fixtures.

    However, before we can explain how to do that, you need to understand how
    the ``DynamicRouter`` works. An example will come
    :ref:`later in this document <start-routing-document>`.

.. _start-routing-getting-controller-template:

Getting the Controller and Template
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A Route needs to specify which Controller should handle a specific Request.
The ``DynamicRouter`` uses one of several possible methods to determine it (in
order of precedence):

* Explicit: The ``Route`` document itself can explicitly declare the target
  Controller if one is returned from ``getDefault('_controller')``.
* By type: The ``Route`` document returns a value from ``getDefault('type')``,
  which is then matched against the provided configuration from config.yml
* By class: Requires the ``Route`` document to implement ``RouteObjectInterface``
  and return an object for ``getContent()``. The returned class type is
  then matched against the provided configuration from config.yml.
* Default: If configured, a default Controller will be used.

Apart from this, the ``DynamicRouter`` is also capable of dynamically
specifying which Template will be used, in a similar way to the one used to
determine the Controller (in order of precedence):

* Explicit: The stored Route document itself can explicitly declare the target
  Template by returning the name of the template via ``getDefault('_template')``.
* By class: Requires the Route instance to implement ``RouteObjectInterface``
  and return an object for ``getContent()``. The returned class type is
  then matched against the provided configuration from config.yml.

Here's an example of how to configure the above mentioned options:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_routing:
            dynamic:
                generic_controller: cmf_content.controller:indexAction
                controllers_by_type:
                    editable_static: sandbox_main.controller:indexAction
                controllers_by_class:
                    Symfony\Cmf\Bundle\ContentBundle\Document\StaticContent: cmf_content.controller::indexAction
                templates_by_class:
                    Symfony\Cmf\Bundle\ContentBundle\Document\StaticContent: CmfContentBundle:StaticContent:index.html.twig

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <container xmlns="http://cmf.symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">

            <config xmlns="http://cmf.symfony.com/schema/dic/routing">
                <dynamic generic-controller="cmf_content.controller:indexAction">
                    <controllers-by-type type="editable_static">
                        sandbox_main.controller:indexAction
                    </controllers-by-type>

                    <controllers-by-class
                        class="Symfony\Cmf\Bundle\ContentBundle\Document\StaticContent"
                    >
                        cmf_content.controller::indexAction
                    </controllers-by-class>

                    <templates-by-class class="Symfony\Cmf\Bundle\ContentBundle\Document\StaticContent"
                    >
                        CmfContentBundle:StaticContent:index.html.twig
                    </templates-by-class>
                </dynamic>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_routing', array(
            'dynamic' => array(
                'generic_controller' => 'cmf_content.controller:indexAction',
                'controllers_by_type' => array(
                    'editable_static' => 'sandbox_main.controller:indexAction',
                ),
                'controllers_by_class' => array(
                    'Symfony\Cmf\Bundle\ContentBundle\Document\StaticContent' => 'cmf_content.controller::indexAction',
                ),
                'templates_by_class' => array(
                    'Symfony\Cmf\Bundle\ContentBundle\Document\StaticContent' => 'CmfContentBundle:StaticContent:index.html.twig',
                ),
            ),
        ));

Notice that ``enabled: true`` is no longer present. It's only required if no
other configuration parameter is provided. The router is automatically enabled
as soon as you add any other configuration to the ``dynamic`` entry.

.. note::

    Internally, the routing component maps these configuration options to
    several ``RouteEnhancerInterface`` instances. The actual scope of these
    enhancers is much wider, and you can find more information about them in
    the :ref:`routing enhancers <component-routing-enhancers>` documentation
    section.

.. _start-routing-linking-a-route-with-a-model-instance:

Linking a Route with a Model Instance
-------------------------------------

Depending on your application's logic, a requested URL may have an associated
model instance from the database. Those Routes can implement the
``RouteObjectInterface``, and optionally return a model instance, that will be
automatically passed to the Controller as the ``contentDocument`` method parameter.

Note that a Route can implement the above mentioned interface but still not
return any model instance, in which case no associated object will be provided.

Furthermore, Routes that implement this interface can also have a custom Route
name, instead of the default Symfony core compatible name, and can contain
any characters. This allows you, for example, to set a path as the route name.

Redirects
---------

You can build redirects by implementing the ``RedirectRouteInterface``. If
you are using the default ``PHPCR-ODM`` route provider, a ready to use
implementation is provided in the ``RedirectRoute`` Document. It can redirect
either to an absolute URI, to a named Route that can be generated by any
Router in the chain or to another Route object known to the route provider.
The actual redirection is handled by a specific Controller that can be
configured as follows:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_routing:
            dynamic:
                controllers_by_class:
                    Symfony\Cmf\Component\Routing\RedirectRouteInterface: cmf_routing.redirect_controller:redirectAction

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <container xmlns="http://cmf.symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">

            <config xmlns="http://cmf.symfony.com/schema/dic/routing">
                <dynamic>
                    <controllers-by-class
                        class="Symfony\Cmf\Component\Routing\RedirectRouteInterface">
                        cmf_routing.redirect_controller:redirectAction
                    </controllers-by-class>
                </dynamic>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_routing', array(
            'dynamic' => array(
                'controllers_by_class' => array(
                    'Symfony\Cmf\Component\Routing\RedirectRouteInterface' => 'cmf_routing.redirect_controller:redirectAction',
                ),
            ),
        ));

.. note::

    The actual configuration for this association exists as a service, not as
    part of a ``config.yml`` file. As discussed before, any of the
    approaches can be used.

URL Generation
--------------

Symfony CMF's Routing component uses the default Symfony2 components to handle
route generation, so you can use the default methods for generating your URLs
with a few added possibilities:

* Pass an implementation of either ``RouteObjectInterface`` or
  ``RouteReferrersInterface`` as the ``name`` parameter
* Alternatively, supply an implementation of ``ContentRepositoryInterface`` and
  the id of the model instance as parameter ``content_id``

See :ref:`bundles-routing-dynamic-generator` for code examples of all above
cases.

The route generation handles locales as well, see
":ref:`ContentAwareGenerator and Locales <component-route-generator-and-locales>`".

.. _start-routing-document:

The PHPCR-ODM Route Document
----------------------------

As mentioned above, you can use any route provider. The example in this
section applies if you use the default PHPCR-ODM route provider
(``Symfony\Cmf\Bundle\RoutingBundle\Doctrine\Phpcr\RouteProvider``).

All routes are located under the base path configured in the application
configuration ``cmf_routing.persistence.phpcr.route_basepath``. By default
this path is ``/cms/routes``. A new route can be created in PHP code as
follows::

    // src/Acme/MainBundle/DataFixtures/PHPCR/LoadRoutingData.php
    namespace Acme\DemoBundle\DataFixtures\PHPCR;

    use Doctrine\ODM\PHPCR\DocumentManager;
    use Symfony\Cmf\Bundle\RoutingBundle\Doctrine\Phpcr\Route;
    use Symfony\Cmf\Bundle\ContentBundle\Doctrine\Phpcr\StaticContent;

    class LoadRoutingData implements FixtureInterface
    {
        /**
         * @param DocumentManager $dm
         */
        public function load(ObjectManager $dm)
        {
            $route = new Route();
            $route->setParent($dm->find(null, '/cms/routes'));
            $route->setName('projects');

            // link a content to the route
            $content = new StaticContent();
            $content->setParent($dm->find(null, '/cms/content'));
            $content->setName('my-content');
            $dm->persist($content);
            $route->setRouteContent($content);

            // now define an id parameter; do not forget the leading slash if you
            // want /projects/{id} and not /projects{id}
            $route->setVariablePattern('/{id}');
            $route->setRequirement('id', '\d+');
            $route->setDefault('id', 1);

            $dm->persist($route),
            $dm->flush();
        }
    }

This will give you a document that matches the URL ``/projects/<number>`` but
also ``/projects`` as there is a default for the id parameter.

Because you defined the ``{id}`` route parameter, your controller can expect an
``$id`` parameter. Additionally, because you called setRouteContent on the
route, your controller can expect the ``$contentDocument`` parameter.
The content could be used to define an intro section that is the same for each
project or other shared data. If you don't need content, you can just not set it
in the document.

For more details, see the
:ref:`route document section in the RoutingBundle documentation <bundle-routing-document>`.

Further Notes
-------------

For more information on the Routing component of Symfony CMF, please refer to:

* :doc:`../components/routing/introduction` for most of the actual functionality implementation
* :doc:`../bundles/routing/introduction` for Symfony2 integration bundle for Routing Bundle
* Symfony2's `Routing`_ component page
* :doc:`../cookbook/handling_multilang_documents` for some notes on multilingual routing

.. _`Doctrine ORM`: http://www.doctrine-project.org/projects/orm.html
.. _`PHPCR-ODM`: http://www.doctrine-project.org/projects/phpcr-odm.html
.. _`Routing`: http://symfony.com/doc/current/components/routing/introduction.html
