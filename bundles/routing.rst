.. index::
    single: Routing; Bundles
    single: RoutingBundle

The RoutingBundle
=================

The `RoutingBundle`_ integrates dynamic routing into Symfony using
:doc:`../components/routing`.

The ``ChainRouter`` is meant to replace the default Symfony Router. All it
does is collect a prioritized list of routers and try to match requests and
generate URLs with all of them. One of the routers in that chain can of course
be the default router so you can still use the standard way for some of your
routes.

Additionally, this bundle delivers useful router implementations. Currently,
there is the ``DynamicRouter`` that routes based on a custom loader logic for
Symfony2 Route objects. The provider can be implemented using a database, for
example with Doctrine `PHPCR-ODM`_ or Doctrine ORM. This bundle provides a
default implementation for Doctrine `PHPCR-ODM`_.

The DynamicRouter service is only made available when explicitly enabled in
the application configuration.

Finally this bundles provides route documents for Doctrine `PHPCR-ODM`_ and a
controller for redirection routes.

.. index:: RoutingBundle
.. index:: Routing

Dependencies
------------

* `Symfony CMF routing`_

ChainRouter
-----------

The ChainRouter can replace the default symfony routing system with a chain-
enabled implementation. It does not route anything on its own, but only loops
through all chained routers. To handle standard configured symfony routes, the
symfony default router can be put into the chain.

Configuration
-------------

In your ``app/config/config.yml``, you can specify which router services you
want to use. If you do not specify the ``routers_by_id`` map at all, by default
the chain router will just load the built-in symfony router. When you specify
the ``routers_by_id`` list, you need to have an entry for ``router.default`` if
you want the Symfony2 router (that reads the routes from
``app/config/routing.yml``).

The format is ``service_name: priority`` - the higher the priority number the
earlier this router service is asked to match a route or to generate a url

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_routing:
            chain:
                routers_by_id:
                    # enable the DynamicRouter with high priority to allow overwriting configured routes with content
                    cmf_routing.dynamic_router: 200
                    # enable the symfony default router with a lower priority
                    router.default: 100
                # whether the chain router should replace the default router. defaults to true
                # if you set this to false, the router is just available as service
                # cmf_routing.router and you  need to do something to trigger it
                # replace_symfony_router: true

Loading Routers with Tagging
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Your routers can automatically register, just add it as a service tagged with
``router`` and an optional ``priority``.  The higher the priority, the earlier
your router will be asked to match the route. If you do not specify the
priority, your router will come last.  If there are several routers with the
same priority, the order between them is undetermined.  The tagged service
will look like this

.. configuration-block::

    .. code-block:: yaml

        services:
            my_namespace.my_router:
                class: %my_namespace.my_router_class%
                tags:
                    - { name: router, priority: 300 }

    .. code-block:: xml

        <service id="my_namespace.my_router" class="%my_namespace.my_router_class%">
            <tag name="router" priority="300" />
            ..
        </service>

See also official Symfony2 `documentation for DependencyInjection tags`_

Dynamic Router
--------------

This implementation of a router uses the NestedMatcher which loads routes from
a RouteProviderInterface. The provider interface can be easily implemented
with Doctrine.

The router works with extended UrlMatcher and UrlGenerator classes that add
loading routes from the database and the concept of referenced content.

The NestedMatcher service is set up with a route provider. See the
configuration section for how to change the route_repository_service and the
following section on more details for the default `PHPCR-ODM`_ based
implementation.

You may want to configure route enhancers to decide what controller is used to
handle the request, to avoid hard coding controller names into your route
documents.

The minimum configuration required to load the dynamic router as service
``cmf_routing.dynamic_router`` is to have ``enabled: true`` in your
config.yml (the router is automatically enabled as soon as you add any other
configuration to the `dynamic` entry). Without enabling it, the dynamic router
service will not be loaded at all, allowing you to use the ChainRouter with
your own routers

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_routing:
            dynamic:
                enabled: true

PHPCR-ODM integration
~~~~~~~~~~~~~~~~~~~~~

This bundle comes with a route repository implementation for `PHPCR-ODM`_.
PHPCR is well suited to the tree nature of the data. If you use `PHPCR-ODM`_
with a route document like the one provided, you can just leave the repository
service at the default.

The default repository loads the route at the path in the request and all
parent paths to allow for some of the path segments being parameters. If you
need a different way to load routes or for example never use parameters, you
can write your own repository implementation to optimize (see
``cmf_routing.xml`` for how to configure the service).

.. index:: PHPCR, ODM

Match Process
~~~~~~~~~~~~~

