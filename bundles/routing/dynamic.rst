.. index::
    single: Routing; DynamicRouter

Dynamic Router
==============

This implementation of a router is configured to load routes from a
``RouteProviderInterface``. This interface can be easily implemented with
Doctrine for example. See the :ref:`following section <bundle-routing-document>`
for more details about the default `PHPCR-ODM`_ provider and
:ref:`further below <bundle-routing-entity>` for the Doctrine ORM
based implementation. If those do not match your needs, you can
:ref:`build your own route provider <bundle-routing-custom_provider>`.

You can configure the route enhancers that decide what controller is used to
handle the request, to avoid hard coding controller names into your route
documents.

To fully understand the capabilities of the dynamic router, read also the
:doc:`routing component documentation <../../components/routing/index>`.

Configuration
-------------

The minimum configuration required to load the dynamic router is to specify a
route provider backend and to register the dynamic router in the chain of
routers.

.. note::

    When your project is also using the :doc:`CoreBundle <../core/introduction>`,
    it is enough to configure persistence on ``cmf_core`` and you do not need to
    repeat the configuration for the dynamic router.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_routing:
            chain:
                routers_by_id:
                    router.default: 200
                    cmf_routing.dynamic_router: 100
            dynamic:
                persistence:
                    phpcr:
                        enabled: true

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/routing">
                <chain>
                    <router-by-id id="router.default">200</router-by-id>
                    <router-by-id id="cmf_routing.dynamic_router">100</router-by-id>
                </chain>
                <dynamic>
                    <persistence>
                        <phpcr enabled="true" />
                    </persistence>
                </dynamic>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_routing', [
            'chain' => [
                'routers_by_id' => [
                    'router.default' => 200,
                    'cmf_routing.dynamic_router' => 100,
                ],
            ],
            'dynamic' => [
                'persistence' => [
                    'phpcr' => [
                        'enabled' => true,
                    ],
                ],
            ],
        ]);

When there is no configuration or ``cmf_routing.dynamic.enabled`` is set to
``false``, the dynamic router services will not be loaded at all, allowing
you to use the ``ChainRouter`` with your own routers.

.. _bundle-routing-dynamic-match:

Match Process
-------------

Most of the match process is described in the documentation of the
`CMF Routing component`_. The only difference is that this bundle will place
the ``contentDocument`` into the request attributes instead of into the route
defaults to avoid issues when generating the URL for the current request.

Your controllers can (and should) declare the parameter ``$contentDocument`` in
their ``Action`` methods if they are supposed to work with content referenced
by the routes.  Note that the :doc:`../content/introduction` provides a default
controller that renders the content with a specified template for when you do
not need any logic.

A custom controller action can look like this::

    // src/AppBundle/Controller/ContentController.php
    namespace AppBundle\Controller;

    use Symfony\Component\HttpFoundation\Response;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    /**
     * A custom controller to handle a content specified by a route.
     */
    class ContentController extends Controller
    {
        /**
         * @param object $contentDocument the name of this parameter is defined
         *      by the RoutingBundle. You can also expect any route parameters
         *      or $contentTemplate if you configured templates_by_class (see below).
         *
         * @return Response
         */
        public function demoAction($contentDocument)
        {
            // ... do things with $contentDocument and gather other information
            $customValue = 42;

            return $this->render('content/demo.html.twig', [
                'cmfMainContent' => $contentDocument,
                'custom_parameter' => $customValue,
            ]);
        }
    }

.. note::

    The ``DynamicRouter`` fires an event at the start of the matching process, read
    more about this in :ref:`the component documentation <components-routing-events>`.

.. _bundles-routing-dynamic_router-enhancer:

Configuring the Controller for a Route
--------------------------------------

To configure what controller is used for which route, you can configure the
*route enhancers*. Many of them operate on routes implementing
``RouteObjectInterface``. This interface tells that the route knows about its
content and returns it by the method ``getRouteContent()``. (See
`CMF Routing component`_ if you want to know more about this interface.)

The possible enhancements that take place, if configured, are (in order of
precedence):

#. (Explicit controller): If there is a ``_controller`` set in
   ``getRouteDefaults()``, no enhancer will overwrite the controller.
   ``_template`` will still be inserted if its not already set;
