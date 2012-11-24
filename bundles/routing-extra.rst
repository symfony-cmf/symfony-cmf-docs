RoutingExtraBundle
==================

The `RoutingExtraBundle <https://github.com/symfony-cmf/RoutingExtraBundle#readme>`_
integrates dynamic routing into Symfony using :doc:`../components/routing`.

The ``ChainRouter`` is meant to replace the default Symfony Router. All it does
is collect a prioritized list of routers and try to match requests and generate
urls with all of them. One of the routers in that chain can of course be the
default router so you can still use the standard way for some of your routes.

Additionally, this bundle delivers useful router implementations. Currently,
there is the ``DynamicRouter`` that routes based on a implemented repository that
provide Symfony2 Route objects. The repository can be implemented using a
database, for example with Doctrine `PHPCR-ODM`_ or Doctrine ORM. The bundle
provides a default implementation for Doctrine `PHPCR-ODM`_.

The DynamicRouter service is only made available when explicitly enabled in the
application configuration.

Finally this bundles provides route documents for Doctrine `PHPCR-ODM`_ and a
controller for redirection routes.

.. index:: RoutingExtraBundle
.. index:: Routing

Dependencies
------------

* `Symfony CMF routing <https://github.com/symfony-cmf/Routing#readme>`_

ChainRouter
-----------

The ChainRouter can replace the default symfony routing system with a chain-
enabled implementation. It does not route anything on its own, but only loops
through all chained routers. To handle standard configured symfony routes, the
symfony default router can be put into the chain.

Configuration
-------------

In your app/config/config.yml, you can specify which router services you want
to use. If you do not specify the routers_by_id map at all, by default the
chain router will just load the built-in symfony router. When you specify the
routers_by_id list, you need to have an entry for ``router.default`` if you
want the Symfony2 router (that reads the routes from app/config/routing.yml).

The format is ``service_name: priority`` - the higher the priority number the
earlier this router service is asked to match a route or to generate a url

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_routing_extra:
            chain:
                routers_by_id:
                    # enable the DynamicRouter with high priority to allow overwriting configured routes with content
                    symfony_cmf_routing_extra.dynamic_router: 200
                    # enable the symfony default router with a lower priority
                    router.default: 100
                # whether the chain router should replace the default router. defaults to true
                # if you set this to false, the router is just available as service
                # symfony_cmf_routing_extra.router and you  need to do something to trigger it
                # replace_symfony_router: true

Loading routers with tagging
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Your routers can automatically register, just add it as a service tagged with `router` and an optional `priority`.
The higher the priority, the earlier your router will be asked to match the route. If you do not specify the priority,
your router will come last.
If there are several routers with the same priority, the order between them is undetermined.
The tagged service will look like this

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

This implementation of a router loads routes from a RouteRepositoryInterface.
This interface can be easily implemented with Doctrine.

The router works with the base UrlMatcher and UrlGenerator classes and only
adds loading routes from the database and the concept of referenced content.

The DynamicRouter service is set up with a repository. See the configuration
section for how to change the route_repository_service and the following
section on more details for the default `PHPCR-ODM`_ based implementation.

You will want to configure the controller mappers that decide what controller
will be used to handle the request, to avoid hardcoding controller names into
your route documents.

The minimum configuration required to load the dynamic router as service
``symfony_cmf_routing_extra.dynamic_router`` is to have ``enabled: true`` in
your config.yml (the router is automatically enabled as soon as you add any
other configuration to the `dynamic` entry). Without enabling it, the dynamic
router service will not be loaded at all, allowing you to use the ChainRouter
with your own routers

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_routing_extra:
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
can write your own repository implementation to optimize (see cmf_routing.xml
for how to configure the service).

.. index:: PHPCR, ODM

Match Process
~~~~~~~~~~~~~

Most of the match process is described in the documentation of the `CMF Routing component`_.
The only difference is that the bundle will place the ``contentDocument`` in the request
attributes instead of the route defaults.

Your controllers can (and should) declare the parameter $contentDocument in their
``Action`` methods if they are supposed to work with content referenced by the routes.
See ``Symfony\Cmf\Bundle\ContentBundle\Controller\ContentController`` for an example.

.. _routing-controller-mapper:

Configuration
~~~~~~~~~~~~~

To configure the ControllerMappers, you can specify mappings. Presence of each
of the mappings makes the DI container inject the respective mapper into the
DynamicRouter.

The possible mappings are (in order of precedence):

* (Explicit controller): If there is a _controller set in getRouteDefaults(),
    it is used and no mapper is asked.
* Explicit template: requires the route document to return a '_template'
    parameter in getRouteDefaults. The configured generic controller is
    returned by the mapper.
* Controller by alias: requires the route document to return a 'type' value in
    getRouteDefaults()