Most of the match process is described in the documentation of the `CMF
Routing component`_.  The only difference is that the bundle will place the
``contentDocument`` in the request attributes instead of the route defaults.

Your controllers can (and should) declare the parameter $contentDocument in
their ``Action`` methods if they are supposed to work with content referenced
by the routes.  See
``Symfony\Cmf\Bundle\ContentBundle\Controller\ContentController`` for an
example.

.. _bundle-routing-route-enhancer:

Configuration
~~~~~~~~~~~~~

To configure what controller is used for which content, you can specify route
enhancers. Presence of each of any enhancer configuration makes the DI
container inject the respective enhancer into the DynamicRouter.

The possible enhancements are (in order of precedence):

* (Explicit controller): If there is a _controller set in ``getRouteDefaults()``,
  no enhancer will overwrite it.
* Explicit template: requires the route document to return a '_template'
  parameter in getRouteDefaults. The configured generic controller is
  set by the enhancer.
* Controller by alias: requires the route document to return a 'type' value in
  ``getRouteDefaults()``
* Controller by class: requires the route document to return an object for
  ``getRouteContent()``. The content document is checked for being ``instanceof`` the
  class names in the map and if matched that controller is used.
  Instanceof is used instead of direct comparison to work with proxy classes
  and other extending classes.
* Template by class: requires the route document to return an object for
  ``getRouteContent()``. The content document is checked for being ``instanceof`` the
  class names in the map and if matched that template will be set as
  ``'_template'`` in the ``$defaults`` and the generic controller used as controller.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_routing:
            dynamic:
                generic_controller: cmf_content.controller:indexAction
                controllers_by_type:
                    editablestatic: sandbox_main.controller:indexAction
                controllers_by_class:
                    Symfony\Cmf\Bundle\ContentBundle\Document\StaticContent: cmf_content.controller:indexAction
                templates_by_class:
                    Symfony\Cmf\Bundle\ContentBundle\Document\StaticContent: CmfContentBundle:StaticContent:index.html.twig

                # the route provider is responsible for loading routes.
                manager_registry: doctrine_phpcr
                manager_name: default

                # if you use the default doctrine route repository service, you
                # can use this to customize the root path for the `PHPCR-ODM`_
                # RouteProvider. This base path will be injected by the
                # Listener\IdPrefix - but only to routes matching the prefix,
                # to allow for more than one route source.
                routing_repositoryroot: /cms/routes

                # If you want to replace the default route provider or content repository
                # you can specify their service IDs here.
                route_provider_service_id: my_bundle.provider.endpoint
                content_repository_service_id: my_bundle.repository.endpoint

                # an orm provider might need different configuration. look at
                # cmf_routing.xml for an example if you need to define your own
                # service


To see some examples, please look at the `CMF sandbox`_ and specifically the
routing fixtures loading.

.. tip::

    You can also define your own RouteEnhancer classes for specific use cases.
    See :ref:`bundle-routing-customize`.

.. _bundle-routing-document:

Using the PHPCR-ODM route document
----------------------------------

All route classes must extend the Symfony core ``Route`` class. The documents can
either be created by code (for example a fixtures script) or with a web interface
like the one provided for Sonata PHPCR-ODM admin (see below).

PHPCR-ODM maps all features of the core route to the storage, so you can use
setDefault, setRequirement, setOption and setHostnamePattern like normal.
Additionally when creating a route, you can define whether .{_format} should be
appended to the pattern and configure the required _format with a requirements.
The other constructor option lets you control whether the route should append a
trailing slash because this can not be expressed with a PHPCR name. The default
is to have no trailing slash.

All routes are located under a configured root path, for example '/cms/routes'.
A new route can be created in PHP code as follows:

.. code-block:: php

    use Symfony\Cmf\Bundle\RoutingBundle\Document\Route;
    $route = new Route;
    $route->setParent($dm->find(null, '/routes'));
    $route->setName('projects');
    // set explicit controller (both service and Bundle:Name:action syntax work)
    $route->setDefault('_controller', 'sandbox_main.controller:specialAction');

The above example should probably be done as a route configured in a Symfony
xml/yml file however, unless the end user is supposed to change the URL or the
controller.

To link a content to this route, simply set it on the document.

.. code-block:: php

    $content = new Content('my content'); // Content must be a mapped class
    $route->setRouteContent($content);

This will put the document into the request parameters and if your controller
specifies a parameter called ``$contentDocument``, it will be passed this
document.

You can also use variable patterns for the URL and define requirements and
defaults.

.. code-block:: php

    // do not forget leading slash if you want /projects/{id} and not /projects{id}
    $route->setVariablePattern('/{id}');
    $route->setRequirement('id', '\d+');
    $route->setDefault('id', 1);