#. ``controllers_by_type``: requires the route document to return a 'type' value in
   ``getRouteDefaults()``. **priority: 60**;
#. ``controllers_by_class``: requires the route document to be an instance of
   ``RouteObjectInterface`` and to return an object for ``getRouteContent()``.
   The content document is checked for being ``instanceof`` the class names in
   the map and if matched that controller is used. ``Instanceof`` is used instead of
   direct comparison to work with proxy classes and other extending classes.
   **priority: 50**;
#. ``templates_by_class``: requires the route document to be an instance of
   ``RouteObjectInterface`` and to return an object for ``getRouteContent()``.
   The content document is checked for being ``instanceof`` the class names in
   the map and if matched that template will be set as ``'_template'``.
   **priority: 40** for the template, generic controller is set at
   **priority: 30**;
#. If a ``_template`` is in the ``$defaults`` but no controller was determined
   so far (neither set on the route nor matched in controller by type or class),
   the generic controller is chosen. **priority: 10**;
#. The default controller is chosen. This controller can use a default template
   to render the content, which will likely further decide how to handle this
   content. See also
   :ref:`the content bundle documentation <bundles-content-introduction_default-template>`.
   **priority: -100**.

See :ref:`the configuration reference <reference-config-routing-dynamic>` to
learn how to configure these enhancers.

If the ContentBundle is present in your application, the generic and default
controllers default to the ``ContentController`` provided by that bundle.

.. tip::

    To see some examples, please look at the `CMF sandbox`_ and specifically
    the routing fixtures loading.

.. tip::

    You can also define your own ``RouteEnhancer`` classes for specific use
    cases. See :ref:`bundle-routing-customize`. Use the priority to insert your
    enhancers in the correct order.

.. _bundle-routing-document:

Doctrine PHPCR-ODM Integration
------------------------------

The RoutingBundle comes with a route provider implementation for `PHPCR-ODM`_.
PHPCR is well suited to the tree nature of the data. If you use `PHPCR-ODM`_
with a route document like the one provided, you can just leave the provider
service at the default.

The default provider loads the route at the path in the request and all
parent paths to allow for some of the path segments being parameters. If you
need a different way to load routes or for example never use parameters, you
can write your own provider implementation to optimize by implementing the
``RouteProviderInterface`` with your own service and specify that service
as ``cmf_routing.dynamic.route_provider_service_id``.

.. index:: PHPCR, ODM

The PHPCR-ODM Route document
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

All route classes must extend the Symfony core ``Route`` class. The default
PHPCR-ODM route document also implements the ``RouteObjectInterface`` to link
routes with content. It maps all features of the core route to the storage, so
you can use ``setDefault``, ``setRequirement``, ``setOption`` and
``setHostnamePattern``. Additionally when creating a route, you can define
whether ``.{_format}`` should be appended to the pattern and configure the
required ``_format`` with a requirements. The other constructor argument lets
you control whether the route should append a trailing slash because this can
not be expressed with a PHPCR name. The default is to have no trailing slash.
Both options can also be changed later through setter methods.

All routes are located under a configured root path, for example
``/cms/routes``. A new route can be created in PHP code as follows::

    use Symfony\Cmf\Bundle\RoutingBundle\Doctrine\Phpcr\Route;

    $route = new Route();
    $route->setParentDocument($dm->find(null, '/cms/routes'));
    $route->setName('projects');

    // set explicit controller (both service and Bundle:Name:action syntax work)
    $route->setDefault('_controller', 'app.controller:specialAction');

The above example should probably be done as a route configured in a Symfony
configuration file, unless the end user is supposed to change the URL
or the controller.

To link a content to this route, simply set it on the document::

    use Symfony\Cmf\Bundle\ContentBundle\Doctrine\Phpcr\Content;

    // ...
    $content = new Content('my content'); // Content must be a mapped class
    $route->setRouteContent($content);

This will make the routing put the document into the request parameters and if
your controller specifies a parameter called ``$contentDocument``, it will be
passed this document.