* Controller by class: requires the route document to return an object for
    getRouteContent(). The content document is checked for being instanceof the
    class names in the map and if matched that controller is returned.
    Instanceof is used instead of direct lookup to work with proxy classes.
* Template by class: requires the route document to return an object for
    getRouteContent(). The content document is checked for being instanceof the
    class names in the map and if matched that template will be set as
    '_template' in the $defaults and return the configured generic controller


.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_routing_extra:
            dynamic:
                generic_controller: symfony_cmf_content.controller:indexAction
                controllers_by_alias:
                    editablestatic: sandbox_main.controller:indexAction
                controllers_by_class:
                    Symfony\Cmf\Bundle\ContentBundle\Document\StaticContent: symfony_cmf_content.controller::indexAction
                templates_by_class:
                    Symfony\Cmf\Bundle\ContentBundle\Document\StaticContent: SymfonyCmfContentBundle:StaticContent:index.html.twig

                # the repository is responsible to load routes
                # for `PHPCR-ODM`_, we mainly use this because it can map from url to repository path
                # an orm repository might need different logic. look at cmf_routing.xml for an example if you
                # need to define your own service
                manager_registry: doctrine_phpcr
                manager_name: default

                # if you use the default doctrine route repository service, you can use this to customize
                # the root path for the `PHPCR-ODM`_ RouteRepository
                # this base path will be injected by the Listener\IdPrefix - but only to routes
                # matching the prefix, to allow for more than one route source.
                routing_repositoryroot: /cms/routes

                # If you want to replace the default route or content reposititories
                # you can specify their service IDs here.
                route_repository_service_id: my_bundle.repository.endpoint
                content_repository_service_id: my_bundle.repository.endpoint

To see some examples, please look at the `CMF sandbox`_ and specifically the routing fixtures loading.


Sonata Admin Configuration
""""""""""""""""""""""""""

If ``sonata-project/doctrine-phpcr-admin-bundle`` is added to the composer.json
require section, the route documents are exposed in the SonataDoctrinePhpcrAdminBundle.
For instructions on how to configure this Bundle see :doc:`doctrine_phpcr_admin`.

By default, ``use_sonata_admin`` is automatically set based on whether
``SonataDoctrinePhpcrAdminBundle`` is available but you can explicitly disable it
to not have it even if sonata is enabled, or explicitly enable to get an error
if Sonata becomes unavailable.

You have a couple of configuration options for the admin. The ``content_basepath``
points to the root of your content documents.


.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_routing_extra:
            use_sonata_admin: auto # use true/false to force using / not using sonata admin
            content_basepath: ~ # used with sonata admin to manage content, defaults to symfony_cmf_core.content_basepath


Form Type
---------

The bundle defines a form type that can be used for classical "accept terms"
checkboxes where you place urls in the label. Simply specify
`symfony_cmf_routing_extra_terms_form_type` as the form type name and specify a
label and an array with content_ids in the options

.. code-block:: php

    $form->add('terms', 'symfony_cmf_routing_extra_terms_form_type', array(
        'label' => 'I have seen the <a href="%team%">Team</a> and <a href="%more%">More</a> pages ...',
        'content_ids' => array('%team%' => '/cms/content/static/team', '%more%' => '/cms/content/static/more')
    ));

The form type automatically generates the routes for the specified content and passes the routes to the trans twig helper for replacement
in the label.

Further notes
-------------

See the documentation of the `CMF Routing component`_ for information on the RouteObjectInterface,
redirections and locales.

Notes:

* RouteObjectInterface: The provided documents implement this interface to map content to routes
* Redirections: This bundle provides a RedirectController.

TODO: see DependencyInjection/Configuration.php of this bundle. I could not figure out how to set
this mapping as a default mapping. Meanwhile, in order to do redirections, you
need to add an entry to your mapping in the project configuration

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_routing_extra:
            controllers_by_class:
                Symfony\Cmf\Component\Routing\RedirectRouteInterface:  symfony_cmf_routing_extra.redirect_controller:redirectAction

Customize
---------

You can add more ControllerMapperInterface implementations if you have a case
not handled by the provided ones.

If you use an ODM / ORM different to `PHPCR-ODM`_, you probably need to specify
the class for the route entity (in `PHPCR-ODM`_, the class is automatically
detected). For more specific needs, have a look at DynamicRouter and see if you want to
extend it. You can also write your own routers to hook into the chain.

.. _`documentation for DependencyInjection tags`: http://symfony.com/doc/2.1/reference/dic_tags.html
.. _`CMF sandbox`: https://github.com/symfony-cmf/cmf-sandbox
.. _`CMF Routing component`: https://github.com/symfony-cmf/Routing
.. _`PHPCR-ODM`: https://github.com/doctrine/phpcr-odm

Learn more from the Cookbook
----------------------------

* :doc:`../cookbook/using-a-custom-route-repository`