This will give you a route that matches the URL ``/projects/<number>`` but
also /projects as there is a default for the id parameter. This will match
``/projects/7`` as well as ``/projects`` but not ``/projects/x-4``.  The
document is still stored at ``/routes/projects``. This will work because, as
mentioned above, the route provider will look for route documents at all
possible paths and pick the first that matches. In our example, if there is a
route document at ``/routes/projects/7`` that matches (no further parameters)
it is selected. Otherwise we check if /routes/projects has a pattern that
matches. If not, the top document at /routes is checked.

Of course you can also have several parameters, like with normal Symfony
routes. The semantics and rules for patterns, defaults and requirements are
exactly the same as in core routes.

Your controller can expect the $id parameter as well as the $contentDocument
as we set a content on the route. The content could be used to define an intro
section that is the same for each project or other shared data. If you don't
need content, you can just not set it in the document.

Sonata Admin Configuration
--------------------------

If ``sonata-project/doctrine-phpcr-admin-bundle`` is added to the
composer.json require section and the SonataDoctrinePhpcrAdminBundle is loaded
in the application kernel, the route documents are exposed in the
SonataDoctrinePhpcrAdminBundle.  For instructions on how to configure this
Bundle see :doc:`doctrine_phpcr_admin`.

By default, ``use_sonata_admin`` is automatically set based on whether
``SonataDoctrinePhpcrAdminBundle`` is available, but you can explicitly
disable it to not have it even if sonata is enabled, or explicitly enable to
get an error if Sonata becomes unavailable.

If you want to use the admin, you want to configure the ``content_basepath``
to point to the root of your content documents.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_routing:
            use_sonata_admin: auto # use true/false to force using / not using sonata admin
            content_basepath: ~ # used with sonata admin to manage content, defaults to cmf_core.content_basepath


Form Type
---------

The bundle defines a form type that can be used for classical "accept terms"
checkboxes where you place urls in the label. Simply specify
``cmf_routing_terms_form_type`` as the form type name and specify a
label and an array with ``content_ids`` in the options::

    $form->add('terms', 'cmf_routing_terms_form_type', array(
        'label' => 'I have seen the <a href="%team%">Team</a> and <a href="%more%">More</a> pages ...',
        'content_ids' => array('%team%' => '/cms/content/static/team', '%more%' => '/cms/content/static/more')
    ));

The form type automatically generates the routes for the specified content and
passes the routes to the trans twig helper for replacement in the label.

Further notes
-------------

See the documentation of the `CMF Routing component`_ for information on the
RouteObjectInterface, redirections and locales.

Notes:

* **RouteObjectInterface**: The provided documents implement this interface to
  map content to routes and to (optional) provide a custom route name instead
  of the symfony core compatible route name.
* **Redirections**: This bundle provides a controller to handle redirections.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_routing:
            controllers_by_class:
                Symfony\Cmf\Component\Routing\RedirectRouteInterface:  cmf_routing.redirect_controller:redirectAction

.. _bundle-routing-customize:

Customize
---------

You can add more RouteEnhancerInterface implementations if you have a case not
handled by the provided ones. Simply define services for your enhancers and
tag them with ``dynamic_router_route_enhancer`` to have them added to the
routing.

If you use an ODM / ORM different to `PHPCR-ODM`_, you probably need to
specify the class for the route entity (in `PHPCR-ODM`_, the class is
automatically detected). For more specific needs, have a look at DynamicRouter
and see if you want to extend it. You can also write your own routers to hook
into the chain.

Learn more from the Cookbook
----------------------------

* :doc:`../cookbook/using-a-custom-route-repository`

Further notes
-------------

For more information on the Routing component of Symfony CMF, please refer to:

* :doc:`../getting-started/routing` for an introductory guide on Routing bundle
* :doc:`../components/routing` for most of the actual functionality implementation
* Symfony2's `Routing`_ component page

.. _`RoutingBundle`: https://github.com/symfony-cmf/RoutingBundle#readme
.. _`Symfony CMF routing`: https://github.com/symfony-cmf/Routing#readme
.. _`documentation for DependencyInjection tags`: http://symfony.com/doc/2.1/reference/dic_tags.html
.. _`CMF sandbox`: https://github.com/symfony-cmf/cmf-sandbox
.. _`CMF Routing component`: https://github.com/symfony-cmf/Routing
.. _`PHPCR-ODM`: https://github.com/doctrine/phpcr-odm
.. _`Routing`: http://symfony.com/doc/current/components/routing/introduction.html