You can also use variable patterns for the URL and define requirements with
``setRequirement`` and defaults with ``setDefault``::

    // do not forget leading slash if you want /projects/{id} and not /projects{id}
    $route->setVariablePattern('/{id}');
    $route->setRequirement('id', '\d+');
    $route->setDefault('id', 1);

This defines a route that matches the URL ``/projects/<number>`` but also
``/projects`` as there is a default for the ``id`` parameter. This will match
``/projects/7`` as well as ``/projects`` but not ``/projects/x-4``. The
document is still stored at ``/routes/projects``. This will work because, as
mentioned above, the route provider will look for route documents at all
possible paths and pick the first that matches. In our example, if there is a
route document at ``/routes/projects/7`` that matches (no further parameters),
it gets chosen. Otherwise, routing checks if ``/routes/projects`` has a pattern
that matches. If not, the top document at ``/routes`` is checked for a matching
pattern.

The semantics and rules for patterns, defaults and requirements are exactly the
same as in core routes. If you have several parameters, or static bits *after*
a parameter, make them part of the variable pattern::

    $route->setVariablePattern('/{context}/item/{id}');
    $route->setRequirement('context', '[a-z]+');
    $route->setRequirement('id', '\d+');

.. note::

    The ``RouteDefaultsValidator`` validates the route defaults parameters.
    For more information, see :ref:`bundle-routing-route-defaults-validator`.

With the above example, your controller can expect both the ``$id`` parameter
as well as the ``$contentDocument`` if you set a content on the route and have
a variable pattern with ``{id}``. The content could be used to define an intro
section that is the same for each id. If you don't need content, you can also
omit setting a content document on the route document.

.. _component-route-generator-and-locales:

.. sidebar:: Locales

    You can use the ``_locale`` default value in a Route to create one Route
    per locale, all referencing the same multilingual content instance. The
    ``ContentAwareGenerator`` respects the ``_locale`` when generating routes
    from content instances. When resolving the route, the ``_locale`` gets
    into the request and is picked up by the Symfony locale system.

    Make sure you configure the valid locales in the configuration so that the
    bundle can optimally handle locales. The
    :ref:`configuration reference <reference-config-routing-locales>` lists
    some options to tweak behavior and performance.

.. note::

    Under PHPCR-ODM, Routes should not be translatable documents, as one
    Route document represents one single url, and serving several translations
    under the same url is not recommended.

    If you need translated URLs, make the ``locale`` part of the route name and
    create one route per language for the same content. The route generator will
    pick the correct route if available.

.. _bundle-routing-entity:

Doctrine ORM integration
------------------------

Alternatively, you can use the `Doctrine ORM`_ provider by specifying the
``persistence.orm`` part of the configuration. It does a similar job but, as
the name indicates, loads ``Route`` entities from an ORM database.

.. caution::

    You must install the CoreBundle to use this feature if your application
    does not have at least DoctrineBundle 1.3.0.

.. _bundles-routing-dynamic-generator:

URL generation with the DynamicRouter
-------------------------------------

Apart from matching an incoming request to a set of parameters, a Router is
also responsible for generating an URL from a route and its parameters. The
``DynamicRouter`` adds more power to the
`URL generating capabilities of Symfony`_.

.. tip::

    All Twig examples below are given with the ``path`` function that generates
    the URL without domain, but will work with the ``url`` function as well.

    Also, you can specify parameters to the generator, which will be used if
    the route contains a dynamic pattern or otherwise will be appended as
    query string, just like with the standard routing.

You can use a ``Route`` object as the name parameter of the generating method.
This will look as follows:

.. configuration-block::

    .. code-block:: html+jinja

        {# myRoute is an object of class Symfony\Component\Routing\Route #}
        <a href="{{ path(myRoute) }}">Read on</a>

    .. code-block:: html+php

        <!-- $myRoute is an object of class Symfony\Component\Routing\Route -->
        <a href="<?php echo $view['router']->generate($myRoute) ?>">
            Read on
        </a>

When using the PHPCR-ODM persistence layer, the repository path of the route
document is considered the route name. Thus you can specify a repository path
to generate a route:

.. configuration-block::

    .. code-block:: html+jinja

        {# Create a link to / on this server #}
        <a href="{{ path('/cms/routes') }}>Home</a>

    .. code-block:: html+php

        <!-- Create a link to / on this server -->
        <a href="<?php echo $view['router']->generate('/cms/routes') ?>">
            Home
        </a>

.. caution::

    It is dangerous to hard-code paths to PHPCR-ODM documents into your
    templates. An admin user could edit or delete them in a way that your
    application breaks. If the route must exist for sure, it probably
    should be a statically configured route. But route names could come from
    code for example.

The ``DynamicRouter`` uses a URL generator that operates on the
``RouteReferrersInterface``. This means you can also generate a route from any
object that implements this interface and provides a route for it:

.. configuration-block::

    .. code-block:: html+jinja

        {# myContent implements RouteReferrersInterface #}
        <a href="{{ path(myContent) }}>Read on</a>

    .. code-block:: html+php

        <!-- $myContent implements RouteReferrersInterface -->
        <a href="<?php echo $view['router']->generate($myContent) ?>">
            Home
        </a>

.. tip::

    If there are several routes for the same content, the one with the locale
    matching the current request locale is preferred

Additionally, the generator also understands the ``content_id`` parameter with
an empty route name and tries to find a content implementing the
``RouteReferrersInterface`` from the configured content repository:

.. configuration-block::

    .. code-block:: html+jinja

        <a href="{{ path(null, {'content_id': '/cms/content/my-content'}) }}>
            Read on
        </a>

    .. code-block:: html+php

        <!-- $myContent implements RouteReferrersInterface -->
        <a href="<?php echo $view['router']->generate(null, [
            'content_id' => '/cms/content/my-content',
        ]) ?>">
            Home
        </a>

.. note::

    To be precise, it is enough for the content to implement the
    ``RouteReferrersReadInterface`` if writing the routes is not desired. See
    :ref:`contributing-bundles-interface_naming` for more on the naming scheme.)

For the implementation details, please refer to the
:ref:`component-routing-generator` section in the the cmf routing component
documentation.

.. sidebar:: Dumping Routes

    The ``RouterInterface`` defines the method ``getRouteCollection`` to get
    all routes available in a router. The ``DynamicRouter`` is able to provide
    such a collection, however this feature is disabled by default to avoid
    dumping large numbers of routes. You can set
    ``cmf_routing.dynamic.route_collection_limit`` to a value bigger than 0
    to have the router return routes up to the limit or ``false`` to disable
    limits and return all routes.

    With this option activated, tools like the ``router:debug`` command or the
    `FOSJsRoutingBundle`_ will also show the routes coming from the database.

    For the case of `FOSJsRoutingBundle`_, if you use the upcoming version 2 of
    the bundle, you can configure ``fos_js_routing.router`` to
    ``router.default`` to avoid the dynamic routes being included.

Handling RedirectRoutes
-----------------------

This bundle also provides a controller to handle ``RedirectionRouteInterface``
documents. You need to configure the route enhancer for this interface:

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
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/routing">
                <dynamic>
                    <controller-by-class class="Symfony\Cmf\Component\Routing\RedirectRouteInterface">
                        cmf_routing.redirect_controller:redirectAction
                    </controller-by-class>
                </dynamic>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Cmf\Bundle\Routing\RedirectRouteInterface;

        $container->loadFromExtension('cmf_routing', [
            'dynamic' => [
                'controllers_by_class' => [
                    RedirectRouteInterface::class => 'cmf_routing.redirect_controller:redirectAction',
                ],
            ],
        ]);

Customize the DynamicRouter
---------------------------

Read on in the chapter :doc:`customizing the dynamic router <dynamic_customize>`.

.. _`CMF sandbox`: https://github.com/symfony-cmf/cmf-sandbox
.. _`CMF Routing component`: https://github.com/symfony-cmf/Routing
.. _`Doctrine ORM`: http://www.doctrine-project.org/projects/orm.html
.. _`PHPCR-ODM`: http://www.doctrine-project.org/projects/phpcr-odm.html
.. _`URL generating capabilities of Symfony`: https://symfony.com/doc/current/book/routing.html#generating-urls
.. _`FOSJsRoutingBundle`: https://github.com/FriendsOfSymfony/FOSJsRoutingBundle
